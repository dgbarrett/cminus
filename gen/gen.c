#include "gen.h"

#define MAX_INSTRUCTIONS 1024
#define DMEM_MAX 1024

#include <stdio.h>
#include <stdlib.h>

#include "Instruction.h"
#include "tmconstants.h"
#include "DMemSymbol.h"
#include "../semantics/symbtable.h"
#include "../semantics/symbtable_print.h"

/*** TMCode ***/
typedef struct _TMCode {
	int pc, sp;
	Instruction ** instructions;
} TMCode;

TMCode * new_TMCode() {
	TMCode * tm = malloc(sizeof(*tm));

	tm -> pc = 0;
	tm -> sp = 0;
	tm -> instructions = calloc(MAX_INSTRUCTIONS, sizeof(*(tm->instructions)));

	return tm;
}

void TMCode_addInstruction(TMCode * tm, Instruction * inst) {
	if (tm && inst) {
		if (tm -> pc + 1 < MAX_INSTRUCTIONS) {
			tm -> instructions[tm->pc++] = inst;
		}
	}
}

void TMCode_addInstructionSequence(TMCode * tm, InstructionSequence * seq) {
	if (tm && seq) {
		Instruction * inst = NULL;
		while((inst = InstructionSequence_next(seq))) {
			tm -> instructions[tm->pc++] = inst;
		}
	}
}

void TMCode_print(TMCode * tm) {
	int i = 0;
	for ( i = 0 ; i < MAX_INSTRUCTIONS ; i++) {
		if (tm -> instructions[i]) Instruction_print(tm -> instructions[i]);
		else break;
	}
}
/**/

char * getTmFilename(char * fname);
void genLoadGlobals(TMCode * tm, ASTNode * root);
void genRuntimeExceptionHandlers(TMCode * tm);
void genStdlibFunctions(TMCode * tm);

void generateCode(ASTNode * root, char * fname) {
	/* char * tmFilename = getTmFilename(fname); */
	TMCode * tmcode = new_TMCode();

	genLoadGlobals(tmcode, root);
	genRuntimeExceptionHandlers(tmcode);
	genStdlibFunctions(tmcode);

	TMCode_print(tmcode);
}

void genLoadGlobals(TMCode * tm, ASTNode * root) {
	int i = 0, totalAlloc = 0;
	SymbolHashTable * ht = ASTNode_getEnclosingScope(root);
	ASTNode ** globals = root -> children;

	for (i = 0 ; globals[i] ; i++) {
		int arrSize = 0, dMemAddr = totalAlloc;
		char * symbolName = globals[i] -> children[1] -> value.str;
		char * symbolType = globals[i] -> children[0] -> value.str;

		Symbol * symbol = HashTable_get(ht, symbolName);

		if (globals[i] -> type == VAR_DECLARATION) {
			totalAlloc++;
		} else if (globals[i] -> type == VAR_ARRAY_DECLARATION) {
			arrSize = globals[i] -> children[2] -> value.num;
			totalAlloc += arrSize;
		}

		if (symbol && (globals[i] -> type == VAR_ARRAY_DECLARATION || globals[i] -> type == VAR_DECLARATION)) {
			DMemSymbol * dmem = new_DMemSymbol(symbolName, symbolType, arrSize, dMemAddr);
			Symbol_associateDMemSymbol(symbol, dmem);
		}
	}
	
	if (totalAlloc > 0) {
		Instruction * inst = tmallocate(totalAlloc);
		Instruction_setComment(inst, "Allocation of space on stack for global variables.");
		TMCode_addInstruction(tm, inst);
	}
}

void genRuntimeExceptionHandlers(TMCode * tm) {
	InstructionSequence * seq = new_InstructionSequence();

	seq -> sequence[0] = loadRegisterWithCount(REGISTER0, EXCEPTION_DIV_BY_ZERO);
	seq -> sequence[1] = outputInteger(REGISTER0);
	seq -> sequence[2] = halt();
	seq -> sequence[3] = loadRegisterWithCount(REGISTER0, EXCEPTION_DMEM);
	seq -> sequence[4] = outputInteger(REGISTER0);
	seq -> sequence[5] = halt();
	seq -> sequence[6] = loadRegisterWithCount(REGISTER0, EXCEPTION_IMEM);
	seq -> sequence[7] = outputInteger(REGISTER0);
	seq -> sequence[8] = halt();

	seq -> sequence[0] -> function = new_TMFunction("HANDLE_EXCEPTION_DIV_BY_ZERO", INTERNAL);
	seq -> sequence[3] -> function = new_TMFunction("HANDLE_EXCEPTION_DMEM", INTERNAL);
	seq -> sequence[6] -> function = new_TMFunction("HANDLE_EXCEPTION_IMEM", INTERNAL);

	Instruction_setComment(seq -> sequence[0], "Start of internal function \"HANDLE_EXCEPTION_DIV_BY_ZERO\".");
	Instruction_setComment(seq -> sequence[3], "Start of internal function \"HANDLE_EXCEPTION_DMEM\".");
	Instruction_setComment(seq -> sequence[6], "Start of internal function \"HANDLE_EXCEPTION_IMEM\".");

	TMCode_addInstructionSequence(tm, seq);
}

void genStdlibFunctions(TMCode * tm) {
	/* int input(void) */
	InstructionSequence * seq = new_InstructionSequence();

	seq -> sequence[0] = storeRegisterOnStack(REGISTER0);
	seq -> sequence[1] = incrementRegister(SP);
	seq -> sequence[2] = readInteger(REGISTER0);
	seq -> sequence[3] = storeRegisterAtStackOffset(REGISTER0, -3);
	seq -> sequence[4] = restoreRegisterFromStack(REGISTER0);
	seq -> sequence[5] = decrementRegister(SP);
	seq -> sequence[6] = loadPC(SP, -1);

	seq -> sequence[0] -> function = new_TMFunction("input", CALLABLE);
	Instruction_setComment(seq -> sequence[0], "Start of callable function \"input\".");

	TMCode_addInstructionSequence(tm, seq);

	/* void output(int) */

	seq = new_InstructionSequence();

	seq -> sequence[0] = storeRegisterOnStack(REGISTER0);
	seq -> sequence[1] = incrementRegister(SP);
	seq -> sequence[2] = saveFramePointer();
	seq -> sequence[3] = loadParamIntoRegister(REGISTER0, 1, 0, 1);
	seq -> sequence[4] = outputInteger(REGISTER0);
	seq -> sequence[5] = decrementRegister(SP);
	seq -> sequence[6] = loadPC(SP, -1);

	seq -> sequence[0] -> function = new_TMFunction("output", CALLABLE);
	Instruction_setComment(seq -> sequence[0], "Start of callable function \"output\".");

	TMCode_addInstructionSequence(tm, seq);
}

char * getTmFilename(char * fname) {
	int i = 0;

	for ( i = 0 ; fname[i] != '\0'; i++) {
		if (fname[i] == '.') break;
	}

	fname[i+1] = 't';
	fname[i+2] = 'm';
	fname[i+3] = '\0';

	return fname;
}