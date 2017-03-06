#include "error.h"

#define ERRSTREAM stderr

char buf[128];

void printError(const char * err, int line) {
	fprintf(ERRSTREAM, "\t[line %d] %s\n", line, err);
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