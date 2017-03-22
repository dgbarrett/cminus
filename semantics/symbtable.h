#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#include "../parse/ast.h"
#include "symbol.h"
#include "hashtable.h"
#include "scope.h"

/*
	Type: SymbolTable
		Holds a series of Scopes which hold definitions for (hopefully) all of 
		the symbols in a program.
*/
typedef struct _SymbolTable {
	Scope * root;
	Scope * currScope;
	int size;
} SymbolTable;	

SymbolTable * buildSymbolTable(ASTNode * root);

#endif