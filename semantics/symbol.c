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
		if (type == SYMBOL_ARRAY || type == SYMBOL_FARRAYPARAM){
			symbol -> datatype = TYPE_INTARR;
		} else {
			symbol -> datatype = TYPE_INT;
		}
	} else {
		if (type == SYMBOL_ARRAY || type == SYMBOL_FARRAYPARAM){
			symbol -> datatype = TYPE_VOIDARR;
		} else {
			symbol -> datatype = TYPE_VOID;
		}
	}

	return symbol;
}

char * SymbolDataType_toString(SymbolDataType dtype) {
	switch(dtype) {
		case TYPE_INT:
			return "int";
		case TYPE_INTARR:
			return "int[]";
		case TYPE_VOID:
			return "void";
		case TYPE_VOIDARR:
			return "void[]";
		default:
			return "??NoDtype??";
	}
}

SymbolDataType SymbolDataType_parentType(SymbolDataType dtype) {
	switch(dtype) {
		case TYPE_INT:
			return TYPE_INT;
		case TYPE_INTARR:
			return TYPE_INT;
		case TYPE_VOID:
			return TYPE_VOID;
		case TYPE_VOIDARR:
			return TYPE_VOID;
		default:
			return TYPE_INT;
	}
}