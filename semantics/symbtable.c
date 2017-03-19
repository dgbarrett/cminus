#include "symbtable.h"

#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOLS 10
#define MAX_SUBSCOPES 10

SymbolTable * 	new_SymbolTable			  ();
SymbolTable * 	buildNonEmptySymbolTable  (SymbolTable * st, ASTNode * root);
Scope * 		new_Scope				  (ScopeType type);
Symbol * 		new_Symbol 				  (char * name, SymbolType type);
int 			isInt					  (char * dtype);
void 			Scope_addSymbol           (Scope * scope, SymbolType type, char * name, int isInt );
void 			
SymbolTable_addVariableToCurrentScope(SymbolTable * st, char * name, char * dtype);

SymbolTable * buildSymbolTable(ASTNode * root) {
	SymbolTable * st = new_SymbolTable();

	if (root) {
		if (root->children) {
			return buildNonEmptySymbolTable(st, root);
		} else return st;
	} else return NULL;
}

SymbolTable * buildNonEmptySymbolTable(SymbolTable * st, ASTNode * node) {
	int i = 0;
	ASTNode * temp = NULL;

	
	for (i=0 ; (temp = node -> children[i]) != NULL ; i++) {
		if (temp -> type == FUNCTION_DECLARATION) {
			/* add function name/ret type to current scope */
			/* create a new sub scope for the function */
			/* add function params to new subscope */
			/* add local vars from compound stmt to scope */
			/* search for compound stmts within compound stmt and do same as above */
		} else if (temp -> type == VAR_DECLARATION) {
			/* add variable name/type to current scope */
			char * varName = temp -> children[0] -> value.str;
			char * varType = temp -> children[1] -> value.str;

			SymbolTable_addVariableToCurrentScope(st, varName, varType);
		}
	}
	return NULL;
}

Symbol * new_Symbol(char * name, SymbolType type) {
	Symbol * symbol = malloc(sizeof(symbol));

	symbol -> name = calloc(strlen(name)+1, sizeof(*(symbol->name)));
	strcpy(symbol -> name, name);

	symbol -> type = type;

	return symbol;
}

Scope * new_Scope(ScopeType type) {
	Scope * scope = malloc(sizeof(*scope));

	scope -> type = type;
	scope -> symbols = calloc(MAX_SYMBOLS, sizeof(*(scope->symbols)));
	scope -> subscopes = calloc(MAX_SUBSCOPES, sizeof(*(scope->subscopes)));

	return scope;
}

SymbolTable * new_SymbolTable() {
	SymbolTable * st = malloc(sizeof(*st));

	st -> size = 0;
	st -> root = new_Scope(SCOPE_FILE);
	st -> currScope = st -> root;

	return st;
}

void SymbolTable_addVariableToCurrentScope(SymbolTable * st, char * name, char * dtype) {
	if (st) {
		Scope_addSymbol(st -> currScope, SYMBOL_VAR, name, isInt(dtype));
	}
}

void Scope_addSymbol(Scope * scope, SymbolType type, char * name, int isInt ) {
	if (scope) {
		int i;
		for (i = 0 ; scope -> symbols[i] != NULL ; i++) {}

		if (i < MAX_SYMBOLS) {
			scope -> symbols[i] = new_Symbol(name, type);
		}
	}
}

int isInt(char * dtype) {
	return strcmp(dtype, "int") == 0;
}
