#include "checking.h"

void checkForRedefinedVariables(Scope * scope);
void addVariableScopesToAST(ASTNode * ast, SymbolTable * symbtable);
void checkExpressions();

void semanticAnalysis(ASTNode * ast, SymbolTable * symbtable) {
	/* prepare tree for analysis */
	checkForRedefinedVariables(symbtable -> root);
	addVariableScopesToAST(ast, symbtable);

	/* actual analysis */
	checkExpressions();
	return;
}

void checkForRedefinedVariables(Scope * scope) {
	int i = 0;
	for (i = 0 ; i < scope -> symbolCount ; i++) {
		if(!HashTable_insert(scope -> allsymbols, scope -> symbols[i])) {
			printf("Redefined symbol found!\n");
		}
	}
}

void addVariableScopesToAST(ASTNode * ast, SymbolTable * symbtable) {
	return;
}

void checkExpressions() {
	return;
}