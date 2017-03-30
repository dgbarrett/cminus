#include "gen.h"

#define MAX_INSTRUCTIONS 1024

#include <stdio.h>
#include <stdlib.h>

#include "Instruction.h"
#include "tmconstants.h"

/* TMCode */
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
void genRuntimeExceptionHandlers(TMCode * tm);
void genStdlibFunctions(TMCode * tm);

void generateCode(ASTNode * root, char * fname) {
	/* char * tmFilename = getTmFilename(fname); */
	TMCode * tmcode = new_TMCode();

	genRuntimeExceptionHandlers(tmcode);
	genStdlibFunctions(tmcode);

	TMCode_print(tmcode);
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