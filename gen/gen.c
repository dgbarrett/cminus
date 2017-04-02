#include "gen.h"

#include <stdio.h>
#include <stdlib.h>

#include "Instruction.h"
#include "tmconstants.h"
#include "DMemSymbol.h"
#include "Parameter.h"
#include "../semantics/symbtable.h"

/*** gen functions ***/
void genInitDMem(TMCode * tm);
void genLoadGlobals(TMCode * tm, ASTNode * root);
void genCallMain(TMCode * tm, ASTNode * root);
void genProgramEnd(TMCode * tm, char * filename);
void genRuntimeExceptionHandlers(TMCode * tm); 
void genStdlibFunctions(TMCode * tm);
void genFunctionDefinition(TMCode * tm, ASTNode * function, int saveRegisters) ;
void finishInstructions(TMCode * tm);
void genFunctionCall(TMCode * tm, Symbol * func, FunctionParameter ** map, SymbolHashTable * funcSymbols); 
void genSaveRegisters(TMCode * tm, char * name);
void genRestoreRegisters(TMCode * tm, char * functionName);
void genCompoundStatement(TMCode * tm, ASTNode * compoundStmt, int registersSaved);
void genLocalVars(TMCode * tm, ASTNode * locals);
void genReturnStatement(TMCode * tm, ASTNode * returnStmt, int registersSaved);
void genExpression(TMCode * tm, ASTNode * expression, int registerNum); 
void genGetAddress(TMCode * tm, ASTNode * expression, int registerNum );
void genDivByZeroCheck(TMCode * tm, int registerNum);
void getPendingAddresses(TMCode * tm, Instruction * inst);
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
		if (ASTNode_isMainFunction(root -> children[i])) {
			genFunctionDefinition(tmcode, root -> children[i], REGISTER_PURE);
		}
	}

	/* Gen other function bodies */
	for (i=0;root->children[i];i++) {
		if (!ASTNode_isMainFunction(root -> children[i])) {
			genFunctionDefinition(tmcode, root -> children[i], REGISTER_PURE);
		}
	}

	finishInstructions(tmcode);

	TMCode_print(tmcode);
}
/*****/

/*
	Function: genInitDMem
		Generates code used to zero out the top of DMem before program 
		execution.
*/
void genInitDMem(TMCode * tm) {
	Instruction * inst = storeRegister(0, 0, 0);
	Instruction_setComment(inst, "Zero out top of memory.");
	TMCode_addInstruction(tm, inst);
}

/*
	Function: genLoadGlobals
		Generates the codes that load the allocates space for the global 
		variables on the stack.  Associates global names with Symbols so
		references to globals can be used easily.
*/
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

/*
	Function: genCallMain
		Generates the inital call to main that starts execution of the user 
		program.
*/
void genCallMain(TMCode * tm, ASTNode * root) {
	ASTNode * mainNode = AST_getMainNode(root);
	SymbolHashTable * functionScope = ASTNode_getEnclosingScope(mainNode -> children[3]);
	Symbol * mainFunction = HashTable_get(functionScope, "main");
	
	if (mainFunction -> type != SYMBOL_FUNCTION) {
		fprintf(stderr, "No main exists\n");
		exit(0);
	}

	ASTNode * paramList = mainNode -> children[2];
	char ** paramNames = ParameterList_getParamNames(paramList);
	FunctionParameter ** map = createParameterMap(mainFunction -> signatureElems);
	
	int i = 0;
	for (i = 0 ; i < mainFunction -> signatureElems ; i++) {
		ParameterMap_addParam(map, i, paramNames[i], REGISTER, 0);
	}

	genFunctionCall(tm, mainFunction, map, functionScope);

	if (mainFunction -> datatype == TYPE_INT) {
		Instruction * inst = loadRegisterFromSP(REGISTER0, -2);
		Instruction_setComment(inst, "Loading return value from inital call to \"main\".");
		TMCode_addInstruction(tm, inst);
		inst = outputInteger(REGISTER0);
		Instruction_setComment(inst, "Outputting return value from inital call to \"main\".");
		TMCode_addInstruction(tm, inst);
	}
}

/*
	Function: genProgramEnd
		Generates the exit point from the program, and all cleanup required 
		before program exit.
*/
void genProgramEnd(TMCode * tm, char * filename) {
	Instruction * inst = halt();

	char buf[128];
	sprintf(buf, "Exit point for main function of \"%s\".", filename);

	Instruction_setComment(inst, buf);
	TMCode_addInstruction(tm, inst);
}

