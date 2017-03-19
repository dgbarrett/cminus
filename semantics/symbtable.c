#include "symbtable.h"

#include <stdlib.h>
#include <string.h>

SymbolTable * 	new_SymbolTable			  ();
SymbolTable * 	buildNonEmptySymbolTable  (SymbolTable * st, ASTNode * root);
Scope * 		new_Scope				  (ScopeType type);
Symbol * 		new_Symbol 				  (char * name, SymbolType type, int isInt);
int 			isInt					  (char * dtype);
int 			
	SymbolTable_createNewSubscopeInCurrent(SymbolTable * st, ScopeType type);
void 			Scope_addSymbol           (Scope * scope, SymbolType type, char * name, int isInt );
void 			
	buildFromCompoundStatement(SymbolTable * st, ASTNode * cmpdStmt, ScopeType stype);
void 			buildFromIfStmt(SymbolTable * st, ASTNode * ifstmt);
void 			
	SymbolTable_setCurrentScope(SymbolTable * st, int subscopeId); 
void 			
	SymbolTable_addVariableToCurrentScope(SymbolTable * st, char * name, char * dtype);
void 
	SymbolTable_addFunctionToCurrentScope(SymbolTable * st, char * name, char * rettype);
void 
	SymbolTable_addParameterToCurrentScope(SymbolTable * st, char * name, char * dtype);

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
			/* TODO(Mar 19/16): getter functions for ASTNode values */
			char * funcName = temp -> children[1] -> value.str;
			char * funcRetType = temp -> children[0] -> value.str;

			/* Create symbol in current scope for the function name */
			SymbolTable_addFunctionToCurrentScope(st, funcName, funcRetType);

			int subscopeId = SymbolTable_createNewSubscopeInCurrent(st, SCOPE_FUNCTION);
			SymbolTable_setCurrentScope(st, subscopeId);

			/* Creates a subscope, holds any local symbols or subscopes */
			buildFromCompoundStatement(st, temp -> children[3], SCOPE_FUNCTION);

			/* Compound statment had no symbols or subscopes */
			/* works because functions cannot be nested */
			if(st -> root -> subscopes[0] == NULL) {
				int subscopeId = SymbolTable_createNewSubscopeInCurrent(st, SCOPE_FUNCTION);
				SymbolTable_setCurrentScope(st, subscopeId);
			}

			/* add function params to subscope */
			ASTNode * paramlist = temp -> children[2];
			ASTNode * param = NULL;
			int j = 0;

			for (j=0 ; (param = paramlist -> children[j]) != NULL ; j++) {
				char * paramName = param -> children[1] -> value.str;
				char * paramType = param -> children[0] -> value.str;

				SymbolTable_addParameterToCurrentScope(st, paramName, paramType);
			}

			/* works because functions cannot be nested */
			st -> currScope = st -> root;

			/* add local vars from compound stmt to scope */
			/* search for compound stmts within compound stmt and do same as above */
		} else if (temp -> type == VAR_DECLARATION) {
			/* add variable name/type to current scope */
			char * varName = temp -> children[1] -> value.str;
			char * varType = temp -> children[0] -> value.str;

			SymbolTable_addVariableToCurrentScope(st, varName, varType);
		}
	}
	return st;
}

void buildFromCompoundStatement(SymbolTable * st, ASTNode * cmpdStmt, ScopeType stype) {
	if (st && cmpdStmt -> type == COMPOUND_STATEMENT) {
		int j = 0;

		/* Add local vars to current scope */
		if (cmpdStmt -> children[0] -> type == LOCAL_VARS) {
			int i = 0;
			ASTNode * localVars = cmpdStmt -> children[0];
			for (i=0 ; i < MAX_CHILDREN && localVars -> children[i] != NULL ; i++) {
				/* TODO(Mar 19/2016: Make this less obscure) */
				char * varName = localVars -> children[i] -> children[1] -> value.str;
				char * varType = localVars -> children[i] -> children[0] -> value.str;
				SymbolTable_addVariableToCurrentScope(st, varName, varType);
			}
		}

		/* Look for nested compound statements */
		ASTNode * substmt = NULL;
		for (j = 0 ; (substmt = cmpdStmt -> children[j]) != NULL ; j++) {
			if (substmt -> type == IF_STATEMENT) {
				buildFromIfStmt(st, substmt);
			}
		}
	}
}

void buildFromIfStmt(SymbolTable * st, ASTNode * ifstmt) {
	if (st && ifstmt -> type == IF_STATEMENT) {
		int i = 0;
		ASTNode * substmt = NULL;
		Scope * temp = st -> currScope;

		for (i = 0 ; (substmt = ifstmt -> children[i]) != NULL ; i++) {
			if (substmt -> type == COMPOUND_STATEMENT){
				int subscopeId = SymbolTable_createNewSubscopeInCurrent(st, SCOPE_SELECTION);
				SymbolTable_setCurrentScope(st, subscopeId);

				buildFromCompoundStatement(st, substmt, SCOPE_SELECTION);
				
				st -> currScope = temp;
			}
		}
	}
}

Symbol * new_Symbol(char * name, SymbolType type, int isInt) {
	Symbol * symbol = malloc(sizeof(symbol));

	symbol -> name = calloc(strlen(name)+1, sizeof(*(symbol->name)));
	strcpy(symbol -> name, name);

	symbol -> type = type;
	symbol -> isInt = isInt;

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

int SymbolTable_createNewSubscopeInCurrent(SymbolTable * st, ScopeType type) {
	int i = 0;
	for (i = 0 ; i < MAX_SUBSCOPES && st -> currScope -> subscopes[i] != NULL ; i++) {} 

	st -> currScope -> subscopes[i] = new_Scope(type);
	st -> currScope -> subscopeCount++;
	return i;
}

void SymbolTable_setCurrentScope(SymbolTable * st, int subscopeId) {
	if (st && st -> currScope && st -> currScope -> subscopes) {
		st -> currScope = st -> currScope -> subscopes[subscopeId];
	}
}

void SymbolTable_addVariableToCurrentScope(SymbolTable * st, char * name, char * dtype) {
	if (st) {
		Scope_addSymbol(st -> currScope, SYMBOL_VAR, name, isInt(dtype));
	}
}

void SymbolTable_addFunctionToCurrentScope(SymbolTable * st, char * name, char * rettype) {
	if (st) {
		Scope_addSymbol(st -> currScope, SYMBOL_FUNCTION, name, isInt(rettype));
	}
}

void SymbolTable_addParameterToCurrentScope(SymbolTable * st, char * name, char * dtype) {
	if (st) {
		Scope_addSymbol(st -> currScope, SYMBOL_FPARAM, name, isInt(dtype));
	}
}


void Scope_addSymbol(Scope * scope, SymbolType type, char * name, int isInt ) {
	if (scope) {
		int i;
		for (i = 0 ; scope -> symbols[i] != NULL ; i++) {}

		if (i < MAX_SYMBOLS) {
			scope -> symbols[i] = new_Symbol(name, type, isInt);
		}
	}
}

int isInt(char * dtype) {
	return strcmp(dtype, "int") == 0 ? 1 : 0;
}
