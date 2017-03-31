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
	int i = 0;
	for (i = 0 ; i < MAX_INSTRUCTIONS ; i++) {
		if (tm -> instructions[i] && tm -> instructions[i] -> function) {
			if (strcmp(tm -> instructions[i] -> function -> name, name) == 0) return i;
		}
	}
	return -199;
}

int TMCode_getFunctionFinaleAddress(TMCode * tm, char * name) {
	int i = 0;
	for (i = 0 ; i < MAX_INSTRUCTIONS ; i++) {
		if (tm -> instructions[i] && tm -> instructions[i] -> finale) {
			if (strcmp(tm -> instructions[i] -> finale -> name, name) == 0) return i;
		}
	}
	return -199;
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

int isMainFunction(ASTNode * node) {
	return  node -> type == FUNCTION_DECLARATION &&
			strcmp(node -> children[1] -> value.str, "main") == 0;
}

char * getTmFilename(char * fname);
void genInitDMem(TMCode * tm);
void genLoadGlobals(TMCode * tm, ASTNode * root);
void genCallMain(TMCode * tm, ASTNode * root);
void genProgramEnd(TMCode * tm, char * filename);
void genRuntimeExceptionHandlers(TMCode * tm);
void genStdlibFunctions(TMCode * tm);
void genFunctionCall(TMCode * tm, Symbol * mainFunction, FunctionParameter ** map);
void genFunctionBody(TMCode * tm, ASTNode * function);
void genCompoundStatement(TMCode * tm, ASTNode * compoundStmt);
void genLocalVars(TMCode * tm, ASTNode * locals);
void genSaveRegisters(TMCode * tm, char * firstcommment);
void genReturnStatement(TMCode * tm, ASTNode * returnStmt);
void genRestoreRegisters(TMCode * tm, char * functionName);
void finishInstructions(TMCode * tm);
void getPendingAddresses(TMCode * tm, Instruction * inst);
char * removeFinaleQualifier(char * string);
int isFinale(char * string);

void generateCode(ASTNode * root, char * fname) {
	int i = 0;
	char * tmFilename = getTmFilename(fname); 
	TMCode * tmcode = new_TMCode();

	genInitDMem(tmcode);
	genLoadGlobals(tmcode, root);
	genCallMain(tmcode, root);
	genProgramEnd(tmcode, tmFilename);
	genRuntimeExceptionHandlers(tmcode);
	genStdlibFunctions(tmcode);

	/* Gen function body for main */
	for (i=0;root->children[i];i++) {
		if (isMainFunction(root -> children[i])) {
			genFunctionBody(tmcode, root -> children[i]);
		}
	}

	finishInstructions(tmcode);

	TMCode_print(tmcode);
}

void finishInstructions(TMCode * tm) {
	int i = 0;
	for ( i = 0 ; i < MAX_INSTRUCTIONS ; i++) {
		if (tm -> instructions[i]				&&
			(tm -> instructions[i] -> r_pending ||
			 tm -> instructions[i] -> s_pending ||
			 tm -> instructions[i] -> t_pending ) ){
			getPendingAddresses(tm,tm -> instructions[i]);
		}
	}
}

void getPendingAddresses(TMCode * tm, Instruction * inst) {
	if (inst -> r_pending) {
		if (!isFinale(inst -> r_pending)) {
			inst -> r = TMCode_getFunctionAddress(tm, inst -> r_pending) - inst -> loc - 1;
		} else {
			char * cleanName = removeFinaleQualifier(inst -> r_pending);
			inst -> r = TMCode_getFunctionFinaleAddress(tm, cleanName) - inst -> loc - 1;
		}
	}

	if (inst -> s_pending) {
		if (!isFinale(inst -> s_pending)) {
			inst -> s = TMCode_getFunctionAddress(tm, inst -> s_pending) - inst -> loc - 1;
		} else {
			char * cleanName = removeFinaleQualifier(inst -> s_pending);
			inst -> s = TMCode_getFunctionFinaleAddress(tm, cleanName) - inst -> loc - 1;
		}
	}

	if (inst -> t_pending) {
		if (!isFinale(inst -> t_pending)) {
			inst -> t = TMCode_getFunctionAddress(tm, inst -> t_pending) - inst -> loc - 1;
		} else {
			char * cleanName = removeFinaleQualifier(inst -> t_pending);
			inst -> t = TMCode_getFunctionFinaleAddress(tm, cleanName) - inst -> loc - 1;
		}
	}
}

char * removeFinaleQualifier(char * string) {
	int i = 0;
	for (i = strlen(string)-1; i >= 0 ; i--) {
		if (string[i] == '_') {
			string[i] = '\0';
			return string;
		}
	}
	return string;
}

int isFinale(char * string) {
	int i = 0;
	for (i = strlen(string)-1; i >= 0 ; i--) {
		if (string[i] == '_') {
			return string[i+1] == 'f' &&
				   string[i+2] == 'i' &&
				   string[i+3] == 'n' &&
				   string[i+4] == 'a' &&
				   string[i+5] == 'l' &&
				   string[i+6] == 'e';
		}
	}
	return 0;
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
			DMemSymbol * dmem = new_DMemSymbol(symbolName, symbolType, arrSize, dMemAddr, ABSOLUTE);
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
}

void genProgramEnd(TMCode * tm, char * filename) {
	Instruction * inst = halt();

	char buf[128];
	sprintf(buf, "Exit point for main function of \"%s\".", filename);

	Instruction_setComment(inst, buf);
	TMCode_addInstruction(tm, inst);
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
			tm -> sp++;
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
	tm -> sp++;

	TMCode_addInstructionSequence(tm, seq);

	seq = new_InstructionSequence();
	seqItr = 0;

	/* Jump to the function */
	int functionAddress = TMCode_getFunctionAddress(tm, func -> name);
	if (functionAddress < 0) seq -> sequence[seqItr++] = jumpToUndeclaredFunction(func -> name, tm -> pc);
	else {
		int jumpOffset = functionAddress - tm -> pc - 1;
		seq -> sequence[seqItr++] = jumpToPCOffset(jumpOffset);
	}

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

	Instruction_setComment(seq -> sequence[0], "[Function] Start of internal function \"HANDLE_EXCEPTION_DIV_BY_ZERO\".");
	Instruction_setComment(seq -> sequence[3], "[Function] Start of internal function \"HANDLE_EXCEPTION_DMEM\".");
	Instruction_setComment(seq -> sequence[6], "[Function] Start of internal function \"HANDLE_EXCEPTION_IMEM\".");
	Instruction_setComment(seq -> sequence[2], "Program exit via internal function \"HANDLE_EXCEPTION_DIV_BY_ZERO\".");
	Instruction_setComment(seq -> sequence[5], "Program exit via internal function \"HANDLE_EXCEPTION_DMEM\".");
	Instruction_setComment(seq -> sequence[8], "Program exit via internal function \"HANDLE_EXCEPTION_IMEM\".");

	TMCode_addInstructionSequence(tm, seq);
}

void genStdlibFunctions(TMCode * tm) {
	/* int input(void) */
	InstructionSequence * seq = new_InstructionSequence();

	seq -> sequence[0] = pushRegisterToStack(REGISTER0);
	
	seq -> sequence[1] = incrementRegister(SP);
	tm -> sp++;

	seq -> sequence[2] = readInteger(REGISTER0);
	seq -> sequence[3] = storeRegisterAtStackOffset(REGISTER0, -3);
	seq -> sequence[4] = restoreRegisterFromStack(REGISTER0);
	seq -> sequence[5] = decrementRegister(SP);
	seq -> sequence[6] = loadPC(SP, -1);

	seq -> sequence[0] -> function = new_TMFunction("input", CALLABLE);
	Instruction_setComment(seq -> sequence[0], "[Function] Start of callable function \"input\".");
	Instruction_setComment(seq -> sequence[6], "Returning from function \"input\".");

	TMCode_addInstructionSequence(tm, seq);

	/* void output(int) */

	seq = new_InstructionSequence();

	seq -> sequence[0] = pushRegisterToStack(REGISTER0);

	seq -> sequence[1] = incrementRegister(SP);
	tm -> sp++;

	seq -> sequence[2] = saveFramePointer();
	seq -> sequence[3] = loadParamIntoRegister(REGISTER0, 1, 0, 1);
	seq -> sequence[4] = outputInteger(REGISTER0);
	seq -> sequence[5] = decrementRegister(SP);
	seq -> sequence[6] = loadPC(SP, -1);

	seq -> sequence[0] -> function = new_TMFunction("output", CALLABLE);
	Instruction_setComment(seq -> sequence[0], "[Function] Start of callable function \"output\".");
	Instruction_setComment(seq -> sequence[6], "Returning from function \"output\".");

	TMCode_addInstructionSequence(tm, seq);
}

int functionHasLocals(ASTNode * cmpdStmt) {
	return cmpdStmt && cmpdStmt -> children[0] && cmpdStmt -> children[0] -> type == LOCAL_VARS;
}

int getLocalAlloc(ASTNode * cmpdStmt) {
	return cmpdStmt -> children[0] -> dataSize;
}

void genFunctionBody(TMCode * tm, ASTNode * function) {
	Instruction * inst = NULL;
	char buf[128];
	char * functionName = function -> children[1] -> value.str;

	/* Save the registers on the stack (0-4) */
	genSaveRegisters(tm, functionName);

	/* Save FRAME POINTER into FP */
	inst = saveFramePointer();
	sprintf(buf, "Saving frame pointer for call to \"%s\" into FP (R5).", functionName);
	Instruction_setComment(inst, buf);
	TMCode_addInstruction(tm, inst);

	genCompoundStatement(tm, function -> children[3]);

	/* Free local variables declared by compound stmt body */
	int finaleLocationSaved = 0;
	if (functionHasLocals(function -> children[3])) {
		int totalAlloc = getLocalAlloc(function -> children[3]);

		inst = decrementRegisterBy(SP, totalAlloc);
		inst -> finale = new_TMFinale(functionName);
		sprintf(buf, "Start of finale for \"%s\". Deallocating local vars", functionName);
		Instruction_setComment(inst, buf);

		TMCode_addInstruction(tm, inst);

		finaleLocationSaved = 1;
		tm -> sp -= totalAlloc;
	} 

	if (finaleLocationSaved) {
		genRestoreRegisters(tm, NULL);
	} else {
		genRestoreRegisters(tm, functionName);
	}

	inst = loadPC(SP, -1);

	sprintf(buf, "Returning from function \"%s\".", functionName);
	Instruction_setComment(inst, buf);
	TMCode_addInstruction(tm, inst);
}

void genCompoundStatement(TMCode * tm, ASTNode * compoundStmt) {
	if (!compoundStmt -> children[0]) return;
	
	if (compoundStmt -> children[0] -> type == LOCAL_VARS) {
		genLocalVars(tm, compoundStmt -> children[0]);
	}

	int i = 0;
	for ( i = 0 ; compoundStmt -> children[i] ; i++ ) {
		if (compoundStmt -> children[i] -> type == RETURN_STATEMENT) {
			genReturnStatement(tm, compoundStmt->children[i]);
		}
	}
}

void genLocalVars(TMCode * tm, ASTNode * locals) {
	int totalAlloc = 0;
	int i = 0;

	SymbolHashTable * ht = ASTNode_getEnclosingScope(locals);

	ASTNode * localVar = NULL;
	for (i = 0; (localVar = locals -> children[i]) ; i++) {
		int arrSize = 0, dMemAddr = totalAlloc;
		char * symbolName = localVar -> children[1] -> value.str;
		char * symbolType = localVar -> children[0] -> value.str;

		Symbol * symbol = HashTable_get(ht, symbolName);

		if (localVar -> type == VAR_DECLARATION) {
			totalAlloc++;
		} else if (localVar -> type == VAR_ARRAY_DECLARATION) {
			arrSize = localVar -> children[2] -> value.num;
			totalAlloc += arrSize;
		}

		if (symbol && 
			(localVar -> type == VAR_ARRAY_DECLARATION || localVar -> type == VAR_DECLARATION)) {
			DMemSymbol * dmem = new_DMemSymbol(symbolName, symbolType, arrSize, dMemAddr, FP_RELATIVE);
			Symbol_associateDMemSymbol(symbol, dmem);
		}
	}

	locals -> dataSize = totalAlloc;

	if (totalAlloc > 0) {
		Instruction * inst = tmallocate(totalAlloc);
		Instruction_setComment(inst, "Allocation of space on stack for local variables.");
		TMCode_addInstruction(tm, inst);
	}
}

void genReturnStatement(TMCode * tm, ASTNode * returnStmt) {
	Instruction * inst = NULL;
	ASTNode * function = ASTNode_getEnclosingFunction(returnStmt);
	char * functionName = function -> children[1] -> value.str;
	char buf[128];

	int finaleAddress = TMCode_getFunctionFinaleAddress(tm,functionName);

	/* if return statement returns a value. */
	if (returnStmt -> children[0]) {
		return;
	} else {
		if (finaleAddress < 0) {
			inst = jumpToUndeclaredFunctionFinale(functionName, tm -> pc);
		} else {
			int jumpOffset = finaleAddress - tm -> pc - 1;
			inst = jumpToPCOffset(jumpOffset);
		}

		sprintf(buf, "Jumping to finale for \"%s\".", functionName);
		Instruction_setComment(inst, buf);

		TMCode_addInstruction(tm, inst);
	}
}

void genSaveRegisters(TMCode * tm, char * name) {
	InstructionSequence * seq = new_InstructionSequence();

	seq -> sequence[0] = pushRegisterToStack(REGISTER0);

	if (name) {
		char buf[128];
		sprintf(buf, "[Function] Start of callable function \"%s\". Saving registers on stack.", name);
		seq -> sequence[0] -> function = new_TMFunction(name, CALLABLE);
		Instruction_setComment(seq -> sequence[0], buf);
	} else {
		Instruction_setComment(seq -> sequence[0], "Saving registers on the stack.");
	}

	seq -> sequence[1] = incrementRegister(SP);
	seq -> sequence[2] = pushRegisterToStack(REGISTER1);
	seq -> sequence[3] = incrementRegister(SP);
	seq -> sequence[4] = pushRegisterToStack(REGISTER2);
	seq -> sequence[5] = incrementRegister(SP);
	seq -> sequence[6] = pushRegisterToStack(REGISTER3);
	seq -> sequence[7] = incrementRegister(SP);
	seq -> sequence[8] = pushRegisterToStack(REGISTER4);

	seq -> sequence[9] = incrementRegister(SP);
	Instruction_setComment(seq -> sequence[9], "Done saving registers on the stack.");

	tm -> sp += 5;

	TMCode_addInstructionSequence(tm,seq);
}

void genRestoreRegisters(TMCode * tm, char * functionName) {
	InstructionSequence * seq = new_InstructionSequence();

	seq -> sequence[0] = restoreRegisterFromStack(REGISTER4);

	if (functionName) {
		char buf[128];
		sprintf(buf, "Start of finale for \"%s\". Restoring registers.", functionName);

		seq -> sequence[0] -> finale = new_TMFinale(functionName);
		Instruction_setComment(seq -> sequence[0], buf);
	}

	seq -> sequence[1] = decrementRegister(SP);
	seq -> sequence[2] = restoreRegisterFromStack(REGISTER3);
	seq -> sequence[3] = decrementRegister(SP);
	seq -> sequence[4] = restoreRegisterFromStack(REGISTER2);
	seq -> sequence[5] = decrementRegister(SP);
	seq -> sequence[6] = restoreRegisterFromStack(REGISTER1);
	seq -> sequence[7] = decrementRegister(SP);
	seq -> sequence[8] = restoreRegisterFromStack(REGISTER0);
	seq -> sequence[9] = decrementRegister(SP);

	tm -> sp -= 5;

	Instruction_setComment(seq -> sequence[9], "Done restoring registers from stack.");

	TMCode_addInstructionSequence(tm,seq);
}
/**/

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