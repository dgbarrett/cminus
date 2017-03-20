#include "checking.h"

#include "../parse/ErrorManager.h"

void checkForRedefinedVariables(ErrorList * errlist, Scope * scope);
void addVariableScopesToAST(ASTNode * ast, SymbolTable * symbtable);
void checkExpressions();
void generateCompleteSymbolListing(Scope * scope, Scope * parent);
void f(ErrorList * errlist, Scope * scope);

void semanticAnalysis(ASTNode * ast, SymbolTable * symbtable) {
	/* prepare tree for analysis */
	ErrorList * semanticErrors = new_ErrorList();

	f(semanticErrors, symbtable -> root);
	addVariableScopesToAST(ast, symbtable);

	/* actual analysis */
	checkExpressions();

	ErrorList_print(semanticErrors);
	return;
}

void f(ErrorList * errlist, Scope * scope) {
	int i = 0;
	if (scope) {
		checkForRedefinedVariables(errlist, scope);	

		for (i=0 ; i<scope->subscopeCount ; i++) {
			f(errlist, scope -> subscopes[i]);
		}
	}
	
}

void checkForRedefinedVariables(ErrorList * errlist, Scope * scope) {
	int i = 0;
	/* Check for symbols defined in the scope that conflict */
	for (i = 0 ; i < scope -> symbolCount ; i++) {
		if(!HashTable_insert(scope -> allsymbols, scope -> symbols[i])) {
			char buf[128];
			Symbol * new = scope -> symbols[i];
			Symbol * old = HashTable_get(scope -> allsymbols, scope -> symbols[i] -> name);

			if (old -> linenum == 0) {
				sprintf(buf,"Symbol '%s' redefined.\n\t '%s' is defined as part of the C- standard library.\n",new->name, new->name);
			} else {
				sprintf(buf,"Symbol '%s' redefined.\n\t Previous definition exists at line %d.\n",new->name, old->linenum);
			}
			ErrorList_insert(errlist, new_Error(buf, new->linenum , 0));
		}
	}

	/* Roll up */
	generateCompleteSymbolListing(scope, scope -> parent);
}

void generateCompleteSymbolListing(Scope * scope, Scope * parent) {
	if (parent) {
		printf("parent found\n");
		int i = 0;
		for (i = 0 ; i < parent -> symbolCount ; i++) {
			HashTable_insert(scope -> allsymbols, parent -> symbols[i]);
		}

		generateCompleteSymbolListing(scope, parent -> parent);
	}
}

void addVariableScopesToAST(ASTNode * ast, SymbolTable * symbtable) {
	return;
}

void checkExpressions() {
	return;
}