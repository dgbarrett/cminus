#include "checking.h"

#include "../parse/ErrorManager.h"

void checkForRedefinedVariables(ErrorList * errlist, Scope * scope);
void addVariableScopesToAST(ASTNode * ast, SymbolTable * symbtable);
void checkExpressions();

void semanticAnalysis(ASTNode * ast, SymbolTable * symbtable) {
	/* prepare tree for analysis */
	ErrorList * semanticErrors = new_ErrorList();

	checkForRedefinedVariables(semanticErrors, symbtable -> root);
	addVariableScopesToAST(ast, symbtable);

	/* actual analysis */
	checkExpressions();

	ErrorList_print(semanticErrors);
	return;
}

void checkForRedefinedVariables(ErrorList * errlist, Scope * scope) {
	int i = 0;
	for (i = 0 ; i < scope -> symbolCount ; i++) {
		if(!HashTable_insert(scope -> allsymbols, scope -> symbols[i])) {
			char buf[128];
			Symbol * new = scope -> symbols[i];
			Symbol * old = HashTable_get(scope -> allsymbols, scope -> symbols[i] -> name);

			sprintf(buf,"Symbol '%s' redefined.\n\t Previous definition exists at line %d.\n",new->name, old->linenum);
			ErrorList_insert(errlist, new_Error(buf, new->linenum , 0));
		}
	}
}

void addVariableScopesToAST(ASTNode * ast, SymbolTable * symbtable) {
	return;
}

void checkExpressions() {
	return;
}