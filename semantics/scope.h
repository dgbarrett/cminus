#ifndef __SCOPE_H__
#define __SCOPE_H__

#include "symbol.h"
#include "hashtable.h"

#define MAX_SYMBOLS 25
#define MAX_SUBSCOPES 25

/*
	Function: ScopeType
		Defines the type of statement the was the root trigger for the Scope.
*/
typedef enum _ScopeType {
	SCOPE_SELECTION=1,
	SCOPE_LOOP,
	SCOPE_FUNCTION,
	SCOPE_FILE,
	SCOPE_STDLIB
} ScopeType;

/*
	Function: Scope
		A scope within a program, holds an array of symbols defined in the 
		scope, as well as a hash table of all symbols acessible from the scope.
*/
typedef struct _Scope {
	int subscopeCount;
	int symbolCount;
	ScopeType type;
	/* Symbols defined in this Scope */
	Symbol ** symbols;
	/* All symbols accessible from this Scope */
	SymbolHashTable * allsymbols;
	struct _Scope ** subscopes;
	struct _Scope * parent;
} Scope;

Scope * 	new_Scope 		(ScopeType type);
void 		Scope_addSymbol	(Scope * scope, SymbolType type, char * name, int isInt, int arrSize, int lineno);

#endif