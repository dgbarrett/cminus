#ifndef __SYMBTABLE_PRINT_H__
#define __SYMBTABLE_PRINT_H__

#include "symbtable.h"

void printSymbolTable	(SymbolTable * st);
void printScopeType		(ScopeType type, int depth);
void printSymbols		(Symbol ** symbols, int depth);

#endif