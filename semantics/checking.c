#include "checking.h"

void checkForRedefinedVariables(SymbolTable * symbtable);
void addVariableScopesToAST(ASTNode * ast, SymbolTable * symbtable);
void checkExpressions();

void semanticAnalysis(ASTNode * ast, SymbolTable * symbtable) {
	/* prepare tree for analysis */
	/* roll up checks for redefinitions */
	checkForRedefinedVariables(symbtable);
	addVariableScopesToAST(ast, symbtable);

	/* actual analysis */
	checkExpressions();
	return;
}

void checkForRedefinedVariables(SymbolTable * symbtable) {
	return;
}

void addVariableScopesToAST(ASTNode * ast, SymbolTable * symbtable) {
	return;
}

void checkExpressions() {
	return;
}