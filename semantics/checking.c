#include "checking.h"

#include "../errors/ErrorManager.h"
#include "../parse/ast_print.h"
#include "symbtable_print.h"

void 	checkForRedefinedVariables 		(ErrorList * errlist, Scope * scope);
void 	checkExpressionTypes				(ErrorList * errlist, ASTNode * node);
void checkConditionTypes(ErrorList * errlist, ASTNode * node);
void checkFunctionSignatures(ErrorList * errlist, ASTNode * node);
void 	generateCompleteSymbolListing	(Scope * scope, Scope * parent);
void 	checkScopeForRedefinedVariables	(ErrorList * errlist, Scope * scope);
char *	getSubexpressionName(ASTNode * subexpr);
SymbolDataType 	evaluateType					(ErrorList * errlist, ASTNode * subexpr);
SymbolDataType eevaluateType( ASTNode * expr);

/* NEW */
void root(ASTNode * root, ErrorList * errlist);
void validateDeclaration(ASTNode * declaration, ErrorList * errlist);
void validateFunctionDeclaration(ASTNode * function, ErrorList * errlist);
void validateCompoundStatement(ASTNode * fbody, ErrorList * errlist);
void validateStatement(ASTNode * stmt, ErrorList * errlist);
void validateIfStatement(ASTNode * ifstmt, ErrorList * errlist);
void validateCondition(ASTNode * condition, ErrorList * errlist);
void validateArrayElement(ASTNode * arrayElem, ErrorList * errlist);
void validateExpression(ASTNode * expression, ErrorList * errlist);
void validateFunctionCall(ASTNode * fcall, ErrorList * errlist);
void validateReturnStatement(ASTNode * retstmt, ErrorList * errlist);
/**/


void root(ASTNode * root, ErrorList * errlist) {
	int i = 0;
	for (i = 0 ; root -> children && root -> children[i] ; i++) {
		validateDeclaration(root -> children[i], errlist);
	}
}

void validateDeclaration(ASTNode * declaration, ErrorList * errlist) {
	if (declaration -> type == FUNCTION_DECLARATION) {
		validateFunctionDeclaration(declaration, errlist);
	}
}

int isReturnableFrom(ASTNode * stmt, ErrorList * errlist) {
	if (stmt -> type == RETURN_STATEMENT) {
		return 1;
	} else if (stmt -> type == IF_STATEMENT) {
		return isReturnableFrom(stmt -> children[1], errlist) &&
			   ( (stmt -> children[2]) && isReturnableFrom(stmt -> children[2], errlist) );
	} else if (stmt -> type == COMPOUND_STATEMENT) {
		int i = 0;
		for (i = 0; i < 25 && stmt -> children[i] ; i++) {
			if (isReturnableFrom(stmt->children[i], errlist)) {
				return 1;
			}
		}
		return 0;
	} else {
		return 0;
	}
}

void validateFunctionDeclaration(ASTNode * function, ErrorList * errlist) {
	int i = 0;
	char * functionName = function -> children[1] -> value.str;
	ASTNode * body = function -> children[3];
	SymbolDataType rettype = SymbolDataType_fromString( function -> children[0] -> value.str );

	validateCompoundStatement(body, errlist);

	for(i=0; body -> children && body -> children[i] ; i++) {}

	/* If theres a functions supposed to return int without a return statement*/
	if (i < 25 && 
		rettype == TYPE_INT && 
		body -> children[i-1] -> type != RETURN_STATEMENT) 
	{
		if (!isReturnableFrom(body, errlist)) {
			ErrorList_insert(
				errlist, 
				new_Error(
					ErrTemplate_MissingReturnStmt(functionName), 
					function -> children[1] -> linenum, 0
				)
			);
		} 
	}
}

void validateCompoundStatement(ASTNode * fbody, ErrorList * errlist) {
	int i = 0;
	for (i = 0; fbody -> children && fbody -> children[i] && i < 25 ; i++) {
		validateStatement(fbody -> children[i], errlist);
	}
}

void validateStatement(ASTNode * stmt, ErrorList * errlist) {
	switch (stmt -> type) {
			case IF_STATEMENT:
				validateIfStatement(stmt, errlist);
				break;
			case WHILE_LOOP:
				validateIfStatement(stmt, errlist);
				break;
			case COMPOUND_STATEMENT:
				validateCompoundStatement(stmt, errlist);
				break;
			case RETURN_STATEMENT:
				validateReturnStatement(stmt, errlist);
				break;
			case FUNCTION_CALL:
				validateFunctionCall(stmt, errlist);
				break;
			case VAR_ARRAY_ELEMENT:
				validateArrayElement(stmt, errlist);
				break;
			case EXPRESSION:
				validateExpression(stmt, errlist);
				break;
			default:
				break;
		}
}

