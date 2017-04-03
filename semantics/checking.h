#ifndef __CHECKING_H__
#define __CHECKING_H__

#include "../parse/ast.h"
#include "symbtable.h"

int semanticAnalysis(ASTNode * ast, SymbolTable * symbtable);

#endif