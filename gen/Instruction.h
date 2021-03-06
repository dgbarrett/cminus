#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#define INTERNAL 1
#define CALLABLE 0
#define STACK_TOP 0

#define MAX_INSTRUCTIONS 1024
#define DMEM_MAX 1024

typedef struct _TMFinale{
	char * name;
} TMFinale;

typedef struct _TMFunction {
	char * name;
	int isInternal;
} TMFunction;

/*
	Type: Instruction
		Represents a single TM ASM instruction.
*/
typedef struct _Instruction {
	char * opcode;
	char comment[256];
	int r,s,t;
	int loc;
	char *r_pending,*s_pending,*t_pending;
	TMFunction * function;
	TMFinale * finale;
} Instruction;

typedef struct _InstructionSequence {
	int itr;
	Instruction ** sequence;
} InstructionSequence;

typedef struct _TMCode {
	int pc, sp;
	Instruction ** instructions;
} TMCode;

/* TMCode */
TMCode * 	new_TMCode();
int 		TMCode_getFunctionAddress(TMCode * tm, char * name);
int 		TMCode_getFunctionFinaleAddress(TMCode * tm, char * name);
void 		TMCode_addInstruction(TMCode * tm, Instruction * inst);
void 		TMCode_addInstructionSequence(TMCode * tm, InstructionSequence * seq);
void 		TMCode_print(TMCode * tm);

/* TMFinale */
TMFinale * 	new_TMFinale(char * name);

/* TMFunction */
TMFunction *	new_TMFunction(char * name, int isInternal);

/* InstructionSequence */
InstructionSequence * 	new_InstructionSequence();
Instruction * 			InstructionSequence_next(InstructionSequence * seq);
void 					InstructionSequence_addInstruction(InstructionSequence * seq, Instruction * inst);
void 					InstructionSequence_print(InstructionSequence * seq);
void 					TMCode_printToFile(TMCode * tm, char * filename);

/* Instruction */
Instruction * 			new_Instruction(char * opcode, int r, int s, int t);
char * 					Instruction_toString(Instruction * inst);
void 					Instruction_setComment(Instruction * inst, char * comment);
void 					Instruction_print(Instruction * inst);

/* Verbose Instruction creation functions */
Instruction * 	loadRegisterWithCount(int regNum, int count);
Instruction * 	outputInteger(int fromRegNum);
Instruction * 	halt();
Instruction * 	pushRegisterToStack(int regNum);
Instruction * 	incrementRegister(int regNum);
Instruction * 	readInteger(int intoRegNum);
Instruction * 	storeRegisterAtStackOffset(int regNum, int offset);
Instruction * 	restoreRegisterFromStack(int regNum);
Instruction *	decrementRegister(int regNum);
Instruction * 	decrementRegisterBy(int regNum, int dec);
Instruction * 	loadPC(int fromRegNum, int offset);
Instruction * 	saveFramePointer();
Instruction * 	tmallocate(int size);
Instruction * 	loadRegisterWithPCOffset(int regNum, int offset);
Instruction * 	storeRegister(int regStored, int offset, int addrReg);
Instruction * 	jumpToUndeclaredFunction(char * functionName, int pc);
Instruction * 	jumpToUndeclaredFunctionFinale(char * functionName, int pc);
Instruction * 	jumpToPCOffset(int offset);
Instruction * 	storeReturnValueOnStack(int fromReg, int regSaved);
Instruction * 	loadRegisterFromFP(int regNum, int offset);
Instruction * 	loadRegisterWithFP(int regNum, int offset);
Instruction * 	loadAddress(int intoReg, int offset, int fromReg);
Instruction * 	addRegisters(int r, int s, int t);
Instruction * 	subtractRegisters(int r, int s, int t);
Instruction *   multiplyRegisters(int r, int s, int t);
Instruction *   divideRegisters(int r, int s, int t);
Instruction * 	jumpIfEqualsZero(int regNum, int jumpPCOffset);
Instruction * 	loadRegisterWithSP(int regNum, int offset);
Instruction * 	loadDMemAddress(int regNum, int address);
Instruction * 	loadRegisterFromSP(int regNum, int offset);
Instruction * 	jumpIfNotEqualsZero(int regNum, int jumpPCOffset);
Instruction * 	jumpIfLessThanEqualZero(int regNum, int jumpPCOffset);
Instruction * 	jumpIfLessThanZero(int regNum, int jumpPCOffset);
Instruction * 	jumpIfGreaterThanEqualZero(int regNum, int jumpPCOffset);
Instruction * 	jumpIfGreaterThanZero(int regNum, int jumpPCOffset);
Instruction * 	incrementRegisterBy(int regNum, int inc);
Instruction * 	load(int regNum, int offset, int addressReg);

/* util */
char * 	new_Name(char * name);

#endif