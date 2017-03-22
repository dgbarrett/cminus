#include "symbtable.h"

#include <stdlib.h>
#include <string.h>

/* SymbolTable builders */
SymbolTable * 	buildNonEmptySymbolTable	(SymbolTable * st, ASTNode * node);
void 			buildFromCompoundStatement	(SymbolTable * st, ASTNode * cmpdStmt);
void 			buildFromIfStmt				(SymbolTable * st, ASTNode * ifstmt);
void 			buildFromWhileLoop			(SymbolTable * st, ASTNode * loop);

/* SymbolTable */
SymbolTable * 	new_SymbolTable 							();
int 			SymbolTable_createNewSubscopeInCurrent 		(SymbolTable * st, ScopeType type);
Symbol * 		SymbolTable_getFromRootScope	 			(SymbolTable * st, char * name);
Symbol * 		SymbolTable_getFromCurrentScope(SymbolTable * st, char * name);
void 			SymbolTable_setCurrentScope 				(SymbolTable * st, int subscopeId);
void 			SymbolTable_addVariableToCurrentScope 		(SymbolTable * st, char * name, char * dtype, int lineno);
void 			SymbolTable_addFunctionToCurrentScope 		(SymbolTable * st, char * name, char * rettype, int lineno);
void 			SymbolTable_addParameterToCurrentScope 		(SymbolTable * st, char * name, char * dtype, int lineno);
void 			SymbolTable_addArrayParameterToCurrentScope	(SymbolTable * st, char * name, char * dtype, int lineno);
void 			SymbolTable_addArrayToCurrentScope 			(SymbolTable * st, char * name, char * dtype, int arrSize, int lineno);

/* Misc */
int isInt(char * dtype);

/*************** Functions for building the SymbolTable ***************/

/*
	Function: buildSymbolTable
		Builds and returns a symbol table when passed the root of an Abstract
		Syntax Tree.
		Building a SymbolTable annotates the AST with pointers to their scopes
		in the relevant places.
*/
SymbolTable * buildSymbolTable(ASTNode * root) {
	SymbolTable * st = new_SymbolTable();

	/* Library functions */
	SymbolTable_addFunctionToCurrentScope(st, "input", "int", 0);
	SymbolTable_addFunctionToCurrentScope(st, "output", "void", 0);

	Symbol_addToFunctionSignature(
		SymbolTable_getFromRootScope(st, "output"),
		TYPE_INT
	);

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
		**Scope does not change on call.
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
					SymbolTable_addArrayParameterToCurrentScope(st, paramName, paramType, param -> children[0] -> linenum);
				} else {
					SymbolTable_addParameterToCurrentScope(st, paramName, paramType, param -> children[0] -> linenum);
				}

				Symbol * function = SymbolTable_getFromRootScope(st, funcName);
				Symbol * param = SymbolTable_getFromCurrentScope(st, paramName);
				Symbol_addToFunctionSignature(function, param -> datatype);

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
		**Scope does not change on call.
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

		cmpdStmt -> scope = st -> currScope;

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

/*
	Function: buildFromIfStmt
		Captures and creates a new subscope for each compound statement in an 
		if statment and adds them as subscopes to the current scope.
		**Scope does not change on call.
*/
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

/*
	Function: buildFromWhileLoop
		Captures and creates a new subscope for the compound statement in a 
		while loop and adds it as a subscope to the current scope.
		**Scope does not change on call.
*/
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

/*
	Function: new_SymbolTable
		Creates a new SymbolTable object.
*/
SymbolTable * new_SymbolTable() {
	SymbolTable * st = malloc(sizeof(*st));

	st -> size = 0;
	st -> root = new_Scope(SCOPE_FILE);
	st -> currScope = st -> root;

	return st;
}

/*
	Function: SymbolTable_createNewSubscopeInCurrent
		Creates a new Scope object as a subscope in the current scope. Returns
		the local id of the subscope which can be used to switch scope. 
*/
int SymbolTable_createNewSubscopeInCurrent(SymbolTable * st, ScopeType type) {
	int i = 0;

	/* Find where to insert the new subscope */
	for (
		i = 0 ; 
		i < MAX_SUBSCOPES && st -> currScope -> subscopes[i] != NULL ; 
		i++
	) {} 

	st -> currScope -> subscopes[i] = new_Scope(type);
	st -> currScope -> subscopes[i] -> parent = st -> currScope;
	st -> currScope -> subscopeCount++;

	return i;
}

Symbol * SymbolTable_getFromRootScope(SymbolTable * st, char * name) {
	if (st && st -> root) {
		int i;
		for (i=0 ; st -> root -> symbols && st -> root -> symbols[i] ; i++) {
			if (strcmp(name, st -> root -> symbols[i] -> name) == 0) {
				return st -> root -> symbols[i];
			}
		}
	}

	return NULL;
}

Symbol * SymbolTable_getFromCurrentScope(SymbolTable * st, char * name) {
	if (st && st -> currScope) {
		int i;
		for (i=0 ; st -> currScope -> symbols && st -> currScope -> symbols[i] ; i++) {
			if (strcmp(name, st -> currScope -> symbols[i] -> name) == 0) {
				return st -> currScope -> symbols[i];
			}
		}
	}

	return NULL;
}

/* 
	Function: SymbolTable_setCurrentScope
		Switch scope to the subscope local to the current scope with 
		id == subscopeId.
*/
void SymbolTable_setCurrentScope(SymbolTable * st, int subscopeId) {
	if (st && st -> currScope && st -> currScope -> subscopes) {
		st -> currScope = st -> currScope -> subscopes[subscopeId];
	}
}

/*
	Function: SymbolTable_addVariableToCurrentScope
		Add a variable to the current scope. dtype is the data type of the 
		variable, lineno is the line number that the variable was declared on, 
		name is the name of the variable.
*/
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

/*************** Misc ***************/

int isInt(char * dtype) {
	return strcmp(dtype, "int") == 0 ? 1 : 0;
}
