#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#define MAX_FUNCTION_PARAMS 10

#include "../gen/DMemSymbol.h"

/* 
	Type: SymbolType
		Denotes the type of a Symbol.
*/
typedef enum _SymbolType {
	SYMBOL_FUNCTION,
	SYMBOL_VAR,
	SYMBOL_FPARAM,
	SYMBOL_FARRAYPARAM,
	SYMBOL_ARRAY
} SymbolType;

/*
	Type: SymbolDataType
		Denotes the data type of a Symbol.
*/
typedef enum _SymbolDataType {
	TYPE_INT = 1,
	TYPE_VOID,
	TYPE_INTARR,
	TYPE_VOIDARR
} SymbolDataType;

/*
	Type: Symbol
		Holds all information associated with a symbol definition.
*/
typedef struct _Symbol {
	char * name;
	SymbolType type;
	SymbolDataType datatype;
	/* Array of types makes up the signature if Symbol is a function */
	SymbolDataType * signature;
	int arrlen;
	int linenum;
	int signatureElems;
	DMemSymbol * dmem;
} Symbol;

char * 			SymbolDataType_toString		(SymbolDataType dtype);
SymbolDataType 	SymbolDataType_fromString	(char * dtype);
SymbolDataType 	SymbolDataType_parentType	(SymbolDataType dtype);

char * 		Symbol_callSignatureToString	(SymbolDataType * dtypes);
Symbol * 	new_Symbol 						(char * name, SymbolType type, int isInt, int arrSize, int lineno);
void 		Symbol_addToFunctionSignature 	(Symbol * symbol, SymbolDataType dtype);
void 		Symbol_associateDMemSymbol 		(Symbol * symbol, DMemSymbol * dmem);

#endif