void validateIfStatement(ASTNode * ifstmt, ErrorList * errlist) {
	int i = 0;
	for (i = 0; ifstmt -> children && ifstmt -> children[i] ; i++) {
		if (i == 0) {
			validateCondition(ifstmt -> children[i], errlist);
		} else {
			validateStatement(ifstmt -> children[i], errlist);
		}
	}
}

void validateCondition(ASTNode * condition, ErrorList * errlist) {
	/* Validate if condition is expression */
	if (condition -> type == EXPRESSION) {
		validateExpression(condition, errlist);
	} else if (condition -> type == VAR_ARRAY_ELEMENT) {
		validateArrayElement(condition, errlist);
	}

	/* Make sure condition is integer */
	SymbolDataType condType = eevaluateType(condition);
	if (condType != TYPE_INT) {
		ErrorList_insert(
			errlist, 
			new_Error(
				ErrTemplate_InvalidConditionType(
					(condition -> parent -> type == IF_STATEMENT) ? "If" : "While loop",
					getSubexpressionName(condition),
					SymbolDataType_toString(condType)
				), 
				condition -> linenum, 0
			)
		);
	}
}

void validateArrayElement(ASTNode * arrayElem, ErrorList * errlist) {
	if (arrayElem) {
		char * arrName = arrayElem -> children[0] -> value.str;
		int arrlen;
		ASTNode * index = arrayElem -> children[1];
		SymbolHashTable * enclosingScope = ASTNode_getEnclosingScope(arrayElem);
		Symbol * arrayDef, *indexSymbol;
		SymbolDataType datatype;

		arrayDef = HashTable_get(enclosingScope, arrName);
		arrlen = index -> value.num;

		if (!arrayDef) {
			ErrorList_insert(
				errlist, 
				new_Error(
					ErrTemplate_UndefinedArray(arrName), 
					arrayElem -> linenum, 0
				)
			);
		} 

		switch(index -> type) {
			case NUMBER:
				if (arrayDef && arrayDef -> arrlen <= arrlen) {
					ErrorList_insert(
						errlist, 
						new_Error(
							ErrTemplate_ArrayOutOfBounds(arrName, arrlen, arrayDef -> arrlen), 
							arrayElem -> linenum, 0
						)
					);
				}
				break;
			case IDENTIFIER:
				indexSymbol = HashTable_get(enclosingScope, index -> value.str);

				if (!indexSymbol) {
					ErrorList_insert(
						errlist, 
						new_Error(
							ErrTemplate_UndefinedArrayIndexId(arrName, index -> value.str), 
							arrayElem -> linenum, 0
						)
					);
				} 
				break;
			case EXPRESSION:
				validateExpression(index,errlist);
				break;
			case FUNCTION_CALL:
				validateFunctionCall(index, errlist);
				break;
			case VAR_ARRAY_ELEMENT:
				validateArrayElement(index, errlist);
				break;
			default:;
		}

		datatype = eevaluateType(index);
		if(datatype != TYPE_INT) {
			ErrorList_insert(
				errlist, 
				new_Error(
					ErrTemplate_InvalidArrayAccessType(arrName, getSubexpressionName(index), SymbolDataType_toString(datatype)), 
					arrayElem -> linenum, 0
				)
			);
		}
	}
}

void validateExpression(ASTNode * expression, ErrorList * errlist) {
	if (expression) {
		SymbolDataType type1, type2;
		SymbolHashTable * enclosingScope = ASTNode_getEnclosingScope(expression);
		Symbol * id = NULL;

		switch( expression -> type ) {
			case EXPRESSION:
				validateExpression(expression -> children[0], errlist);
				validateExpression(expression -> children[1], errlist);

				type1 = eevaluateType(expression -> children[0]);
				type2 = eevaluateType(expression -> children[1]);

				if (type1 != type2) {
					ErrorList_insert(
						errlist, 
						new_Error(
							ErrTemplate_MismatchedExprType(
								Operator_toString(expression -> value.operation),
								getSubexpressionName(expression -> children[0]),
								SymbolDataType_toString(type1),
								getSubexpressionName(expression -> children[1]),
								SymbolDataType_toString(type2)
							), 
							expression -> linenum, 0
						)
					);
				}
				break;
			case IDENTIFIER:
				id = HashTable_get(enclosingScope, expression -> value.str);

				if (!id) {
					ErrorList_insert(
						errlist, 
						new_Error(
							ErrTemplate_UndefinedSymbol(expression -> value.str), 
							expression -> linenum, 0
						)
					);
				}
				break;
			case VAR_ARRAY_ELEMENT:
				validateArrayElement(expression, errlist);
				break;
			case FUNCTION_CALL:
				validateFunctionCall(expression, errlist);
				break;
			default:;
		}
	}
}