/*
	Function: genRuntimeExceptionHandlers
		Generates code snippets used to print runtime error "messages" and then
		exit from the program.
*/
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

/*
	Function: genStdlibFunctions
		Generates the code for the input and output functions.
*/
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

/*
	Function: genFunctionDefinition
		Generates the asm code that defines a function.
*/
void genFunctionDefinition(TMCode * tm, ASTNode * function, int saveRegisters) {
	Instruction * inst = NULL;
	char buf[128];
	char * functionName = function -> children[1] -> value.str;

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
		sprintf(buf, "[Function] Start of callable function \"%s\". Saving FP.", functionName);
	}
	Instruction_setComment(inst, buf);
	TMCode_addInstruction(tm, inst);

	genCompoundStatement(tm, function -> children[3], (saveRegisters) ? 6 : 0);

	/* Free local variables declared by compound stmt body */
	int finaleLocationSaved = 0;
	if (CompoundStatement_hasLocals(function -> children[3])) {
		int totalAlloc = CompoundStatement_getLocalAllocSize(function -> children[3]);

		inst = decrementRegisterBy(SP, totalAlloc);
		inst -> finale = new_TMFinale(functionName);
		sprintf(buf, "Start of finale for \"%s\". Deallocating local vars", functionName);
		Instruction_setComment(inst, buf);

		TMCode_addInstruction(tm, inst);

		finaleLocationSaved = 1;
		tm -> sp -= totalAlloc;

		genRestoreRegisters(tm, NULL);
	} else {
		if (saveRegisters) {
			genRestoreRegisters(tm, functionName);
			finaleLocationSaved = 1;
		}
	}

	inst = loadPC(SP, -1);

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
void genFunctionCall(TMCode * tm, Symbol * func, FunctionParameter ** map, SymbolHashTable * funcSymbols) {
	InstructionSequence * seq = new_InstructionSequence();
	int seqItr = 0;
	int i = 0;

	int registersSaved = 6;
	int returnsInt = (func -> datatype == TYPE_INT) ? 1 : 0;
	int numParams = func -> signatureElems;

	/* Push parameters to the stack */
	for ( i = 0 ; map[i] ; i++) {
		if (map[i] -> isRegister) {
			Symbol * paramSymbol = HashTable_get(funcSymbols, map[i] -> name);
			int dMemAddr = -1*(registersSaved + 1 + returnsInt + numParams);

			if (paramSymbol -> datatype == TYPE_INT) {
				paramSymbol -> dmem = new_DMemSymbol(map[i] -> name, "int", 0, dMemAddr, FP_RELATIVE);
			} else if ( paramSymbol -> datatype == TYPE_VOID ) {
				paramSymbol -> dmem = new_DMemSymbol(map[i] -> name, "void", 0, dMemAddr, FP_RELATIVE);
			} 

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
	seq -> sequence[10] = pushRegisterToStack(FP);
	seq -> sequence[11] = incrementRegister(SP);
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

	seq -> sequence[1] = decrementRegister(SP);
	seq -> sequence[2] = restoreRegisterFromStack(REGISTER4);
	seq -> sequence[3] = decrementRegister(SP);
	seq -> sequence[4] = restoreRegisterFromStack(REGISTER3);
	seq -> sequence[5] = decrementRegister(SP);
	seq -> sequence[6] = restoreRegisterFromStack(REGISTER2);
	seq -> sequence[7] = decrementRegister(SP);
	seq -> sequence[8] = restoreRegisterFromStack(REGISTER1);
	seq -> sequence[9] = decrementRegister(SP);
	seq -> sequence[10] = restoreRegisterFromStack(REGISTER0);
	seq -> sequence[11] = decrementRegister(SP);

	tm -> sp -= 5;

	Instruction_setComment(seq -> sequence[11], "Done restoring registers from stack.");

	TMCode_addInstructionSequence(tm,seq);
}

/*
	Function: genCompoundStatement
		Generate the code for a compound statement.
*/
void genCompoundStatement(TMCode * tm, ASTNode * compoundStmt, int registersSaved) {
	if (!compoundStmt -> children[0]) return;
	
	if (compoundStmt -> children[0] -> type == LOCAL_VARS) {
		genLocalVars(tm, compoundStmt -> children[0]);
	}

	int i = 0;
	for ( i = 0 ; compoundStmt -> children[i] ; i++ ) {
		if (compoundStmt -> children[i] -> type == RETURN_STATEMENT) {
			genReturnStatement(tm, compoundStmt->children[i], registersSaved);
		} else if (compoundStmt -> children[i] -> type == EXPRESSION) {
			genExpression(tm, compoundStmt -> children[i], REGISTER0);
		}
	}
}

/*
	Function: genLocalVars
		Generate the code for to allocate space local symbols and save their 
		relative addresses for later use.
*/
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

/*
	Function: genExpression
		Generate the code for an expression, and save the result of the 
		expression in registerNum.
*/
void genExpression(TMCode * tm, ASTNode * expression, int registerNum) {
	if (tm && expression) {
		Instruction * inst = NULL;
		SymbolHashTable * scope = ASTNode_getEnclosingScope(expression);
		Symbol * symbol = NULL;

		switch ( expression -> type ) {
			case NUMBER:
				inst = loadRegisterWithCount(registerNum, expression -> value.num);
				Instruction_setComment(inst,"Saving expression number into register.");
				TMCode_addInstruction(tm, inst);
				break;
			case IDENTIFIER:
				symbol = HashTable_get(scope, expression -> value.str);
				if (symbol) {
					if (symbol -> dmem) {
						if (symbol -> dmem -> addressType == FP_RELATIVE) {
							inst = loadRegisterFromFP(registerNum, symbol -> dmem -> dMemAddr);
							Instruction_setComment(inst, "Loading symbol value into register.");
						}
					} 
				}
				TMCode_addInstruction(tm, inst);
				break;
			case EXPRESSION:
				switch( expression -> value.operation ) {
					case ASSIGN:
						ExitIfOutOfRegisterError(registerNum);
						genGetAddress(tm, expression -> children[0], registerNum);
						genExpression(tm, expression -> children[1], registerNum + 1);

						inst = storeRegister(registerNum + 1, 0, registerNum);
						Instruction_setComment(inst, "Assigning value to address.");
						TMCode_addInstruction(tm, inst);

						inst = loadAddress(registerNum, 0, registerNum + 1);
						Instruction_setComment(inst, "Saving result of expression into register.");
						TMCode_addInstruction(tm, inst);
						break;
					case ADD:
					case SUB:
					case MUL:
					case DIV:
						ExitIfOutOfRegisterError(registerNum);
						genExpression(tm, expression -> children[0], registerNum);
						genExpression(tm, expression -> children[1], registerNum + 1);

						if (expression -> value.operation == ADD) {
							inst = addRegisters(registerNum, registerNum, registerNum + 1);
							Instruction_setComment(inst, "Add expression.");
						} else if (expression -> value.operation == SUB) {
							inst = subtractRegisters(registerNum, registerNum, registerNum + 1);
							Instruction_setComment(inst, "Subtraction expression.");
						} else if (expression -> value.operation == MUL) {
							inst = multiplyRegisters(registerNum, registerNum, registerNum + 1);
							Instruction_setComment(inst, "Multiplication expression.");
						} else if (expression -> value.operation == DIV) {
							genDivByZeroCheck(tm, registerNum + 1);
							inst = divideRegisters(registerNum, registerNum, registerNum + 1);
							Instruction_setComment(inst, "Division expression.");
						} 

						TMCode_addInstruction(tm, inst);
						break;
					case EQ:
						ExitIfOutOfRegisterError(registerNum);
						genExpression(tm, expression -> children[0], registerNum);
						genExpression(tm, expression -> children[1], registerNum + 1);

						inst = subtractRegisters(registerNum, registerNum, registerNum + 1);
						Instruction_setComment(inst, "Getting difference between values.");
						TMCode_addInstruction(tm, inst);

						inst = jumpIfNotEqualsZero(registerNum, 2);
						TMCode_addInstruction(tm, inst);

						inst = loadRegisterWithCount(registerNum, EXPRESSION_TRUE);
						Instruction_setComment(inst, "Value loaded if expression true.");
						TMCode_addInstruction(tm, inst);

						inst = incrementRegister(PC);
						TMCode_addInstruction(tm, inst);

						inst = loadRegisterWithCount(registerNum, EXPRESSION_FALSE);
						Instruction_setComment(inst, "Value loaded if expression false.");
						TMCode_addInstruction(tm, inst); 
					default:;
				}
				break;
			default:;
		}

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
	
	switch(expression -> type) {
		case IDENTIFIER:
			symbol = HashTable_get(scope, expression -> value.str);
			if (symbol && symbol -> dmem) {
				if (symbol -> dmem -> addressType == FP_RELATIVE) {
					inst = loadRegisterWithFP(registerNum, symbol -> dmem -> dMemAddr);
					Instruction_setComment(inst, "Loading symbol address into register.");
					TMCode_addInstruction(tm, inst);
				}
			}
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