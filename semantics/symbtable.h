#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#include "../parse/ast.h"

#define MAX_SYMBOLS 10
#define MAX_SUBSCOPES 10

typedef enum _SymbolType {
	SYMBOL_FUNCTION,
	SYMBOL_VAR,
	SYMBOL_FPARAM,
	SYMBOL_FARRAYPARAM,
	SYMBOL_ARRAY
} SymbolType;

typedef struct _Symbol {
	char * name;
	SymbolType type;
	int isInt;
	int arrlen;
	int linenum;
} Symbol;

typedef enum _ScopeType {
	SCOPE_SELECTION,
	SCOPE_LOOP,
	SCOPE_FUNCTION,
	SCOPE_FILE
} ScopeType;

typedef struct _SymbolHashTable {
	int size;
	int maxsize;
	Symbol ** symbols;
} SymbolHashTable;

typedef struct _Scope {
	int subscopeCount;
	int symbolCount;
	ScopeType type;
	Symbol ** symbols;
	SymbolHashTable * allsymbols;
	struct _Scope ** subscopes;
	struct _Scope * parent;
} Scope;

typedef struct _SymbolTable {
	Scope * root;
	Scope * currScope;
	int size;
} SymbolTable;	



SymbolTable * buildSymbolTable(ASTNode * root);

#endif