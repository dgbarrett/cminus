#ifndef __SYMBOL_H__
#define __SYMBOL_H__

typedef enum _SymbolType {
	SYMBOL_FUNCTION,
	SYMBOL_VAR,
	SYMBOL_FPARAM,
	SYMBOL_FARRAYPARAM,
	SYMBOL_ARRAY
} SymbolType;

typedef enum _SymbolDataType {
	TYPE_INT = 1,
	TYPE_VOID,
	TYPE_INTARR,
	TYPE_VOIDARR
} SymbolDataType;

typedef struct _Symbol {
	char * name;
	SymbolType type;
	SymbolDataType datatype;
	SymbolDataType * signature;
	int isInt;
	int arrlen;
	int linenum;
	int signatureElems;
} Symbol;

Symbol * 	new_Symbol 	(char * name, SymbolType type, int isInt, int arrSize, int lineno);
char * SymbolDataType_toString(SymbolDataType dtype);
SymbolDataType SymbolDataType_fromString(char * dtype);
char * Symbol_callSignatureToString(SymbolDataType * dtypes);
SymbolDataType SymbolDataType_parentType(SymbolDataType dtype);
void Symbol_addToFunctionSignature(Symbol * symbol, SymbolDataType dtype);

#endif