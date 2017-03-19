#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#include "../parse/ast.h"

#define MAX_SYMBOLS 10
#define MAX_SUBSCOPES 10

typedef enum _SymbolType {
	SYMBOL_FUNCTION,
	SYMBOL_VAR,
	SYMBOL_FPARAM
} SymbolType;

typedef struct _Symbol {
	char * name;
	SymbolType type;
	/* is 1 if function returns int, or var is int type */
	int isInt;
} Symbol;

typedef enum _ScopeType {
	SCOPE_SELECTION,
	SCOPE_LOOP,
	SCOPE_FUNCTION,
	SCOPE_FILE
} ScopeType;

typedef struct _Scope {
	ScopeType type;
	Symbol ** symbols;
	struct _Scope ** subscopes;
} Scope;

typedef struct _SymbolTable {
	Scope * root;
	Scope * currScope;
	int size;
} SymbolTable;	

SymbolTable * buildSymbolTable(ASTNode * root);

#endif