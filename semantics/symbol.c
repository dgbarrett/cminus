#include "symbol.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
	Function: new_Symbol
		Creates a new Symbol object.
*/
Symbol * new_Symbol(char * name, SymbolType type, int isInt, int arrSize, int lineno) {
	Symbol * symbol = malloc(sizeof(*symbol));

	symbol -> name = calloc(strlen(name)+1, sizeof(*(symbol->name)));
	strcpy(symbol -> name, name);

	symbol -> type = type;
	symbol -> arrlen = arrSize;
	symbol -> linenum = lineno;
	symbol -> signature = calloc(MAX_FUNCTION_PARAMS + 1, sizeof(*(symbol->signature)));
	symbol -> signatureElems = 0;

	if (isInt == 1) {
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

/*
	Function: Symbol_callSignatureToString
		Returns the string form of the call signature specified by dtypes.
*/
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

/*
	Function: Symbol_addToFunctionSignature
		Append the element dtype to the function signature of symbol.
*/
void Symbol_addToFunctionSignature(Symbol * symbol, SymbolDataType dtype) {
	if (symbol) {
		if (symbol -> signature){
			if (symbol -> signatureElems < MAX_FUNCTION_PARAMS) {
				int i = 0;
				for (i = 0 ; i < symbol->signatureElems ; i++) {}
				symbol -> signature[i] = dtype;
				symbol -> signatureElems++;
			} 
		} 
	} 
}

/*
	Function: SymbolDataType_toString
		Returns the string form of a SymbolDataType.
*/
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

/*
	Function: SymbolDataType_fromString
		Converts a string representation into a SymbolDataType.
*/
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


/*
	Function: SymbolDataType_parentType
		Get the parent type for a given SymbolDataType.
		ex.
			parent of int[] is int.
*/
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