#include "scope.h"

#include <stdlib.h>

/*
	Function: new_Scope
		Creates a new Scope object.
*/
Scope * new_Scope(ScopeType type) {
	Scope * scope = malloc(sizeof(*scope));

	scope -> type = type;
	scope -> allsymbols = new_SymbolHashTable();
	scope -> symbols = calloc(MAX_SYMBOLS, sizeof(*(scope->symbols)));
	scope -> subscopes = calloc(MAX_SUBSCOPES, sizeof(*(scope->subscopes)));
	scope -> subscopeCount = 0;
	scope -> symbolCount = 0;

	return scope;
}

/*
	Function: Scope_addSymbol
		Adds a Symbol to the Scope.
*/
void Scope_addSymbol(Scope * scope, SymbolType type, char * name, int isInt, int arrSize, int lineno) {
	if (scope) {
		int i;
		for (i = 0 ; scope -> symbols[i] != NULL ; i++) {}

		if (i < MAX_SYMBOLS) {
			scope -> symbols[i] = new_Symbol(name, type, isInt, arrSize, lineno);
		}

		scope -> symbolCount++;
	}
}


