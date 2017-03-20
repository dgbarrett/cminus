#include "symbtable.h"

#include <stdlib.h>
#include <string.h>

#define HASH_TABLE_SIZE 499

/* SymbolTable builders */
SymbolTable * buildNonEmptySymbolTable(SymbolTable * st, ASTNode * node);
void buildFromCompoundStatement(SymbolTable * st, ASTNode * cmpdStmt);
void buildFromIfStmt(SymbolTable * st, ASTNode * ifstmt);
void buildFromWhileLoop(SymbolTable * st, ASTNode * loop);

/* SymbolTable */
SymbolTable * new_SymbolTable();
int SymbolTable_createNewSubscopeInCurrent(SymbolTable * st, ScopeType type);
void SymbolTable_setCurrentScope(SymbolTable * st, int subscopeId);
void SymbolTable_addVariableToCurrentScope(SymbolTable * st, char * name, char * dtype, int lineno);
void SymbolTable_addFunctionToCurrentScope(SymbolTable * st, char * name, char * rettype, int lineno);
void SymbolTable_addParameterToCurrentScope(SymbolTable * st, char * name, char * dtype, int lineno);
void SymbolTable_addArrayParameterToCurrentScope(SymbolTable * st, char * name, char * dtype, int lineno);
void SymbolTable_addArrayToCurrentScope(SymbolTable * st, char * name, char * dtype, int arrSize, int lineno);

/* Symbol */
Symbol * new_Symbol(char * name, SymbolType type, int isInt, int arrSize, int lineno);

/* Scope */
Scope * new_Scope(ScopeType type);
void Scope_addSymbol(Scope * scope, SymbolType type, char * name, int isInt, int arrSize, int lineno);

/* SymbolHashTable */
SymbolHashTable * new_SymbolHashTable();
Symbol * HashTable_get(SymbolHashTable * st, char * name);
int HashTable_hash(char * name);
int HashTable_insert(SymbolHashTable * st, Symbol * symbol);

/* Misc */
int isInt(char * dtype);

/*************** Funcitons for building the SymbolTable ***************/

/*
	Function: buildSymbolTable
		Builds and returns a symbol table when passed the root of an Abstract
		Syntax Tree.
*/
SymbolTable * buildSymbolTable(ASTNode * root) {
	SymbolTable * st = new_SymbolTable();

	/* Library functions */
	SymbolTable_addFunctionToCurrentScope(st, "input", "int", 0);
	SymbolTable_addFunctionToCurrentScope(st, "output", "void", 0);

	if (root) {
		if (root->children) {
			return buildNonEmptySymbolTable(st, root);
		} else return st;
	} else return NULL;
}

/* 
	Function: buildNonEmptySymbolTable
		Builds a symbol table from the root of an AST that contains symbols 
		beyond only the standard library functions.
*/
SymbolTable * buildNonEmptySymbolTable(SymbolTable * st, ASTNode * root) {
	int i = 0, j = 0;
	ASTNode * declaration = NULL;

	for (i=0 ; (declaration = root -> children[i]) != NULL ; i++) {
		if (declaration -> type == FUNCTION_DECLARATION) {
			/* add function name/ret type to current scope */
			/* TODO(Mar 19/16): getter functions for ASTNode values */
			char * funcName = declaration -> children[1] -> value.str;
			char * funcRetType = declaration -> children[0] -> value.str;

			/* Create symbol in current scope for the function name */
			SymbolTable_addFunctionToCurrentScope(st, funcName, funcRetType, declaration -> linenum);

			/* Create new subscope in file root scope for the above function.*/
			SymbolTable_setCurrentScope(
				st, 
				SymbolTable_createNewSubscopeInCurrent(st, SCOPE_FUNCTION)
			);

			/* Build the symbol table for the compound statement. */
			buildFromCompoundStatement(st, declaration -> children[3]);

			/* Add function params to subscope */
			ASTNode * paramlist = declaration -> children[2];
			ASTNode * param = NULL;
			for (j=0 ; (param = paramlist -> children[j]) != NULL ; j++) {
				char * paramName = param -> children[1] -> value.str;
				char * paramType = param -> children[0] -> value.str;

				if (param -> type == ARRAY_PARAMETER) {
					SymbolTable_addArrayParameterToCurrentScope(st, paramName, paramType, param -> linenum);
				} else {
					SymbolTable_addParameterToCurrentScope(st, paramName, paramType, param -> children[0] -> linenum);
				}
			}

			/* Reset scope to root so entire SymbolTable can be accessed*/
			st -> currScope = st -> root;
		} else if (declaration -> type == VAR_DECLARATION) {
			/* Add variable name/type to current scope */
			char * varName = declaration -> children[1] -> value.str;
			char * varType = declaration -> children[0] -> value.str;

			SymbolTable_addVariableToCurrentScope(st, varName, varType, declaration -> children[0] -> linenum);
		}
	}
	return st;
}

