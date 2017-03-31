#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#define INTERNAL 1
#define CALLABLE 0
#define STACK_TOP 0

typedef struct _TMFunction {
	char * name;
	int isInternal;
} TMFunction;

typedef struct _Instruction {
	char * opcode;
	char comment[256];
	int r,s,t;
	TMFunction * function;
} Instruction;

typedef struct _InstructionSequence {
	int itr;
	Instruction ** sequence;
} InstructionSequence;

TMFunction *			new_TMFunction(char * name, int isInternal);

InstructionSequence * 	new_InstructionSequence();
Instruction * 			InstructionSequence_next(InstructionSequence * seq);
void 					InstructionSequence_addInstruction(InstructionSequence * seq, Instruction * inst);
void 					InstructionSequence_print(InstructionSequence * seq);

Instruction * 			new_Instruction(char * opcode, int r, int s, int t);
void 					Instruction_setComment(Instruction * inst, char * comment);
void 					Instruction_print(Instruction * inst);

/* Verbose Instruction creation functions */
Instruction * 	loadRegisterWithCount(int regNum, int count);
Instruction * 	outputInteger(int fromRegNum);
Instruction * 	halt();
Instruction * 	storeRegisterOnStack(int regNum);
Instruction * 	incrementRegister(int regNum);
Instruction * 	readInteger(int intoRegNum);
Instruction * 	storeRegisterAtStackOffset(int regNum, int offset);
Instruction * 	restoreRegisterFromStack(int regNum);
Instruction *	decrementRegister(int regNum);
Instruction * 	loadPC(int fromRegNum, int offset);
Instruction * 	loadParamIntoRegister(int intoReg, int numSavedRegisters, int returnsInt, int numParams);
Instruction * 	saveFramePointer();
Instruction * 	tmallocate(int size);

/* util */
char * 	new_Name(char * name);

#endif