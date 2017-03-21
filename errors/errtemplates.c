#include "errtemplates.h"

#include <stdio.h>
#include <stdlib.h>

char * ErrTemplate_MismatchedExprType(char * exprType, char * name1, char * type1, char * name2, char * type2){
	char * buf = calloc(512, sizeof(*buf));
	sprintf( 
		buf,
		"Types of %s expression do not match.\n\t Left side (%s) is %s, right side (%s) is %s.\n",
		exprType, name1, type1, name2, type2
	);
	return buf;
}

char * ErrTemplate_UndefinedSymbol(char * name) {
	char * buf = calloc(512, sizeof(*buf));
	sprintf(buf, "Undefined identifier. \"%s\" is not defined.\n", name);
	return buf;
}