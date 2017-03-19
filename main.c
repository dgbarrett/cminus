#include <stdio.h>

#include "parse/parse.h"
#include "parse/ast.h"
#include "parse/ast_print.h"
#include "semantics/SymbolTable.h"
#include "args.h"

int main(int argc, char const *argv[])
{
	if (argc <= MAX_ARGS && argc >= 2) {
		const char * sourceFile = NULL;
		FILE * input;
		int i;

		/* get input file */
		for (i = 1 ; i < argc ; i++) {
			if (isCMinusFile(argv[i])){
				sourceFile = argv[i];
			}

			if (isFlag(argv[i])) {
				setFlag(argv[i]);
			}
		}

		input = fopen(sourceFile, "r");

		if (input) {
			/* parse out ast */
			ASTNode * root = parse(input);
			SymbolTable * symtable = buildSymbolTable(root);

			/* print if requested */
			if (root) {
				if (AST_FLAG) {
					printf("\n");
					printSyntaxTree(root, NIL, 0, 0);
					printf("\n");
				}

				if (SYMBTABLE_FLAG) {
					printf("\n");
					printf("[DUMMY SYMBOL TABLE PRINT]\n");
					/* printSymbolTable(root) */
					printf("\n");
				}
			}
			
		} else{
			fprintf(stderr, "Invalid file name. Exiting.\n");
			exit(0);
		}
	} else {
		fprintf(stderr, "Too many arguments provided.  Expected max of %d, found %d\n", MAX_ARGS, argc);
	}

	return 0;
}
