#include "gen.h"

#include <stdio.h>
#include <stdlib.h>

#include "tmconstants.h"
#include "DMemSymbol.h"
#include "Parameter.h"
#include "../semantics/symbtable.h"
#include "../semantics/checking.h"

/*** gen functions ***/
void genInitDMem(TMCode * tm);
void genLoadGlobals(TMCode * tm, ASTNode * root);
void genCallMain(TMCode * tm, ASTNode * root);
void genProgramEnd(TMCode * tm, char * filename);
void genRuntimeExceptionHandlers(TMCode * tm); 
void genStdlibFunctions(TMCode * tm);
void genFunctionDefinition(TMCode * tm, ASTNode * function, int saveRegisters) ;
void finishInstructions(TMCode * tm);
void genFunctionCall(TMCode * tm, Symbol * func, SymbolHashTable * funcSymbols);
void genSaveRegisters(TMCode * tm, char * name);
void genRestoreRegisters(TMCode * tm, char * functionName);
void genCompoundStatement(TMCode * tm, ASTNode * compoundStmt, int registersSaved, int totalLocalAllocSize);
int  genLocalVars(TMCode * tm, ASTNode * locals, int totalLocalAllocSize);
void genReturnStatement(TMCode * tm, ASTNode * returnStmt, int registersSaved);
void genExpression(TMCode * tm, ASTNode * expression, int registerNum);
void genVarArrayElement(TMCode * expression, ASTNode * arrayElem, int registerNum); 
void genGetAddress(TMCode * tm, ASTNode * expression, int registerNum );
void genDivByZeroCheck(TMCode * tm, int registerNum);
void getPendingAddresses(TMCode * tm, Instruction * inst);
void genIfStatement(TMCode * tm, ASTNode * ifstmt, int registerNum, int totalLocalAllocSize);
void genStatement(TMCode * tm, ASTNode * stmt, int registerNum, int totalLocalAllocSize);
/**/

/*** util functions ***/
char * getTmFilename(char * fname);
char * removeFinaleQualifier(char * string);
int isFinale(char * string);
void ExitIfOutOfRegisterError(int registerNum);
/**/

/***** TOP LEVEL *****/
/*
	Function: generateCode
		Generate the code for the given AST. 
		fname is expected as "*.cm" and is used to name the "*.tm" file 
		produced by the compiler.
*/
TMCode * generateCode(ASTNode * root, char * fname) {
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
		if (ASTNode_isMainFunction(root -> children[i])) {
			genFunctionDefinition(tmcode, root -> children[i], REGISTER_PURE);
		}
	}

	/* Gen other function bodies */
	for (i=0;root->children[i];i++) {
		if (!ASTNode_isMainFunction(root -> children[i]) && root->children[i]->type == FUNCTION_DECLARATION) {
			genFunctionDefinition(tmcode, root -> children[i], REGISTER_PURE);
		}
	}

	finishInstructions(tmcode);

	TMCode_printToFile(tmcode, tmFilename);

	return tmcode;
}

void printTMCode(TMCode * tm) {
	printf("\nGenerated TM Assembly Code:\n");
	printf("---------------------------\n");

	TMCode_print(tm);

	printf("\n");
}
/*****/

/*
	Function: genInitDMem
		Generates code used to zero out the top of DMem before program 
		execution.
*/
void genInitDMem(TMCode * tm) {
	Instruction * inst = load(SP, 0, 0);
	Instruction_setComment(inst, "Loading SP with DMEM_MAX.");
	TMCode_addInstruction(tm, inst);

	inst = storeRegister(0, 0, 0);
	Instruction_setComment(inst, "Zeroing out the top of DMem.");
	TMCode_addInstruction(tm, inst);
}

/*
	Function: genLoadGlobals
		Generates the codes that load the allocates space for the global 
		variables on the stack.  Associates global names with Symbols so
		references to globals can be used easily.
*/
void genLoadGlobals(TMCode * tm, ASTNode * root) {
	int i = 0, totalAlloc = 0, globalCount = 0;
	char buf[128];
	SymbolHashTable * ht = ASTNode_getEnclosingScope(root);
	ASTNode ** globals = root -> children;

	for (i = 0 ; globals[i] ; i++) {
		int arrSize = 0, dMemAddr = totalAlloc;
		char * symbolName = globals[i] -> children[1] -> value.str;
		char * symbolType = globals[i] -> children[0] -> value.str;

		Symbol * symbol = HashTable_get(ht, symbolName);

		if (globals[i] -> type == VAR_DECLARATION) {
			totalAlloc++;
			globalCount++;
		} else if (globals[i] -> type == VAR_ARRAY_DECLARATION) {
			arrSize = globals[i] -> children[2] -> value.num;
			totalAlloc += arrSize;
			globalCount++;
		}

		if (symbol && (globals[i] -> type == VAR_ARRAY_DECLARATION || globals[i] -> type == VAR_DECLARATION)) {
			DMemSymbol * dmem = new_DMemSymbol(symbolName, symbolType, arrSize, dMemAddr, ABSOLUTE);
			Symbol_associateDMemSymbol(symbol, dmem);
		} 
	}
	
	if (totalAlloc > 0) {
		Instruction * inst = tmallocate(totalAlloc);
		sprintf(buf,"Allocating space on the stack for (%d) global variables.", globalCount);
		Instruction_setComment(inst, buf);
		TMCode_addInstruction(tm, inst);
	}
}

