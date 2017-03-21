#include "symbol.h"

#include <stdlib.h>
#include <string.h>

/*************** Symbol ***************/

/*
	Function: new_Symbol
		Creates a new Symbol object.
*/
Symbol * new_Symbol(char * name, SymbolType type, int isInt, int arrSize, int lineno) {
	Symbol * symbol = malloc(sizeof(*symbol));

	symbol -> name = calloc(strlen(name)+1, sizeof(*(symbol->name)));
	strcpy(symbol -> name, name);

	symbol -> type = type;
	symbol -> isInt = isInt;
	symbol -> arrlen = arrSize;
	symbol -> linenum = lineno;

	if (symbol -> isInt == 1) {
		symbol -> datatype = TYPE_INT;
	} else {
		symbol -> datatype = TYPE_VOID;
	}

	return symbol;
}

char * SymbolDataType_toString(SymbolDataType dtype) {
	return (dtype == TYPE_INT) ? "int" : "void";
}