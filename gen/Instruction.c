#include "Instruction.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tmconstants.h"

#define MAX_SEQ_INST 100

/*** TMCode ***/

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

/*** TMFinale ***/
TMFinale * new_TMFinale(char * name) {
	TMFinale * f = malloc(sizeof(*f));

	f -> name = new_Name(name);

	return f;
}

/*** TMFunction ***/
TMFunction * new_TMFunction(char * name, int isInternal) {
	TMFunction * f = malloc(sizeof(*f));

	f -> name = new_Name(name);
	f -> isInternal = isInternal;

	return f;
}

/*** InstructionSequence ***/
InstructionSequence * new_InstructionSequence() {
	InstructionSequence * seq = malloc(sizeof(*seq));

	seq -> itr = 0;
	seq -> sequence = calloc(MAX_SEQ_INST, sizeof(*(seq -> sequence)));

	return seq;
}

Instruction * InstructionSequence_next(InstructionSequence * seq) {
	if (seq && seq->itr < MAX_SEQ_INST) {
		return seq -> sequence[seq->itr++];
	} else return NULL;
}

void InstructionSequence_addInstruction(InstructionSequence * seq, Instruction * inst) {
	if (seq && inst) {
		int i = 0;
		do {
			if (!seq -> sequence[i]) {
				seq -> sequence[i] = inst;
				return;
			}
		} while(++i < MAX_SEQ_INST);
	}
}

void InstructionSequence_print(InstructionSequence * seq) {
	if (seq && seq -> sequence) {
		int i = 0;
		for (i = 0 ; i < MAX_SEQ_INST ; i++) {
			if (!seq -> sequence[i]) break;
			else Instruction_print(seq -> sequence[i]);
		}
	}
}
/**/

/*** Instruction ***/
Instruction * new_Instruction(char * opcode, int r, int s, int t) {
	Instruction * inst = malloc(sizeof(*inst));

	inst -> opcode = calloc(strlen(opcode) + 1, sizeof(*(inst->opcode)));
	strcpy(inst -> opcode, opcode);

	inst -> r = r;
	inst -> s = s;
	inst -> t = t;
	inst -> loc = 0;
	inst -> function = NULL;
	inst -> finale = NULL;

	return inst;
}

void Instruction_setComment(Instruction * inst, char * comment) {
	if (inst && comment) {
		strcpy(inst -> comment, comment);
	}
}

void Instruction_print(Instruction * inst) {
	if (inst) {
		printf("%4s %7d,%7d,%7d", inst -> opcode, inst->r, inst->s, inst->t);

		if (strlen(inst -> comment) != 0) printf("\t\t\t;%s\n", inst -> comment);
		else printf("\n");
	} 
}
/**/

/*** Verbose Instruction creation functions ***/
Instruction * loadRegisterWithCount(int regNum, int count) {
	return new_Instruction("LDC", regNum, count, 0);
}

Instruction * outputInteger(int fromRegNum) {
	return new_Instruction("OUT", fromRegNum, 0, 0);
}

Instruction * halt() {
	return new_Instruction("HALT", 0,0,0);
}

Instruction * pushRegisterToStack(int regNum) {
	return new_Instruction("ST", regNum, 0, SP);
}

Instruction * storeRegisterAtStackOffset(int regNum, int offset) {
	return new_Instruction("ST", regNum, offset, SP);
}

Instruction * incrementRegister(int regNum) {
	return new_Instruction("LDA", regNum, 1, regNum);
}

Instruction * readInteger(int intoRegNum) {
	return new_Instruction("IN", intoRegNum, 0,0);
}

Instruction * restoreRegisterFromStack(int regNum) {
	return new_Instruction("LD", regNum, -1, SP);
}

Instruction * decrementRegister(int regNum) {
	return new_Instruction("LDA", regNum, -1, regNum);
}

Instruction * decrementRegisterBy(int regNum, int dec) {
	return new_Instruction("LDA", regNum, -1*dec, regNum);
}

Instruction * loadPC(int fromRegNum, int offset) {
	return new_Instruction("LD", PC, offset, fromRegNum);
}

Instruction * loadParamIntoRegister(int intoReg, int numSavedRegisters, int returnsInt, int numParams) {
	return new_Instruction("LD", intoReg, -1*(numSavedRegisters + returnsInt + numParams + 1),FP);
}

/* Space for return value is before return address (FP-1) */
Instruction * storeReturnValueOnStack(int fromReg, int regSaved) {
	return new_Instruction("ST", fromReg, -2-regSaved, FP);
}

Instruction * saveFramePointer() {
	return new_Instruction("LDA", FP, 0, SP);
}

Instruction * tmallocate(int size) {
	return new_Instruction("LDA", SP, size, SP);
}

Instruction * jumpToUndeclaredFunction(char * functionName, int pc) {
	Instruction * inst = new_Instruction("LDA", PC, GEN_ERR, PC);
	inst -> s_pending = new_Name(functionName);
	inst -> loc = pc;
	return inst;
}

Instruction * jumpToUndeclaredFunctionFinale(char * functionName, int pc) {
	Instruction * inst = new_Instruction("LDA", PC, GEN_ERR, PC);

	char buf[128];
	sprintf(buf, "%s_finale", functionName);

	inst -> s_pending = new_Name(buf);
	inst -> loc = pc;

	return inst;
}

Instruction * loadRegisterWithPCOffset(int regNum, int offset) {
	return new_Instruction("LDA", regNum, offset, PC);
}

Instruction * storeRegister(int regStored, int offset, int addrReg) {
	return new_Instruction("ST", regStored, offset, addrReg);
}

Instruction * loadReturnAddressIntoPC() {
	return new_Instruction("LDA", PC, -1, SP);
}

Instruction * jumpToPCOffset(int offset) {
	return new_Instruction("LDA", PC, offset, PC);
}

Instruction * loadRegisterFromFP(int regNum, int offset) {
	return new_Instruction("LD", regNum, offset, FP);
}

Instruction * loadRegisterWithFP(int regNum, int offset) {
	return new_Instruction("LDA", regNum, offset, FP);
}

Instruction * loadAddress(int intoReg, int offset, int fromReg) {
	return new_Instruction("LDA", intoReg, offset, fromReg);
}

Instruction * addRegisters(int r, int s, int t) {
	return new_Instruction("ADD", r,s,t);
}

Instruction * subtractRegisters(int r, int s, int t) {
	return new_Instruction("SUB", r,s,t);
}

Instruction * multiplyRegisters(int r, int s, int t) {
	return new_Instruction("MUL", r,s,t);
}

Instruction * divideRegisters(int r, int s, int t) {
	return new_Instruction("DIV", r,s,t);
}

Instruction * jumpIfEqualsZero(int regNum, int jumpPCOffset) {
	return new_Instruction("JEQ", regNum, jumpPCOffset, PC);
}

Instruction * loadRegisterWithSP(int regNum, int offset) {
	return new_Instruction("LDA", regNum, offset, SP);
}

Instruction * loadRegisterFromSP(int regNum, int offset) {
	return new_Instruction("LD", regNum, offset, SP);
}

Instruction * load(int regNum, int offset, int addressReg) {
	return new_Instruction("LD", regNum, offset, addressReg);
}

Instruction * jumpIfNotEqualsZero(int regNum, int jumpPCOffset) {
	return new_Instruction("JNE", regNum, jumpPCOffset, PC);
}
/**/

/*** util ***/
char * new_Name(char * name) {
	char * newName = calloc(strlen(name) + 1, sizeof(*newName));
	strcpy(newName, name);
	return newName;
}
/**/