/*
	Function: genCallMain
		Generates the inital call to main that starts execution of the user 
		program.
*/
void genCallMain(TMCode * tm, ASTNode * root) {
	ASTNode * mainNode = AST_getMainNode(root);

	if (!mainNode) {
		fprintf(stderr,"[ERROR] No main function provided. Cannot continue.\n");
		exit(0);
	}

	int i = 0;
	SymbolHashTable * functionScope = ASTNode_getEnclosingScope(mainNode -> children[3]);
	Symbol * mainFunction = HashTable_get(functionScope, "main");

	/* Construct dummy ASTNode acting as initial call to main */
	ASTNode * mainCall = FunctionCall();
	FunctionCall_functionCalled(mainCall, Identifier("main"));
	mainCall -> scope = root -> scope;

	/* Construct ArgumentList (ASTNode) matching signature of main */
	ASTNode * arglist = ArgumentList();
	for (i = 0 ; i < mainFunction -> signatureElems ; i++) {
		/* Every param in main is initially zero */
		ArgumentList_append(arglist, Number("0"));
	}

	/* Set the signature of the function call with the ArgumentList */
	FunctionCall_arguments(mainCall, arglist);

	/* Generate the function call */
	/* The value returned from main (if one exists) is stored in REGISTER0 */
	genExpression(tm, mainCall, REGISTER0);

	/* output return value from main */
	/*if (mainFunction -> datatype == TYPE_INT) {
		Instruction * inst = outputInteger(REGISTER0);
		Instruction_setComment(inst, "Outputting return value from inital call to \"main\".");
		TMCode_addInstruction(tm, inst);
	}*/
}

/*
	Function: genProgramEnd
		Generates the exit point from the program, and all cleanup required 
		before program exit.
*/
void genProgramEnd(TMCode * tm, char * filename) {
	char buf[128];
	Instruction * inst = NULL;

	/*	Zero out registers, except for SP and PC */
	inst = loadRegisterWithCount(REGISTER0, 0);
	inst -> finale = new_TMFinale("!exit");
	Instruction_setComment(inst, "Start of program exit routine.");
	TMCode_addInstruction(tm, inst);
	inst = loadRegisterWithCount(REGISTER1, 0);
	Instruction_setComment(inst, "Zeroing out lower 6 registers.");
	TMCode_addInstruction(tm, inst);
	inst = loadRegisterWithCount(REGISTER2, 0);
	TMCode_addInstruction(tm, inst);
	inst = loadRegisterWithCount(REGISTER3, 0);
	TMCode_addInstruction(tm, inst);
	inst = loadRegisterWithCount(REGISTER4, 0);
	TMCode_addInstruction(tm, inst);
	inst = loadRegisterWithCount(FP, 0);
	TMCode_addInstruction(tm, inst);

	/* Zero out memory */
	/* Load DMEM_MAX in SP and zero out from top down. */
	inst = loadRegisterWithCount(SP, DMEM_MAX - 1);
	Instruction_setComment(inst, "Zeroing out DMem. SP = DMEM_MAX.");
	TMCode_addInstruction(tm, inst);

	inst = jumpIfLessThanZero(SP, 3);
	Instruction_setComment(inst, "Jump to zero out SP if done clearing DMem.");
	TMCode_addInstruction(tm, inst);

	inst = storeRegister(REGISTER0, 0,SP);
	Instruction_setComment(inst, "Register 0 is always 0 after above zeroing of registers.");
	TMCode_addInstruction(tm, inst);

	inst = decrementRegister(SP);
	Instruction_setComment(inst, "SP--.");
	TMCode_addInstruction(tm, inst);

	inst = decrementRegisterBy(PC, 4);
	Instruction_setComment(inst, "Jump back to top of DMem clearing loop.");
	TMCode_addInstruction(tm, inst);

	/* Zero out SP */
	inst = loadRegisterWithCount(SP, 0);
	Instruction_setComment(inst, "Zeroing out SP.");
	TMCode_addInstruction(tm, inst);

	/* Halt execution of the program (clean exit) */
	inst = halt();
	sprintf(buf, "Exit point for program (%s).", filename);
	Instruction_setComment(inst, buf);
	TMCode_addInstruction(tm, inst);
}

/*
	Function: genRuntimeExceptionHandlers
		Generates code snippets used to print runtime error "messages" and then
		exit from the program.
*/
void genRuntimeExceptionHandlers(TMCode * tm) {
	Instruction * inst = NULL;
	int finaleAddress = TMCode_getFunctionFinaleAddress(tm, "!exit");

	inst = loadRegisterWithCount(REGISTER0, EXCEPTION_DIV_BY_ZERO);
	inst -> function = new_TMFunction("HANDLE_EXCEPTION_DIV_BY_ZERO", INTERNAL);
	Instruction_setComment(inst, "Start of internal function \"HANDLE_EXCEPTION_DIV_BY_ZERO\".");
	TMCode_addInstruction(tm, inst);

	inst = outputInteger(REGISTER0);
	TMCode_addInstruction(tm, inst);

	inst = jumpToPCOffset(finaleAddress - tm -> pc - 1);
	Instruction_setComment(inst, "Jump to program exit routine.");
	TMCode_addInstruction(tm, inst);


	inst = loadRegisterWithCount(REGISTER0, EXCEPTION_DMEM);
	inst -> function = new_TMFunction("HANDLE_EXCEPTION_DMEM", INTERNAL);
	Instruction_setComment(inst, "Start of internal function \"HANDLE_EXCEPTION_DMEM\".");
	TMCode_addInstruction(tm, inst);

	inst = outputInteger(REGISTER0);
	TMCode_addInstruction(tm, inst);

	inst = jumpToPCOffset(finaleAddress - tm -> pc - 1);
	Instruction_setComment(inst, "Jump to program exit routine.");
	TMCode_addInstruction(tm, inst);

	inst = loadRegisterWithCount(REGISTER0, EXCEPTION_IMEM);
	inst -> function = new_TMFunction("HANDLE_EXCEPTION_IMEM", INTERNAL);
	Instruction_setComment(inst, "Start of internal function \"HANDLE_EXCEPTION_IMEM\".");
	TMCode_addInstruction(tm, inst);

	inst = outputInteger(REGISTER0);
	TMCode_addInstruction(tm, inst);

	inst = jumpToPCOffset(finaleAddress - tm -> pc - 1);
	Instruction_setComment(inst, "Jump to program exit routine.");
	TMCode_addInstruction(tm, inst);
}

