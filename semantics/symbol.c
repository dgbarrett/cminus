#include "symbol.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_FUNCTION_PARAMS 10

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
	symbol -> signature = calloc(MAX_FUNCTION_PARAMS + 1, sizeof(*(symbol->signature)));
	symbol -> signatureElems = 0;

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

char * Symbol_callSignatureToString(SymbolDataType * dtypes) {
	char * buf = calloc(128, sizeof(*buf));
	strcpy(buf,"(");

	if (dtypes) {
		int i = 0;

		for (i = 0 ; dtypes[i] ; i++) {
			if (i != 0) strcat(buf, ", ");
			switch(dtypes[i]) {
				case(TYPE_INT):
					strcat(buf,"int");
					break;
				case(TYPE_INTARR):
					strcat(buf,"int[]");
					break;
				case(TYPE_VOID):
					strcat(buf,"void");
					break;
				case(TYPE_VOIDARR):
					strcat(buf,"void[]");
					break;
				default:
					strcat(buf, "??");
			}
		}
	}

	strcat(buf, ")");
	return buf;
}

void Symbol_addToFunctionSignature(Symbol * symbol, SymbolDataType dtype) {
	if (symbol) {
		if (symbol -> signature){
			if (symbol -> signatureElems < MAX_FUNCTION_PARAMS) {
				int i = 0;
				for (i = 0 ; i < symbol->signatureElems ; i++) {}
				symbol -> signature[i] = dtype;
				symbol -> signatureElems++;
			} else printf("too amny elems\n");
		} else printf("Sig was null\n");
	} else printf("Symbol was null\n");
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

SymbolDataType SymbolDataType_fromString(char * dtype) {
	if (strcmp(dtype, "int") == 0) {
		return TYPE_INT;
	} else if (strcmp(dtype, "int[]")==0) {
		return TYPE_INTARR;
	}else if (strcmp(dtype, "void")==0) {
		return TYPE_VOID;
	}else if (strcmp(dtype, "void[]")==0) {
		return TYPE_VOIDARR;
	} else return TYPE_INT;
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