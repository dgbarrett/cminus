#ifndef __CHECKING_H__
#define __CHECKING_H__

#include "../parse/ast.h"
#include "symbtable.h"
#include "hashtable.h"

void semanticAnalysis(ASTNode * ast, SymbolTable * symbtable);

#endif