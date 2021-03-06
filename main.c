#include <stdio.h>

#include "parse/parse.h"
#include "parse/ast.h"
#include "parse/ast_print.h"
#include "semantics/symbtable.h"
#include "semantics/symbtable_print.h"
#include "semantics/checking.h"
#include "gen/gen.h"
#include "args.h"

int main(int argc, char *argv[])
{
	if (argc <= MAX_ARGS && argc >= 2) {
		char * sourceFile = NULL;
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
			ASTNode * root = parse(input);

			if (root) {
				SymbolTable * symtable = buildSymbolTable(root);
				int semanticsOK = semanticAnalysis(root, symtable);

				if (semanticsOK) {
					TMCode * asmCode = generateCode(root, sourceFile);
					if (ASM_FLAG) printTMCode(asmCode);
				}

				if (AST_FLAG) printSyntaxTree(root);
				if (SYMBTABLE_FLAG) printSymbolTable(symtable);
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
