#include <stdio.h>

#include "parse.h"
#include "SyntaxTree.h"

int main(int argc, char const *argv[])
{
	/*
	initScanner();

	int tok;
	while ( (tok = getToken() ) != ENDFILE ) {
		printToken(tok);
	}
	*/

	ASTNode * root = parse();
	printf("\n");
	printSyntaxTree(root,0);
	printf("\n");
	
	return 0;
}