void validateFunctionCall(ASTNode * fcall, ErrorList * errlist) {
	if (fcall && fcall -> type == FUNCTION_CALL) {
		int i = 0;
		char * name;
		char * functionName = fcall -> children[0] -> value.str;

		ASTNode * arglist = fcall -> children[1];
		SymbolHashTable * enclosingScope = ASTNode_getEnclosingScope(arglist);
		Symbol * function = HashTable_get(enclosingScope, functionName);

		SymbolDataType * expectedSignature = NULL;
		SymbolDataType * actualSignature = calloc(11, sizeof(*actualSignature));

		int actualLen = 0;
		int expectedLen = 0;

		Symbol * symbol;
		for (i = 0 ; arglist -> children[i] ; i++) {
			switch( arglist -> children[i] -> type ) {
				case IDENTIFIER:
					name = arglist -> children[i] -> value.str;
					symbol = HashTable_get(enclosingScope, name);

					if (!symbol) {
						ErrorList_insert(
							errlist, 
							new_Error(
								ErrTemplate_UndefinedSymbolInFCall(functionName, name), 
								fcall -> linenum, 0
							)
						);
					}
					break;
				case VAR_ARRAY_ELEMENT:
					validateArrayElement(arglist -> children[i], errlist);
					break;
				case EXPRESSION:
					validateExpression(arglist -> children[i], errlist);
					break;
				case FUNCTION_CALL:
					validateFunctionCall(arglist -> children[i], errlist);
					break;
				default:;
			}

			actualSignature[i] = eevaluateType(arglist -> children[i]);
			actualLen++;
		}

		/* Wait for here to check function so we can print the signature of the
		 function that is not defined yet in the error message */
		if (!function) {
			ErrorList_insert(
				errlist, 
				new_Error(
					ErrTemplate_UndefinedFunction(functionName, Symbol_callSignatureToString(actualSignature)), 
					fcall -> linenum, 0
				)
			);
			return;
		} else {
			expectedSignature = function -> signature;
			expectedLen = function -> signatureElems;
		}

		if (actualLen != expectedLen) {
			ErrorList_insert(
				errlist, 
				new_Error(
					ErrTemplate_MismatchedSignature(
						functionName, 
						Symbol_callSignatureToString(expectedSignature), 
						Symbol_callSignatureToString(actualSignature)
					), 
					fcall -> linenum, 0
				)
			);
		} else {
			for (i=0; i < actualLen ; i++) {
				if (expectedSignature[i] != actualSignature[i]) {
					ErrorList_insert(
						errlist, 
						new_Error(
							ErrTemplate_MismatchedSignature(
								functionName, 
								Symbol_callSignatureToString(expectedSignature), 
								Symbol_callSignatureToString(actualSignature)
							), 
							fcall -> linenum, 0
						)
					);
					break;
				}
			}
		}
		/* get the enclosing scope */
	}
}

void validateReturnStatement(ASTNode * retstmt, ErrorList * errlist) {
	ASTNode * function;
	SymbolDataType returnType = TYPE_VOID;
	SymbolDataType expectedReturnType;

	function = ASTNode_getEnclosingFunction(retstmt);
	expectedReturnType = SymbolDataType_fromString(function -> children[0] -> value.str);

	if (retstmt -> children && retstmt -> children[0]) {
		validateExpression(retstmt -> children[0], errlist);
		returnType = eevaluateType(retstmt -> children[0]);
	} else {
		returnType = TYPE_VOID;
	}

	/* Print error message if required */
	if (expectedReturnType != returnType) {
		/* Function is not returning the type we expected */
		printf("TEMP - Function return type is incorrect\n");
	}
}



void semanticAnalysis(ASTNode * ast, SymbolTable * symbtable) {
	/* List of semantic errors */
	ErrorList * semanticErrors = new_ErrorList();

	checkScopeForRedefinedVariables(semanticErrors, symbtable -> root);

	root(ast, semanticErrors);

	/* Print any errors */
	ErrorList_print(semanticErrors);
}

