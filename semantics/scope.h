#ifndef __SCOPE_H__
#define __SCOPE_H__

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

Scope * 	new_Scope 		(ScopeType type);
void 		Scope_addSymbol	(Scope * scope, SymbolType type, char * name, int isInt, int arrSize, int lineno);

#endif