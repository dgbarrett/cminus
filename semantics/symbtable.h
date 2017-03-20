#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#include "../parse/ast.h"
#include "symbol.h"
#include "hashtable.h"

#define MAX_SYMBOLS 10
#define MAX_SUBSCOPES 10

typedef enum _ScopeType {
	SCOPE_SELECTION,
	SCOPE_LOOP,
	SCOPE_FUNCTION,
	SCOPE_FILE
} ScopeType;

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