/*
	Function: checkScopeForRedefinedVariables
		Checks a scope and all its subscopes for redefined variables.
*/
void checkScopeForRedefinedVariables(ErrorList * errlist, Scope * scope) {
	int i = 0;
	if (scope) {
		checkForRedefinedVariables(errlist, scope);	

		for (i=0 ; i<scope->subscopeCount ; i++) {
			checkScopeForRedefinedVariables(errlist, scope -> subscopes[i]);
		}
	}
}
/*
	Function: checkForRedefinedVariables
		Check a Scope to see if duplicate symbols are defined within it.  If 
		there are duplicates, store a new Error in the ErrorList.  The function
		also generates a SymbolHashTable in the Scope storing all symbols 
		accessible from the Scope. 
*/
void checkForRedefinedVariables(ErrorList * errlist, Scope * scope) {
	int i = 0;
	/* Check for symbols defined in the scope that conflict */
	for (i = 0 ; i < scope -> symbolCount ; i++) {
		if(!HashTable_insert(scope -> allsymbols, scope -> symbols[i])) {
			char * errMsg;
			Symbol * new = scope -> symbols[i];
			Symbol * old = HashTable_get(scope -> allsymbols, scope -> symbols[i] -> name);

			if (old -> linenum == 0) {
				errMsg = ErrTemplate_RedefinedStdlibSymbol(new -> name);
			} else {
				errMsg = ErrTemplate_RedefinedSymbol(new -> name, old -> linenum);
			}

			ErrorList_insert(
				errlist, 
				new_Error(
					errMsg,
					new->linenum, 
					0
				)
			);
		}
	}

	/* Roll up complete symbol table. */
	generateCompleteSymbolListing(scope, scope -> parent);
}

/*
	Function: generateCompleteSymbolListing
		Traverse from a given scope to the root scope collecting a listing 
		of symbols accessible from scope.  The most local symbol overrides any
		others above it with the same name.
*/
void generateCompleteSymbolListing(Scope * scope, Scope * parent) {
	if (parent) {
		int i = 0;
		for (i = 0 ; i < parent -> symbolCount ; i++) {
			HashTable_insert(scope -> allsymbols, parent -> symbols[i]);
		}

		generateCompleteSymbolListing(scope, parent -> parent);
	}
}

char * getSubexpressionName(ASTNode * subexpr) {
	char * name = calloc(64, sizeof(*name));
	char * num = NULL;
	char * n1, *n2, *op;
	/* Error Handling */

	switch(subexpr -> type) {
		case IDENTIFIER:
			strcpy(name, subexpr -> value.str);
			return name;
		case NUMBER:
			num = calloc(16, sizeof(*num));
			sprintf(num, "%d", subexpr -> value.num);
			free(name);
			return num;
		case FUNCTION_CALL:
			strcpy(name, "call to ");
			strcat(name, subexpr -> children[0] -> value.str);
			return name;
		case EXPRESSION:
			n1 = getSubexpressionName(subexpr -> children[0]);
			n2 = getSubexpressionName(subexpr -> children[1]);
			op = Operator_toString(subexpr -> value.operation);
			sprintf(name, "%s %s %s", n1, op, n2);
			return name;
		case VAR_ARRAY_ELEMENT:
			num = calloc(16, sizeof(*num));
			strcpy(name, subexpr -> children[0] -> value.str);
			sprintf(num, "[%s]", getSubexpressionName(subexpr -> children[1]));
			strcat(name, num);
			return name;
		default:
			free(name);
			return "??NoName??";
	}
}

SymbolDataType eevaluateType( ASTNode * expr) {
	if (expr) {
		SymbolHashTable * enclosingScope = ASTNode_getEnclosingScope(expr);
		Symbol * temp = NULL;
		SymbolDataType t1,t2;

		switch(expr -> type) {
			case NUMBER:
				return TYPE_INT;
			case IDENTIFIER:
				temp = HashTable_get(enclosingScope, expr -> value.str);
				if ( temp ) {
					return temp -> datatype;
				} else {
					return TYPE_INT;
				}
			case FUNCTION_CALL:
				return eevaluateType(expr -> children[0]);
			case VAR_ARRAY_ELEMENT:
				/* evaluate the type of the element access */
				t1 = eevaluateType(expr -> children[1]);
				temp = HashTable_get(enclosingScope, expr -> children[0] -> value.str);

				if (temp) {
					return SymbolDataType_parentType(temp -> datatype);
				} else return TYPE_INT;
			case EXPRESSION:
				t1 = eevaluateType(expr->children[0]);
				t2 = eevaluateType(expr->children[1]);

				if (expr -> value.operation == ASSIGN) {
					return t1;
				} else return TYPE_INT;
			default:
				break;
		}
	}
	return TYPE_INT;
}



