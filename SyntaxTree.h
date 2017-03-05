#ifndef __SYNATX_TREE_H__
#define __SYNATX_TREE_H__

#include <stdlib.h>

typedef struct _ASTNode {
	void * node;
} ASTNode;

typedef struct _SyntaxTree {
	void * nothing;
} SyntaxTree;

SyntaxTree * new_SyntaxTree();

#endif