/*
	Function: genStdlibFunctions
		Generates the code for the input and output functions.
*/
void genStdlibFunctions(TMCode * tm) {
	/* int input(void) */
	InstructionSequence * seq = new_InstructionSequence();

	seq -> sequence[0] = pushRegisterToStack(REGISTER0);
	
	seq -> sequence[1] = decrementRegister(SP);
	tm -> sp++;

	seq -> sequence[2] = readInteger(REGISTER0);
	seq -> sequence[3] = storeRegisterAtStackOffset(REGISTER0, 3);
	seq -> sequence[4] = restoreRegisterFromStack(REGISTER0);
	seq -> sequence[5] = incrementRegister(SP);
	seq -> sequence[6] = loadPC(SP, 1);

	seq -> sequence[0] -> function = new_TMFunction("input", CALLABLE);
	Instruction_setComment(seq -> sequence[0], "Start of callable function \"input\".");
	Instruction_setComment(seq -> sequence[6], "Returning from function \"input\".");

	TMCode_addInstructionSequence(tm, seq);

	/* void output(int) */

	seq = new_InstructionSequence();

	seq -> sequence[0] = pushRegisterToStack(REGISTER0);

	seq -> sequence[1] = decrementRegister(SP);
	tm -> sp++;

	seq -> sequence[2] = loadRegisterFromSP(REGISTER0, 3);
	seq -> sequence[3] = outputInteger(REGISTER0);
	seq -> sequence[4] = incrementRegister(SP);
	seq -> sequence[5] = loadPC(SP, 1);

	seq -> sequence[0] -> function = new_TMFunction("output", CALLABLE);
	Instruction_setComment(seq -> sequence[0], "Start of callable function \"output\".");
	Instruction_setComment(seq -> sequence[6], "Returning from function \"output\".");

	TMCode_addInstructionSequence(tm, seq);
}

/*
	Function: genFunctionDefinition
		Generates the asm code that defines a function.
*/
void genFunctionDefinition(TMCode * tm, ASTNode * function, int saveRegisters) {
	Instruction * inst = NULL;
	char buf[128];
	char * functionName = function -> children[1] -> value.str;

	/* Create DMem symbols for params */
	SymbolHashTable * funcSymbols = ASTNode_getEnclosingScope(function -> children[3]);
	Symbol * func = HashTable_get(funcSymbols, functionName);

	int registersSaved = (saveRegisters) ? 6: 0;
	int returnsInt = (func -> datatype == TYPE_INT) ? 1 : 0;
	int numParams = func -> signatureElems;
	int i = 0;

	ASTNode * paramList = function -> children[2];
	char ** paramNames = ParameterList_getParamNames(paramList);

	/* Generate DMem symbols for params */
	for ( i = 0 ; paramNames[i] ; i++) {
		Symbol * paramSymbol = HashTable_get(funcSymbols, paramNames[i]);
		int dMemAddr = (-1*(registersSaved + 1 + returnsInt + numParams)) + i;

		if (paramSymbol -> datatype == TYPE_INT) {
			paramSymbol -> dmem = new_DMemSymbol(paramNames[i], "int", 0, dMemAddr, FP_RELATIVE);
		} else if ( paramSymbol -> datatype == TYPE_VOID ) {
			paramSymbol -> dmem = new_DMemSymbol(paramNames[i], "void", 0, dMemAddr, FP_RELATIVE);
		} else if ( paramSymbol -> datatype == TYPE_INTARR) {
			paramSymbol -> dmem = new_DMemSymbol(paramNames[i], "int[]", 0, dMemAddr, FP_RELATIVE);
		} else if ( paramSymbol -> datatype == TYPE_VOIDARR) {
			paramSymbol -> dmem = new_DMemSymbol(paramNames[i], "void[]", 0, dMemAddr, FP_RELATIVE);
		}
	}

	/* Save the registers on the stack (0-4) */
	if (saveRegisters) {
		genSaveRegisters(tm, functionName);
	}

	/* Save FRAME POINTER into FP (Register 5) */
	inst = saveFramePointer();
	if (saveRegisters) {
		sprintf(buf, "Saving frame pointer for call to \"%s\" into FP (R5).", functionName);
	} else {
		inst -> function = new_TMFunction(functionName, CALLABLE);
		sprintf(buf, "Start of callable function \"%s\". Saving FP.", functionName);
	}
	Instruction_setComment(inst, buf);
	TMCode_addInstruction(tm, inst);

	genCompoundStatement(tm, function -> children[3], (saveRegisters) ? 6 : 0, 0);

	int finaleLocationSaved = 0;
	if (CompoundStatement_hasLocals(function -> children[3])) {
		if (saveRegisters) {
			genRestoreRegisters(tm, NULL);
		}
		finaleLocationSaved=1;
	} else {
		if (saveRegisters) {
			genRestoreRegisters(tm, functionName);
			finaleLocationSaved = 1;
		}
	}
	

	inst = loadPC(SP, 1);

	if (finaleLocationSaved) {
		sprintf(buf, "Returning from function \"%s\".", functionName);
	} else {
		inst -> finale = new_TMFinale(functionName);
		sprintf(buf, "Finale for function \"%s\". Returning from function.", functionName);
	}
	
	Instruction_setComment(inst, buf);
	TMCode_addInstruction(tm, inst);
}