/*
	Function: buildFromCompoundStatement
		Adds the local vars from cmpdStmt to the current scope, creates any
		necessary subscopes.
*/
void buildFromCompoundStatement(SymbolTable * st, ASTNode * cmpdStmt) {
	int i = 0, j = 0;
	if (st && cmpdStmt -> type == COMPOUND_STATEMENT) {

		/* Add local vars to current scope */
		if (cmpdStmt -> children[0] -> type == LOCAL_VARS) {

			/* Add each variable declared in the local variables */
			ASTNode * localVars = cmpdStmt -> children[0];
			ASTNode * localVar = NULL;
			for (i=0 ; i < MAX_CHILDREN && (localVar = localVars -> children[i]) != NULL ; i++) {
				char * varName = localVar -> children[1] -> value.str;
				char * varType = localVar -> children[0] -> value.str;

				if (localVar -> type == VAR_ARRAY_DECLARATION) {
					int arrSize = localVar -> children[2] -> value.num;
					SymbolTable_addArrayToCurrentScope(st, varName, varType, arrSize, localVar->children[0]->linenum);
				} else {
					SymbolTable_addVariableToCurrentScope(st, varName, varType, localVar->children[0]->linenum);
				}
			}
		}

		/* Look for nested compound statements */
		ASTNode * substmt = NULL;
		for (j = 0 ; (substmt = cmpdStmt -> children[j]) != NULL ; j++) {
			switch(substmt -> type) {
				case IF_STATEMENT:
					buildFromIfStmt(st, substmt);
					break;
				case WHILE_LOOP:
					buildFromWhileLoop(st, substmt);
					break;
				default:;
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

				buildFromCompoundStatement(st, substmt);
				st -> currScope = temp;
			} 
		}
	}
}

void buildFromWhileLoop(SymbolTable * st, ASTNode * loop) {
	if (st && loop -> type == WHILE_LOOP) {
		Scope * temp = st -> currScope;

		int subscopeId = SymbolTable_createNewSubscopeInCurrent(st, SCOPE_LOOP);
		SymbolTable_setCurrentScope(st, subscopeId);

		buildFromCompoundStatement(st, loop->children[1]);
		st -> currScope = temp;
	}
}

/*************** SymbolTable ***************/

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
	st -> currScope -> subscopes[i] -> parent = st -> currScope;
	st -> currScope -> subscopeCount++;
	return i;
}

void SymbolTable_setCurrentScope(SymbolTable * st, int subscopeId) {
	if (st && st -> currScope && st -> currScope -> subscopes) {
		st -> currScope = st -> currScope -> subscopes[subscopeId];
	}
}

void SymbolTable_addVariableToCurrentScope(SymbolTable * st, char * name, char * dtype, int lineno) {
	if (st) {
		Scope_addSymbol(st -> currScope, SYMBOL_VAR, name, isInt(dtype), -1, lineno);
	}
}

void SymbolTable_addFunctionToCurrentScope(SymbolTable * st, char * name, char * rettype, int lineno) {
	if (st) {
		Scope_addSymbol(st -> currScope, SYMBOL_FUNCTION, name, isInt(rettype), -1, lineno);
	}
}

void SymbolTable_addParameterToCurrentScope(SymbolTable * st, char * name, char * dtype, int lineno) {
	if (st) {
		Scope_addSymbol(st -> currScope, SYMBOL_FPARAM, name, isInt(dtype), -1, lineno);
	}
}

void SymbolTable_addArrayParameterToCurrentScope(SymbolTable * st, char * name, char * dtype, int lineno) {
	if (st) {
		Scope_addSymbol(st -> currScope, SYMBOL_FARRAYPARAM, name, isInt(dtype), -1, lineno);
	}
}


void SymbolTable_addArrayToCurrentScope(SymbolTable * st, char * name, char * dtype, int arrSize, int lineno) {
	if (st) {
		Scope_addSymbol(st -> currScope, SYMBOL_ARRAY, name, isInt(dtype), arrSize, lineno);
	}
}

/*************** Symbol ***************/

Symbol * new_Symbol(char * name, SymbolType type, int isInt, int arrSize, int lineno) {
	Symbol * symbol = malloc(sizeof(*symbol));

	symbol -> name = calloc(strlen(name)+1, sizeof(*(symbol->name)));
	strcpy(symbol -> name, name);

	symbol -> type = type;
	symbol -> isInt = isInt;
	symbol -> arrlen = arrSize;
	symbol -> linenum = lineno;

	return symbol;
}

/*************** Scope ***************/

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

/*************** SymbolHashTable ***************/

SymbolHashTable * new_SymbolHashTable() {
	SymbolHashTable * st = malloc(sizeof(*st));

	st -> symbols = calloc(HASH_TABLE_SIZE, sizeof(*(st->symbols)));
	st -> maxsize = HASH_TABLE_SIZE;
	st -> size = 0;

	return st;
}

int HashTable_insert(SymbolHashTable * st, Symbol * symbol) {
	if (st && symbol) {
		int hash = HashTable_hash(symbol -> name);
		if (st -> symbols[hash] != NULL) return 0;
		else {
			st -> symbols[hash] = symbol;
			st -> size++;
			return 1;
		}
	} else return 0;
}

Symbol * HashTable_get(SymbolHashTable * st, char * name) {
	if (st && name) {
		int hash = HashTable_hash(name);
		return st -> symbols[hash];
	}
	return NULL;
}

int HashTable_hash(char * name) {
	if (name) {
		int i = 0, temp = 0;
		for (i = 0 ; name[i] != '\0' ; i++) {
			temp = ((temp << 4) + (name[i] * (i+222))) % HASH_TABLE_SIZE;
		}	
		return temp;
	} else return -1;
}

/*************** Misc ***************/

int isInt(char * dtype) {
	return strcmp(dtype, "int") == 0 ? 1 : 0;
}
