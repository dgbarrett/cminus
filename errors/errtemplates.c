#include "errtemplates.h"

#include <stdio.h>
#include <stdlib.h>

char * ErrTemplate_MismatchedExprType(char * exprType, char * name1, char * type1, char * name2, char * type2){
	char * buf = calloc(512, sizeof(*buf));
	sprintf( 
		buf,
		"Types of %s expression do not match.\n\tLeft  side is %4s: \"%s\"\n\tRight side is %4s: \"%s\"",
		exprType, type1, name1, type2, name2
	);
	return buf;
}

char * ErrTemplate_UndefinedSymbol(char * name) {
	char * buf = calloc(512, sizeof(*buf));
	sprintf(buf, "Undefined identifier. \"%s\" is not defined.", name);
	return buf;
}

char * ErrTemplate_UndefinedArray(char * name) {
	char * buf = calloc(512, sizeof(*buf));
	sprintf(buf, "Undefined identifier. Array identifier \"%s\" is not defined.", name);
	return buf;
}

char * ErrTemplate_UndefinedFunction(char * name, char * signature) {
	char * buf = calloc(512, sizeof(*buf));
	sprintf(buf, "Undefined identifier. Function \"%s%s\" is not defined.", name, signature);
	return buf;
}

char * ErrTemplate_InvalidArrayAccessType(char * arrname, char * access, char * accessType) {
	char * buf = calloc(512, sizeof(*buf));
	sprintf(buf,"Invalid array access. Arrays indices must be integers.\n\tAttempted to access: %s[%s], typeof(%s) == %s", arrname, access, access, accessType);
	return buf;
}

char * ErrTemplate_InvalidConditionType(char * stmtType, char * condition, char * condType) {
	char * buf = calloc(512, sizeof(*buf));
	sprintf(buf, "Invalid condition type. %s condition must be of type integer.\n\tProvided condition (%s) is of type %s.", stmtType, condition, condType);
	return buf;
}

char * ErrTemplate_MismatchedSignature(char * funcName, char * expectedSig, char * actualSig) {
	char * buf = calloc(512, sizeof(*buf));
	sprintf(buf, "Signature provided for function call does not match the expected signature.\n\t Got \"%s%s\" expected \"%s%s\".", funcName, actualSig, funcName, expectedSig);
	return buf;
}

char * ErrTemplate_RedefinedSymbol(char * name, int line){
	char * buf = calloc(512, sizeof(*buf));
	sprintf(buf,"Symbol \"%s\" redefined.\n\t Previous definition exists at line %d.",name, line);
	return buf;
}

char * ErrTemplate_RedefinedStdlibSymbol(char * name){
	char * buf = calloc(512, sizeof(*buf));
	sprintf(buf,"Symbol \"%s\" redefined.\n\t \"%s\" is defined as part of the C- standard library.", name, name);
	return buf;
}

char * ErrTemplate_MissingReturnStmt(char * functionName) {
	char * buf = calloc(512, sizeof(*buf));
	sprintf(buf, "Function \"%s\" is missing a return statement.\n\t Expected return value of type int.", functionName);
	return buf;
}

char * ErrTemplate_ArrayOutOfBounds(char * name, int index, int arrlen) {
	char * buf = calloc(512, sizeof(*buf));
	sprintf(buf,"Attempting to access array \"%s\" of size=%d at index=%d.\n\tMax index for \"%s\" is %d.", name, arrlen, index, name, arrlen-1);
	return buf;
}

char * ErrTemplate_UndefinedArrayIndexId(char * arrname, char * index) {
	char * buf = calloc(512, sizeof(*buf));
	sprintf(buf, "Undefined identifier. \"%s\" is not defined.\n\t Attempting to access %s[%s].", index, arrname, index);
	return buf;
}

char * ErrTemplate_UndefinedSymbolInFCall(char * fname, char * symname) {
	char * buf = calloc(512, sizeof(*buf));
	sprintf(buf, "Undefined identifier. \"%s\" is not defined.\n\t In call to \"%s\".", symname, fname);
	return buf;
}

char * ErrTemplate_MismatchedReturnType(char * fname, char * expectedRetType, char * retType) {
	char * buf = calloc(512, sizeof(*buf));
	sprintf(buf, "Expected return value of type %s from \"%s\".  Got return of type %s.",expectedRetType, fname, retType);
	return buf;
}

