#ifndef __PARSE_ERROR_H__
#define __PARSE_ERROR_H__

#include <stdio.h>

void printError_incompleteCompoundStatement(int line);
void printError_missingStmtTerminator(int line);
void printError_arraySizeTypeInvalid(int line) ;
void printError(const char * err, int line);

#endif