/*
	Function: finishInstructions
		Look for placeholder labels in the Instructions in the TMCode and 
		replace them with the appropriate address/offset.
*/
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

/*
	Function: genFunctionCall
		Generates the code used to call a function.  func gives the address of
		the jump required for the call. map gives the locations of the 
		parameters (in memory or in register).  funcSymbols gives the symbols
		in scope at the time of the call.
 */
void genFunctionCall(TMCode * tm, Symbol * func, SymbolHashTable * funcSymbols) {
	Instruction * inst = NULL;
	char buf[128];

	/* Allocate space on stack for return value if needed */
	if (func -> datatype == TYPE_INT) {
		inst = tmallocate(1);
		sprintf(buf, "Allocating space for return value from \"%s\".", func -> name);
		Instruction_setComment(inst, buf);
		TMCode_addInstruction(tm,inst);
	}

	/* Push return address to stack */
	inst = loadRegisterWithPCOffset(REGISTER4, 3);
	sprintf(buf, "Loading return address for call to \"%s\" into REGISTER4.",func -> name);
	Instruction_setComment(inst, buf);
	TMCode_addInstruction(tm,inst);

	inst = pushRegisterToStack(REGISTER4);
	sprintf(buf, "Pushing return address for call to \"%s\" to the stack.", func -> name);
	Instruction_setComment(inst, buf);
	TMCode_addInstruction(tm,inst);

	inst = decrementRegister(SP);
	Instruction_setComment(inst, "SP++.");
	TMCode_addInstruction(tm,inst);
	tm -> sp++;

	/* Jump to the function */
	int functionAddress = TMCode_getFunctionAddress(tm, func -> name);
	if (functionAddress < 0) inst = jumpToUndeclaredFunction(func -> name, tm -> pc);
	else {
		int jumpOffset = functionAddress - tm -> pc - 1;
		inst = jumpToPCOffset(jumpOffset);
	}

	sprintf(buf,"Jumping to \"%s\".", func -> name);
	Instruction_setComment(inst, buf);

	TMCode_addInstruction(tm, inst);
}

/*
	Function: genSaveRegisters
		Generate the code used to save the registers (0-5) on the stack.
		If name is non null, the first instruction in the register save 
		sequence is marked as the entry point for the function identified 
		by name.
*/
void genSaveRegisters(TMCode * tm, char * name) {
	InstructionSequence * seq = new_InstructionSequence();

	seq -> sequence[0] = pushRegisterToStack(REGISTER0);

	if (name) {
		char buf[128];
		sprintf(buf, "Start callable function \"%s\". Saving registers on stack.", name);
		seq -> sequence[0] -> function = new_TMFunction(name, CALLABLE);
		Instruction_setComment(seq -> sequence[0], buf);
	} else {
		Instruction_setComment(seq -> sequence[0], "Saving registers on the stack.");
	}

	seq -> sequence[1] = decrementRegister(SP);
	seq -> sequence[2] = pushRegisterToStack(REGISTER1);
	seq -> sequence[3] = decrementRegister(SP);
	seq -> sequence[4] = pushRegisterToStack(REGISTER2);
	seq -> sequence[5] = decrementRegister(SP);
	seq -> sequence[6] = pushRegisterToStack(REGISTER3);
	seq -> sequence[7] = decrementRegister(SP);
	seq -> sequence[8] = pushRegisterToStack(REGISTER4);
	seq -> sequence[9] = decrementRegister(SP);
	seq -> sequence[10] = pushRegisterToStack(FP);
	seq -> sequence[11] = decrementRegister(SP);
	Instruction_setComment(seq -> sequence[11], "Done saving registers on the stack.");

	tm -> sp += 5;

	TMCode_addInstructionSequence(tm,seq);
}

/*
	Function: genRestoreRegisters
		Generate the code used to save the registers (0-5) on the stack.
		If functionName is non null, the first instruction in the register 
		restore sequence is marked as the entry point for the finale of the 
		function identified by functionName.
*/
void genRestoreRegisters(TMCode * tm, char * functionName) {
	InstructionSequence * seq = new_InstructionSequence();

	seq -> sequence[0] = restoreRegisterFromStack(FP);

	if (functionName) {
		char buf[128];
		sprintf(buf, "Start of finale for \"%s\". Restoring registers.", functionName);

		seq -> sequence[0] -> finale = new_TMFinale(functionName);
		Instruction_setComment(seq -> sequence[0], buf);
	}

	seq -> sequence[1] = incrementRegister(SP);
	seq -> sequence[2] = restoreRegisterFromStack(REGISTER4);
	seq -> sequence[3] = incrementRegister(SP);
	seq -> sequence[4] = restoreRegisterFromStack(REGISTER3);
	seq -> sequence[5] = incrementRegister(SP);
	seq -> sequence[6] = restoreRegisterFromStack(REGISTER2);
	seq -> sequence[7] = incrementRegister(SP);
	seq -> sequence[8] = restoreRegisterFromStack(REGISTER1);
	seq -> sequence[9] = incrementRegister(SP);
	seq -> sequence[10] = restoreRegisterFromStack(REGISTER0);
	seq -> sequence[11] = incrementRegister(SP);

	tm -> sp -= 5;

	Instruction_setComment(seq -> sequence[11], "Done restoring registers from stack.");

	TMCode_addInstructionSequence(tm,seq);
}

