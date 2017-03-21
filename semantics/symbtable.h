#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#include "../parse/ast.h"
#include "symbol.h"
#include "hashtable.h"
#include "scope.h"

typedef struct _SymbolTable {
	Scope * root;
	Scope * currScope;
	int size;
} SymbolTable;	

SymbolTable * buildSymbolTable(ASTNode * root);

#endif