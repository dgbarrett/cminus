#include "errtemplates.h"

#include <stdio.h>
#include <stdlib.h>

char * ErrTemplate_MismatchedExprType(char * exprType, char * name1, char * type1, char * name2, char * type2){
	char * buf = calloc(512, sizeof(*buf));
	sprintf( 
		buf,
		"Types of %s expression do not match.\n\t Left side (%s) is %s, right side (%s) is %s.",
		exprType, name1, type1, name2, type2
	);
	return buf;
}

char * ErrTemplate_UndefinedSymbol(char * name) {
	char * buf = calloc(512, sizeof(*buf));
	sprintf(buf, "Undefined identifier. \"%s\" is not defined.", name);
	return buf;
}

char * ErrTemplate_InvalidArrayAccessType(char * arrname, char * access, char * accessType) {
	char * buf = calloc(512, sizeof(*buf));
	sprintf(buf,"Invalid array access. Arrays indices must be integers.\n\tAttempted to access: %s[%s], typeof(%s) == %s", arrname, access, access, accessType);
	return buf;
}

char * ErrTemplate_InvalidConditionType(char * stmtType, char * condition, char * condType) {
	char * buf = calloc(512, sizeof(*buf));
	sprintf(buf, "Invalid condition type. %s condition must be of type integer.\n\tProvided condition%s is of type %s.", stmtType, condition, condType);
	return buf;
}