/*
	Function: genCompoundStatement
		Generate the code for a compound statement.
*/
void genCompoundStatement(TMCode * tm, ASTNode * compoundStmt, int registersSaved, int totalLocalAllocSize) {
	if (!compoundStmt -> children[0]) return;
	
	if (compoundStmt -> children[0] -> type == LOCAL_VARS) {
		totalLocalAllocSize = genLocalVars(tm, compoundStmt -> children[0], totalLocalAllocSize);
	}

	int i = 0;
	for ( i = 0 ; compoundStmt -> children[i] ; i++ ) {
		genStatement(tm, compoundStmt->children[i], REGISTER0, totalLocalAllocSize);
	}

	ASTNode * function = ASTNode_getEnclosingFunction(compoundStmt);
	char * functionName = function -> children[1] -> value.str;

	/* Free local variables declared by compound stmt body */
	if (CompoundStatement_hasLocals(compoundStmt)) {
		char buf[128];
		int totalAlloc = CompoundStatement_getLocalAllocSize(compoundStmt);
		Instruction * inst = incrementRegisterBy(SP, totalAlloc);
		
		inst -> finale = new_TMFinale(functionName);
		sprintf(buf, "Start of finale for \"%s\". Deallocating local vars.", functionName);
		Instruction_setComment(inst, buf);

		TMCode_addInstruction(tm, inst);

		tm -> sp -= totalAlloc;
	} 
}

/*
	Function: genIfStatement
*/
void genIfStatement(TMCode * tm, ASTNode * ifstmt, int registerNum, int totalLocalAllocSize) {
	char buf[128];
	/* generate if expression */
	genExpression(tm, ifstmt -> children[0], registerNum);
	
	int elseJumpPC = tm -> pc;
	/* jump to else body (or next code if no else body) when if expression == 0 */
	Instruction * elseBodyJump = jumpIfEqualsZero(registerNum, -1);
	TMCode_addInstruction(tm,elseBodyJump);

	/* generate if body */
	genStatement(tm, ifstmt -> children[1], registerNum, totalLocalAllocSize);

	if (ifstmt -> children[2]) {
		int jumpToNextPC = tm -> pc;
		Instruction * jumpToNext = loadRegisterWithPCOffset(PC, -1);
		TMCode_addInstruction(tm,jumpToNext);
		
		int elseBodyPC = tm -> pc;
		genStatement(tm, ifstmt -> children[2], registerNum, totalLocalAllocSize);


		elseBodyJump -> s = elseBodyPC - elseJumpPC - 1;
		jumpToNext -> s = tm -> pc - jumpToNextPC - 1;

		sprintf(buf,"Jump to instruction after if/else statement (PC + %d).", jumpToNext -> s);
		Instruction_setComment(jumpToNext, buf);
	} else {
		elseBodyJump -> s = tm -> pc - elseJumpPC - 1;
	}

	if (ifstmt -> children[2]) {
		sprintf(buf, "if (REGISTER%d == 0) Jump to else body at PC + %d.", registerNum, elseBodyJump -> s);
	} else {
		sprintf(buf, "if (REGISTER%d == 0) Jump to instruction after if statement (PC + %d).", registerNum,elseBodyJump -> s);
	}

	Instruction_setComment(elseBodyJump, buf);

}

void genWhileLoop(TMCode * tm, ASTNode * stmt, int registerNum, int totalLocalAllocSize) {
	char buf[128];	
	int genConditionPC = tm -> pc;
	genExpression(tm, stmt -> children[0], registerNum);

	int loopBodyPC = tm -> pc;
	Instruction * jumpPastLoop = jumpIfEqualsZero(registerNum, -1);
	TMCode_addInstruction(tm, jumpPastLoop);

	genStatement(tm, stmt -> children[1], registerNum, totalLocalAllocSize);

	Instruction * jumpToGenCond = loadRegisterWithPCOffset(PC, genConditionPC - tm->pc - 1);
	sprintf(buf,"Jump to top of loop (PC - %d).", -1*jumpToGenCond -> s);
	Instruction_setComment(jumpToGenCond, buf);
	TMCode_addInstruction(tm, jumpToGenCond);

	jumpPastLoop -> s = tm->pc - loopBodyPC - 1;

	sprintf(buf,"if (REGISTER%d == 0) Jump to instruction after loop body (PC + %d)", registerNum, jumpPastLoop->s);
	Instruction_setComment(jumpPastLoop, buf);
}

void genStatement(TMCode * tm, ASTNode * stmt, int registerNum, int totalLocalAllocSize) {
	switch (stmt -> type) {
		case RETURN_STATEMENT:
			genReturnStatement(tm,stmt, 6);
			break;
		case EXPRESSION:
		case FUNCTION_CALL:
			genExpression(tm, stmt, registerNum);
			break;
		case IF_STATEMENT:
			genIfStatement(tm, stmt, registerNum, totalLocalAllocSize);
			break;
		case COMPOUND_STATEMENT:
			genCompoundStatement(tm, stmt, 6, totalLocalAllocSize);
			break;
		case WHILE_LOOP:
			genWhileLoop(tm, stmt, registerNum, totalLocalAllocSize);
		default:;
	}
}

