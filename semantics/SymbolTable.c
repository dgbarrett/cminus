#include "SymbolTable.h"

#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOLS 10
#define MAX_SUBSCOPES 10

SymbolTable * 	new_SymbolTable			  ();
SymbolTable * 	buildNonEmptySymbolTable  (SymbolTable * st, ASTNode * root);
Scope * 		new_Scope				  (ScopeType type);
Symbol * 		new_Symbol 				  (char * name, int isFunction);
void 			Scope_addSymbol 		  (Scope * scope, char * name, int isFunction);
void 			Scope_addVariable		  (Scope * scope, char * name);
void 			Scope_addFunction		  (Scope * scope, char * name);

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

	/*
	for (i=0 ; (temp = node -> children[i]) != NULL ; i++) {
		if (temp -> type == )
	}*/
	return NULL;
}

Symbol * new_Symbol(char * name, int isFunction) {
	Symbol * symbol = malloc(sizeof(symbol));

	symbol -> name = calloc(strlen(name) + 1, sizeof(*(symbol->name)));
	strcpy(symbol -> name, name);

	symbol -> isFunction = isFunction;

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

	return st;
}

void Scope_addVariable(Scope * scope, char * name) {
	Scope_addSymbol(scope, name, 0);
}

void Scope_addFunction(Scope * scope, char * name) {
	Scope_addSymbol(scope, name, 1);
}

void Scope_addSymbol(Scope * scope, char * name, int isFunction) {
	if (scope) {
		int i;
		for (i = 0 ; scope -> symbols[i] != NULL ; i++) {}

		if (i < MAX_SYMBOLS) {
			scope -> symbols[i] = new_Symbol(name, isFunction);
		}
	}
}

