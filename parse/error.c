#include "error.h"

#define ERRSTREAM stderr

char buf[128];

void printError(const char * err, int line) {
	fprintf(ERRSTREAM, "[ ERROR ][line %3d] %s\n", line, err);
}

void printError_incompleteCompoundStatement(int line) {
	sprintf(buf, "Incomplete compound statement starting at \"{\"");
	printError(buf, line);
}

void printError_missingStmtTerminator(int line) {
	sprintf(buf, "Statement terminator is missing (';').");
	printError(buf, line);
}

void printError_arraySizeTypeInvalid(int line) {
	sprintf(buf, "Array sizes must be numbers.");
	printError(buf, line);
}

void printError_invalidRelationalOperation(int line) {
	sprintf(buf, "Not a valid relational operator.");
	printError(buf, line);
}

void printError_invalidCondition(int line) {
	sprintf(buf, "Condition can not be empty.");
	printError(buf, line);
}

void printError_invalidFunctionParams(int line) {
	sprintf(buf, "Function parameter list is invalid.");
	printError(buf, line);
}

void printError_invalidReturnType(int line) {
	sprintf(buf, "Missing or invalid return type for function.");
	printError(buf, line);
}

void printError_invalidArrayAccess(int line) {
	sprintf(buf, "Invalid array access attempt.");
	printError(buf, line);
}