/*
	Function: genLocalVars
		Generate the code for to allocate space local symbols and save their 
		relative addresses for later use.
*/
int genLocalVars(TMCode * tm, ASTNode * locals, int totalLocalAllocSize) {
	int totalAlloc = 0;
	int varCount = 0;
	int i = 0;
	char buf[128];

	SymbolHashTable * ht = ASTNode_getEnclosingScope(locals);

	ASTNode * localVar = NULL;
	for (i = 0; (localVar = locals -> children[i]) ; i++) {
		int arrSize = 0, dMemAddr = totalLocalAllocSize + totalAlloc;
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

		varCount++;
	}

	locals -> dataSize = totalAlloc;

	if (totalAlloc > 0) {
		Instruction * inst = tmallocate(totalAlloc);
		sprintf(buf, "Allocating space on the stack for (%d) local variables.", varCount);
		Instruction_setComment(inst, buf);
		TMCode_addInstruction(tm, inst);
	}

	return totalLocalAllocSize + totalAlloc;
}

/*
	Function: genReturnStatement
		Generate the code for a return statement.
		registersSaved is the number of registers saved by the containg 
		function upon its call.
*/
void genReturnStatement(TMCode * tm, ASTNode * returnStmt, int registersSaved) {
	Instruction * inst = NULL;
	ASTNode * function = ASTNode_getEnclosingFunction(returnStmt);
	char * functionName = function -> children[1] -> value.str;
	char buf[128];

	/* if return statement returns a value. */
	if (returnStmt -> children[0]) {
		genExpression(tm, returnStmt -> children[0], REGISTER0);
		inst = storeReturnValueOnStack(REGISTER0, registersSaved);

		sprintf(buf, "Storing value returned from \"%s\" on stack.", functionName);
		Instruction_setComment(inst, buf);
		TMCode_addInstruction(tm, inst);
	} 

	/* Gen the jump that is the return */
	int finaleAddress = TMCode_getFunctionFinaleAddress(tm,functionName);
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

void genFunctionCall2(TMCode * tm, ASTNode * expression, int registerNum);

/*
	Function: genExpression
		Generate the code for an expression, and save the result of the 
		expression in registerNum.
*/
void genExpression(TMCode * tm, ASTNode * expression, int registerNum) {
	if (tm && expression) {
		Instruction * inst = NULL;
		SymbolHashTable * scope = ASTNode_getEnclosingScope(expression);
		Symbol * symbol = NULL, *symbol2 = NULL;
		char buf[128];
		char op[5];

		strcpy(op, "???");

		switch ( expression -> type ) {
			case NUMBER:
				inst = loadRegisterWithCount(registerNum, expression -> value.num);
				sprintf(buf, "REGISTER%d = %d", registerNum, expression -> value.num);
				Instruction_setComment(inst, buf);
				TMCode_addInstruction(tm, inst);
				break;
			case IDENTIFIER:
				symbol = HashTable_get(scope, expression -> value.str);
				if (symbol) {
					if (symbol -> datatype == TYPE_INTARR || symbol -> datatype == TYPE_VOIDARR) {
						genGetAddress(tm, expression, registerNum);
					} else {
						if (symbol -> dmem) {
							if (symbol -> dmem -> addressType == FP_RELATIVE) {
								inst = loadRegisterFromFP(registerNum, -symbol -> dmem -> dMemAddr);
								sprintf(buf, "REGISTER%d = %s", registerNum, symbol -> name);
								Instruction_setComment(inst, buf);
								TMCode_addInstruction(tm, inst);
							} else if (symbol -> dmem -> addressType == ABSOLUTE) {
								inst = loadRegisterWithCount(registerNum, symbol -> dmem -> dMemAddr);
								sprintf(buf, "REGISTER%d = &%s", registerNum, symbol -> name);
								Instruction_setComment(inst, buf);
								TMCode_addInstruction(tm, inst);

								inst = load(registerNum, 0, registerNum);
								sprintf(buf, "REGISTER%d = &%s", registerNum, symbol -> name);
								Instruction_setComment(inst, buf);
								TMCode_addInstruction(tm, inst);
							}
						} 
					}
				}
				break;
			case EXPRESSION:
				ExitIfOutOfRegisterError(registerNum);
				switch( expression -> value.operation ) {
					case ASSIGN:
						genGetAddress(tm, expression -> children[0], registerNum);
						genExpression(tm, expression -> children[1], registerNum + 1);

						if (expression -> children[0] -> type == IDENTIFIER) {
							symbol = HashTable_get(scope, expression -> children[0] -> value.str);

							if (symbol -> datatype == TYPE_INTARR || symbol -> datatype == TYPE_VOIDARR) {
								symbol2 = HashTable_get(scope, expression -> children[1] -> value.str);

								symbol -> dmem -> dMemAddr = symbol2 -> dmem -> dMemAddr;
								symbol -> dmem -> addressType = symbol2 -> dmem -> addressType;
								/* probably should copy length too once i figure that out */
								return;
							}
						}

						inst = storeRegister(registerNum + 1, 0, registerNum);
						sprintf(buf, "%s = REGISTER%d", getExpressionString(expression -> children[0]), registerNum + 1);
						Instruction_setComment(inst, buf);
						TMCode_addInstruction(tm, inst);

						inst = loadAddress(registerNum, 0, registerNum + 1);
						sprintf(buf, "REGISTER%d = %s. Saving result of expression.", registerNum, getExpressionString(expression -> children[0]));
						Instruction_setComment(inst,buf);
						TMCode_addInstruction(tm, inst);
						break;
					case ADD:
					case SUB:
					case MUL:
					case DIV:
						genExpression(tm, expression -> children[0], registerNum);
						genExpression(tm, expression -> children[1], registerNum + 1);

						if (expression -> value.operation == ADD) {
							inst = addRegisters(registerNum, registerNum, registerNum + 1);
							strcpy(op, "+");
						} else if (expression -> value.operation == SUB) {
							inst = subtractRegisters(registerNum, registerNum, registerNum + 1);
							strcpy(op, "-");
						} else if (expression -> value.operation == MUL) {
							inst = multiplyRegisters(registerNum, registerNum, registerNum + 1);
							strcpy(op, "*");
						} else if (expression -> value.operation == DIV) {
							genDivByZeroCheck(tm, registerNum + 1);
							inst = divideRegisters(registerNum, registerNum, registerNum + 1);
							strcpy(op, "/");
						} 

						sprintf(buf, "REGISTER%d %s= REGISTER%d", registerNum, op, registerNum + 1);
						Instruction_setComment(inst, buf);

						TMCode_addInstruction(tm, inst);
						break;
					case NEQ:
						genExpression(tm, expression -> children[0], registerNum);
						genExpression(tm, expression -> children[1], registerNum + 1);

						inst = subtractRegisters(registerNum, registerNum, registerNum + 1);
						sprintf(buf, "REGISTER%d -= REGISTER%d", registerNum, registerNum + 1);
						Instruction_setComment(inst, buf);
						TMCode_addInstruction(tm, inst);

						inst = jumpIfEqualsZero(registerNum, 1);
						sprintf(buf, "if (REGISTER%d == 0) Jump to PC + 1.", registerNum);
						Instruction_setComment(inst, buf);
						TMCode_addInstruction(tm, inst);

						inst = loadRegisterWithCount(registerNum, EXPRESSION_TRUE);
						sprintf(buf, "REGISTER%d = %d", registerNum, EXPRESSION_TRUE);
						Instruction_setComment(inst, buf);
						TMCode_addInstruction(tm, inst);
						break;
					case EQ:
					case GT:
					case GE:
					case LT:
					case LE:
						genExpression(tm, expression -> children[0], registerNum);
						genExpression(tm, expression -> children[1], registerNum + 1);

						inst = subtractRegisters(registerNum, registerNum, registerNum + 1);
						sprintf(buf, "REGISTER%d -= REGISTER%d", registerNum, registerNum + 1);
						Instruction_setComment(inst, buf);
						TMCode_addInstruction(tm, inst);

						if (expression -> value.operation == EQ) {
							inst = jumpIfNotEqualsZero(registerNum, 2);
							strcpy(op, "!=");
						} else if (expression -> value.operation == GT) {
							inst = jumpIfLessThanEqualZero(registerNum, 2);
							strcpy(op, "<=");
						} else if (expression -> value.operation == GE) {
							inst = jumpIfLessThanZero(registerNum, 2);
							strcpy(op, "<");
						} else if (expression -> value.operation == LT) {
							inst = jumpIfGreaterThanEqualZero(registerNum, 2);
							strcpy(op, ">=");
						} else if (expression -> value.operation == LE) {
							inst = jumpIfGreaterThanZero(registerNum, 2);
							strcpy(op, ">");
						} 

						sprintf(buf, "if (REGISTER%d %s 0) jump to PC + 2.", registerNum, op);
						Instruction_setComment(inst, buf);
						TMCode_addInstruction(tm, inst);

						inst = loadRegisterWithCount(registerNum, EXPRESSION_TRUE);
						sprintf(buf, "REGISTER%d = %d", registerNum, EXPRESSION_TRUE);
						Instruction_setComment(inst, buf);
						TMCode_addInstruction(tm, inst);

						inst = incrementRegister(PC);
						Instruction_setComment(inst, "Jump to PC + 1.");
						TMCode_addInstruction(tm, inst);

						inst = loadRegisterWithCount(registerNum, EXPRESSION_FALSE);
						sprintf(buf, "REGISTER%d = %d", registerNum, EXPRESSION_FALSE);
						Instruction_setComment(inst, buf);
						TMCode_addInstruction(tm, inst); 
						break;
					default:;
				}
				break;
			case FUNCTION_CALL:
				genFunctionCall2(tm, expression, registerNum);
				break;
			case VAR_ARRAY_ELEMENT:
				genVarArrayElement(tm, expression, registerNum);
				break;
			default:;
		}
	}
}

void genVarArrayElement(TMCode * tm, ASTNode * arrayElem, int registerNum) {
	char buf[128];
	genGetAddress(tm, arrayElem -> children[0], registerNum);

	Instruction * inst = NULL;
	ASTNode * index = arrayElem -> children[1];
	if (index -> type == NUMBER) {
		inst = decrementRegisterBy(registerNum, index -> value.num);
		Instruction_setComment(inst, "Moving to correct positon in array.");
		TMCode_addInstruction(tm,inst);
	} else {
		genExpression(tm, index, registerNum + 1);

		inst = subtractRegisters(registerNum, registerNum, registerNum + 1);
		sprintf(buf, "REGISTER%d -= REGISTER%d", registerNum, registerNum + 1);
		Instruction_setComment(inst, buf);
		TMCode_addInstruction(tm, inst);
	}

	inst = load(registerNum, 0, registerNum);
	sprintf(buf, "REGISTER%d = %s", registerNum, getExpressionString(arrayElem));
	Instruction_setComment(inst, buf);
	TMCode_addInstruction(tm,inst);
}

void genFunctionCall2(TMCode * tm, ASTNode * expression, int registerNum) {
	char buf[128];
	int i = 0, signatureLen;
	Instruction * inst = NULL;
	char * functionName = expression -> children[0] -> value.str;
	SymbolHashTable * functionScope = ASTNode_getEnclosingScope(expression);
	Symbol * functionSymbol = HashTable_get(functionScope, functionName);
	signatureLen = functionSymbol -> signatureElems;


	for (i=0 ; i < signatureLen ; i++) {
		/* Save value of argument in registerNum */
		genExpression(tm, expression -> children[1] -> children[i], registerNum);

		/* Push argument to stack */
		inst = pushRegisterToStack(registerNum);
		sprintf(buf, "Pushing REGISTER%d to stack as param %d for call to \"%s\".", registerNum, i, functionName);
		Instruction_setComment(inst, buf);
		TMCode_addInstruction(tm, inst);

		inst = decrementRegister(SP);
		Instruction_setComment(inst, "SP++.");
		TMCode_addInstruction(tm,inst);
		tm -> sp++;
	}

	genFunctionCall(tm, functionSymbol, functionScope);

	if (functionSymbol -> datatype == TYPE_INT) {
		int retValOffset = 2;
		inst = loadRegisterFromSP(registerNum, retValOffset);
		sprintf(buf, "Loading return value from call to \"%s\" into REGISTER%d.", functionName, registerNum);
		Instruction_setComment(inst, buf);
		TMCode_addInstruction(tm,inst);

		inst = incrementRegisterBy(SP,2+functionSymbol->signatureElems);
		Instruction_setComment(inst, "Cleanup from function call.");
		TMCode_addInstruction(tm, inst);
	} else {
		inst = incrementRegisterBy(SP,1+functionSymbol->signatureElems);
		Instruction_setComment(inst, "Cleanup from function call.");
		TMCode_addInstruction(tm, inst);
	}
}

/*
	Function: genGetAddress
		Generates code which loads the address of the expression into the
		register specified by registerNum
*/
void genGetAddress(TMCode * tm, ASTNode * expression, int registerNum ) {
	Instruction * inst = NULL;
	SymbolHashTable * scope = ASTNode_getEnclosingScope(expression);
	Symbol * symbol = NULL;
	char buf[128];
	
	switch(expression -> type) {
		case IDENTIFIER:
			symbol = HashTable_get(scope, expression -> value.str);
			if (symbol && symbol -> dmem) {
				if (symbol -> type == SYMBOL_FARRAYPARAM) {
					inst = loadRegisterFromFP(registerNum, -symbol -> dmem -> dMemAddr);
					sprintf(buf, "*REGISTER%d = &%s", registerNum, getExpressionString(expression));
					Instruction_setComment(inst, buf);
					TMCode_addInstruction(tm, inst);
				} else {
					if (symbol -> dmem -> addressType == FP_RELATIVE) {
						inst = loadRegisterWithFP(registerNum, -symbol -> dmem -> dMemAddr);
						sprintf(buf, "REGISTER%d = &%s", registerNum, getExpressionString(expression));
					Instruction_setComment(inst, buf);
						TMCode_addInstruction(tm, inst);
					} else if (symbol -> dmem -> addressType == ABSOLUTE) {
						inst = loadRegisterWithCount(registerNum, symbol -> dmem -> dMemAddr);
						sprintf(buf, "REGISTER%d = &%s", registerNum, getExpressionString(expression));
						Instruction_setComment(inst, buf);
						TMCode_addInstruction(tm, inst);
					}
				}
			}
			break;
		case VAR_ARRAY_ELEMENT:
			genGetAddress(tm, expression -> children[0], registerNum);
			genExpression(tm, expression -> children[1], registerNum + 1);

			inst = subtractRegisters(registerNum, registerNum, registerNum + 1);
			sprintf(buf, "REGISTER%d -= REGISTER%d", registerNum, registerNum+1);
			Instruction_setComment(inst, buf);
			TMCode_addInstruction(tm, inst);
			break;
		default:;
	}
}

/*
	Function: genDivideByZeroCheck
		Generate code to check that a division expression is not dividing by 
		zero.
*/
void genDivByZeroCheck(TMCode * tm, int registerNum) {
	int handlerAddress = TMCode_getFunctionAddress(tm, "HANDLE_EXCEPTION_DIV_BY_ZERO");
	int jumpOffset = handlerAddress - tm -> pc - 1;

	Instruction * inst = jumpIfEqualsZero(registerNum, jumpOffset);
	Instruction_setComment(inst, "Divide by zero check.");
	TMCode_addInstruction(tm, inst);
}

/*
	Function: getPendingAddresses
		Replace any placeholder values inside and instruction with their 
		appropriate address/offset.
*/
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
/**/

/*** util functions ***/
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

void ExitIfOutOfRegisterError(int registerNum){
	if (registerNum + 1 > 4) {
		printf("[ERROR] OUT OF REGISTERS\n");
		exit(0);
	}
}
/**/