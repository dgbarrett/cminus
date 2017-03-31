#include "gen.h"

#define MAX_INSTRUCTIONS 1024
#define DMEM_MAX 1024

#include <stdio.h>
#include <stdlib.h>

#include "Instruction.h"
#include "tmconstants.h"
#include "DMemSymbol.h"
#include "Parameter.h"
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

int TMCode_getFunctionAddress(TMCode * tm, char * name) {
	return -1;
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
		if (tm -> instructions[i]) {
			printf("%04d:        ",i);
			Instruction_print(tm -> instructions[i]);
		} else break;
	}
}
/**/

char * getTmFilename(char * fname);
void genInitDMem(TMCode * tm);
void genLoadGlobals(TMCode * tm, ASTNode * root);
void genCallMain(TMCode * tm, ASTNode * root);
void genRuntimeExceptionHandlers(TMCode * tm);
void genStdlibFunctions(TMCode * tm);
void genFunctionCall(TMCode * tm, Symbol * mainFunction, FunctionParameter ** map);

void generateCode(ASTNode * root, char * fname) {
	/* char * tmFilename = getTmFilename(fname); */
	TMCode * tmcode = new_TMCode();

	genInitDMem(tmcode);
	genLoadGlobals(tmcode, root);
	genCallMain(tmcode, root);
	genRuntimeExceptionHandlers(tmcode);
	genStdlibFunctions(tmcode);

	TMCode_print(tmcode);
}

void genInitDMem(TMCode * tm) {
	Instruction * inst = storeRegister(0, 0, 0);
	Instruction_setComment(inst, "Zero out top of memory.");
	TMCode_addInstruction(tm, inst);
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

void genCallMain(TMCode * tm, ASTNode * root) {
	SymbolHashTable * ht = ASTNode_getEnclosingScope(root);
	Symbol * mainFunction = HashTable_get(ht, "main");

	if (mainFunction -> type != SYMBOL_FUNCTION) {
		fprintf(stderr, "No main exists\n");
		exit(0);
	}

	FunctionParameter ** map = createParameterMap(mainFunction -> signatureElems);
	int i = 0;
	for (i = 0 ; i < mainFunction -> signatureElems ; i++) {
		ParameterMap_addParam(map, i, REGISTER, 0);
	}

	genFunctionCall(tm, mainFunction, map);

	/*InstructionSequence * seq = new_InstructionSequence();

	int functionAddress = TMCode_getFunctionAddress(tm, "main");
	if (functionAddress < 0) {
		seq -> sequence[0] = jumpToUndeclaredFunction("main");
	} else {
		seq -> sequence[0] = jumpToFunction(functionAddress);
	}

	Instruction_setComment(seq -> sequence[0], "Initial jump to \"main\".");

	TMCode_addInstructionSequence(tm, seq);*/
}

void genFunctionCall(TMCode * tm, Symbol * func, FunctionParameter ** map) {
	InstructionSequence * seq = new_InstructionSequence();
	int seqItr = 0;
	int i = 0;

	/* Push parameters to the stack */
	for ( i = 0 ; map[i] ; i++) {
		if (map[i] -> isRegister) {
			seq -> sequence[seqItr++] = pushRegisterToStack(map[i] -> addr);
			Instruction_setComment(seq -> sequence[seqItr - 1], "Pushing register parameter to the stack.");
			seq -> sequence[seqItr++] = incrementRegister(SP);
			Instruction_setComment(seq -> sequence[seqItr - 1], "SP++.");
		}
	}

	/* Allocate space on stack for return value if needed */
	if (func -> datatype == TYPE_INT) {
		seq -> sequence[seqItr++] = tmallocate(1);
		Instruction_setComment(seq -> sequence[seqItr - 1], "Allocating space for return value.");
	}

	/* Push return address to stack */
	seq -> sequence[seqItr++] = loadRegisterWithPCOffset(REGISTER4, 3);
	Instruction_setComment(seq -> sequence[seqItr - 1], "Loading return address into temp register.");
	seq -> sequence[seqItr++] = pushRegisterToStack(REGISTER4);
	Instruction_setComment(seq -> sequence[seqItr - 1], "Pushing return address to the stack.");
	seq -> sequence[seqItr++] = incrementRegister(SP);
	Instruction_setComment(seq -> sequence[seqItr - 1], "SP++.");

	/* Jump to the function */
	int functionAddress = TMCode_getFunctionAddress(tm, func -> name);
	if (functionAddress < 0) seq -> sequence[seqItr++] = jumpToUndeclaredFunction(func -> name);
	else seq -> sequence[seqItr++] = jumpToFunction(functionAddress);

	char buf[128];
	sprintf(buf,"Jumping to \"%s\".", func -> name);
	Instruction_setComment(seq -> sequence[seqItr - 1], buf);

	TMCode_addInstructionSequence(tm, seq);
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

	seq -> sequence[0] = pushRegisterToStack(REGISTER0);
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

	seq -> sequence[0] = pushRegisterToStack(REGISTER0);
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