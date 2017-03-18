#ifndef __AST_PRINT_H__
#define __AST_PRINT_H__

#include <stdio.h>
#include "ast.h"

void printSyntaxTree(ASTNode * root, ASTNodeType parentType, int index, int depth);
void printNodeType(ASTNode * node);

#endif