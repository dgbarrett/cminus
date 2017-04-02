#include "checking.h"

#include "../errors/ErrorManager.h"
#include "../parse/ast_print.h"
#include "symbtable_print.h"

/* Generally useful functions */
char *				getExpressionString	(ASTNode * expr);
int 				isReturnableFrom	(ASTNode * stmt, ErrorList * errlist);
SymbolDataType 		evaluateType		(ASTNode * expr);
SymbolDataType * 	getArglistSignature	(ASTNode * arglist);
/**/

/* Redefined variable checking */
void 	generateCompleteSymbolListing		(Scope * scope, Scope * parent);
void 	checkAllScopesForRedefinedVariables	(ErrorList * errlist, Scope * scope);
/**/

/* Type checking */
void validateProgram(ASTNode * root, ErrorList * errlist);
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

/*
	Function: semanticAnalysis
		Perform semantic analysis on a program specified by an AST and a 
		SymbolTable
*/
void semanticAnalysis(ASTNode * ast, SymbolTable * symbtable) {
	ErrorList * semanticErrors = new_ErrorList();

	checkAllScopesForRedefinedVariables(semanticErrors, symbtable -> root);
	validateProgram(ast, semanticErrors);

	ErrorList_print(semanticErrors);
}

/*************** TYPE CHECKING ***************/
/*
	Function: validateProgram
		Validates each declaration in the declaration list the makes up a 
		program.
*/
void validateProgram(ASTNode * root, ErrorList * errlist) {
	int i = 0;
	for (i = 0 ; root -> children && root -> children[i] ; i++) {
		validateDeclaration(root -> children[i], errlist);
	}
}

/*
	Function: validateDeclaration
		Validates a single declaration.
		Declarations can either be a variable of function.
*/
void validateDeclaration(ASTNode * declaration, ErrorList * errlist) {
	if (declaration -> type == FUNCTION_DECLARATION) {
		validateFunctionDeclaration(declaration, errlist);
	}
}

/*
	Function: validateFunctionDeclaration
		Semantically validates a funciton declaration, adding any errors to 
		errlist.
*/
void validateFunctionDeclaration(ASTNode * function, ErrorList * errlist) {
	char * functionName = function -> children[1] -> value.str;
	ASTNode * functionBody = function -> children[3];
	SymbolDataType returnType = SymbolDataType_fromString( function -> children[0] -> value.str );

	/* Validate the body of the function (always a compound statement) */
	validateCompoundStatement(functionBody, errlist);

	/* All possible executions paths must lead to a return if function returns
	   an int. */
	if (!isReturnableFrom(functionBody, errlist) && returnType == TYPE_INT) {
		ErrorList_insert(
			errlist, 
			new_Error(
				ErrTemplate_MissingReturnStmt(functionName), 
				function -> linenum, 0
			)
		);
	} 
}

/*
	Function: validateCompoundStatement
		Validate a set of statements in a compound statement.
*/
void validateCompoundStatement(ASTNode * fbody, ErrorList * errlist) {
	int i = 0;
	for (i = 0; fbody -> children && fbody -> children[i] && i < 25 ; i++) {
		validateStatement(fbody -> children[i], errlist);
	}
}

/*
	Function: validateStatement
		Validates a single statement.  
*/
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

/*
	Function: validateIfStatement
		Semantically validates the condition and body of an if statement.
*/
void validateIfStatement(ASTNode * ifstmt, ErrorList * errlist) {
	int i = 0;
	for (i = 0; ifstmt -> children && ifstmt -> children[i] ; i++) {
		if (i == 0) validateCondition(ifstmt -> children[i], errlist);
		else validateStatement(ifstmt -> children[i], errlist);
	}
}

/*
	Function: validateCondition
		Semantically validates a condition for an If statement or while loop.
		Conditions must be of type int.
*/
void validateCondition(ASTNode * condition, ErrorList * errlist) {
	/* Validate if condition is expression */
	if (condition -> type == EXPRESSION) {
		validateExpression(condition, errlist);
	} else if (condition -> type == VAR_ARRAY_ELEMENT) {
		validateArrayElement(condition, errlist);
	}

	/* ERROR CASE: Condition type is not integer. */
	SymbolDataType condType = evaluateType(condition);
	if (condType != TYPE_INT) {
		ErrorList_insert(
			errlist, 
			new_Error(
				ErrTemplate_InvalidConditionType(
					(condition -> parent -> type == IF_STATEMENT) ? "If" : "While loop",
					getExpressionString(condition),
					SymbolDataType_toString(condType)
				), 
				condition -> linenum, 0
			)
		);
	}
}

/*
	Function: validateArrayElement
		Semantically validates the access of an array.
*/
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

		/* ERROR CASE: No array under the name provided */
		if (!arrayDef) {
			ErrorList_insert(
				errlist, 
				new_Error(
					ErrTemplate_UndefinedArray(arrName), 
					arrayElem -> linenum, 0
				)
			);
		} 

		/* ERROR CASE: Symbol exists in Scope, but is not an array */
		if (arrayDef && (arrayDef -> datatype == TYPE_INT || arrayDef -> datatype == TYPE_VOID)) {
			ErrorList_insert(
				errlist, 
				new_Error(
					ErrTemplate_AccessingVarAsArray(arrName), 
					arrayElem -> linenum, 0
				)
			);
		} else if (arrayDef) {
			switch(index -> type) {
				case NUMBER:
					/* ERROR CASE: Accessing array beyond its length */
					if ((arrayDef -> type != SYMBOL_FARRAYPARAM) && arrayDef && (arrayDef -> arrlen <= arrlen)) {
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

					/* ERROR CASE: Symbol used as index is undefined */
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
				case FUNCTION_CALL:
				case VAR_ARRAY_ELEMENT:
					validateExpression(index,errlist);
					break;
				default:;
			}

			/* ERROR CASE: Type of array index is not integer */
			datatype = evaluateType(index);
			if(datatype != TYPE_INT) {
				ErrorList_insert(
					errlist, 
					new_Error(
						ErrTemplate_InvalidArrayAccessType(arrName, getExpressionString(index), SymbolDataType_toString(datatype)), 
						arrayElem -> linenum, 0
					)
				);
			}
		}
	}
}

/*
	Function: validateExpression
		Semantically validate an expression.
*/
void validateExpression(ASTNode * expression, ErrorList * errlist) {
	if (expression) {
		SymbolDataType type1, type2;
		SymbolHashTable * enclosingScope = ASTNode_getEnclosingScope(expression);
		Symbol * id = NULL;

		switch( expression -> type ) {
			case EXPRESSION:
				validateExpression(expression -> children[0], errlist);
				validateExpression(expression -> children[1], errlist);

				type1 = evaluateType(expression -> children[0]);
				type2 = evaluateType(expression -> children[1]);

				/* ERROR CASE: Types on either side of expression do not 
				   match. */
				if (type1 != type2) {
					ErrorList_insert(
						errlist, 
						new_Error(
							ErrTemplate_MismatchedExprType(
								Operator_toString(expression -> value.operation),
							    getExpressionString(expression -> children[0]),
								SymbolDataType_toString(type1),
							 	getExpressionString(expression -> children[1]),
								SymbolDataType_toString(type2)
							), 
							expression -> linenum, 0
						)
					);
				}
				break;
			case IDENTIFIER:
				id = HashTable_get(enclosingScope, expression -> value.str);

				/* ERROR CASE: Given symbol is not defined in the current scope */
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

/*
	Function: validateFunctionCall
		Semantically validates a call to a function.
*/
void validateFunctionCall(ASTNode * fcall, ErrorList * errlist) {
	if (fcall && fcall -> type == FUNCTION_CALL) {
		int i = 0;
		char * functionName = fcall -> children[0] -> value.str;

		ASTNode * arglist = fcall -> children[1];
		SymbolHashTable * enclosingScope = ASTNode_getEnclosingScope(arglist);
		Symbol * function = HashTable_get(enclosingScope, functionName);

		SymbolDataType * expectedSignature = NULL;
		SymbolDataType * actualSignature = calloc(11, sizeof(*actualSignature));

		int actualLen = 0;
		int expectedLen = 0;

		for (i = 0 ; arglist -> children[i] ; i++) {
			validateExpression(arglist -> children[i], errlist);

			actualSignature[i] = evaluateType(arglist -> children[i]);
			actualLen++;
		}

		/* ERROR CASE: Function being called is not defined. */
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

		/* ERROR CASE: Expected length of the function signature does not match
		   the length of the signature from the call. */
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
				/* ERROR CASE: Expected and provided call signatures do not 
				   match */
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
	}
}

/*
	Function: validateReturnStatement
		Semantically validates a return statement, and ensures it is
		returning the correct type.
*/
void validateReturnStatement(ASTNode * retstmt, ErrorList * errlist) {
	ASTNode * function;
	SymbolDataType returnType = TYPE_VOID;
	SymbolDataType expectedReturnType;

	function = ASTNode_getEnclosingFunction(retstmt);
	expectedReturnType = SymbolDataType_fromString(function -> children[0] -> value.str);

	if (retstmt -> children && retstmt -> children[0]) {
		validateExpression(retstmt -> children[0], errlist);
		returnType = evaluateType(retstmt -> children[0]);
	} else {
		returnType = TYPE_VOID;
	}

	/* ERROR CASE: Return statment returing the wrong type. */
	if (expectedReturnType != returnType) {
		ErrorList_insert(
			errlist, 
			new_Error(
				ErrTemplate_MismatchedReturnType(
					function -> children[1] -> value.str, 
					SymbolDataType_toString(expectedReturnType), 
					SymbolDataType_toString(returnType)
				), 
				retstmt -> linenum, 0
			)
		);
	}
}

/*************** CHECKING FOR REDEFINTIONS IN SAME SCOPE ***************/
/*
	Function: checkScopeForRedefinedVariables
		Checks a scope and all its subscopes for redefined variables.
*/
void checkAllScopesForRedefinedVariables(ErrorList * errlist, Scope * scope) {
	int i = 0;
	if (scope) {
		/* Check for redefinitions withing the Scope */
		for (i = 0 ; i < scope -> symbolCount ; i++) {
			/* Attempt to insert all symbols declared in this scope. 
			   If we cannot add a symbol, a name conflict exists */
			if(!HashTable_insert(scope -> allsymbols, scope -> symbols[i])) {
				Symbol * new = scope -> symbols[i];
				Symbol * old = HashTable_get(scope -> allsymbols, scope -> symbols[i] -> name);


				char * errMsg = (old -> linenum == 0) 
								? 
								ErrTemplate_RedefinedStdlibSymbol(new -> name) 
								: 
								ErrTemplate_RedefinedSymbol(new -> name, old -> linenum); 
				
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

		/* Generate complete list of symbols accessible from the Scope. */
		generateCompleteSymbolListing(scope, scope -> parent);	

		/* Check the sub-Scopes of the Scope for redefined variables */
		for (i=0 ; i<scope->subscopeCount ; i++) {
			checkAllScopesForRedefinedVariables(errlist, scope -> subscopes[i]);
		}
	}
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



/*************** GENERALLY USEFUL FUNCTIONS ***************/
/* 
	Function: getExpressionString
		Returns the string representation an expression.
*/
char * getExpressionString(ASTNode * subexpr) {
	char * name = calloc(64, sizeof(*name));
	char * num = NULL;
	char * n1, *n2, *op;
	SymbolHashTable * enclosingScope = ASTNode_getEnclosingScope(subexpr);
	Symbol * fcall = NULL;
	SymbolDataType * signature = NULL;

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
			fcall = HashTable_get(enclosingScope, subexpr -> children[0] -> value.str);

			if (!fcall) {
				signature = getArglistSignature(subexpr -> children[1]);
			} else {
				signature = fcall -> signature;
			}

			sprintf(name, "%s%s", subexpr -> children[0] -> value.str, Symbol_callSignatureToString(signature));
			return name;
		case EXPRESSION:
			n1 = getExpressionString(subexpr -> children[0]);
			n2 = getExpressionString(subexpr -> children[1]);
			op = Operator_toString(subexpr -> value.operation);
			sprintf(name, "%s %s %s", n1, op, n2);
			return name;
		case VAR_ARRAY_ELEMENT:
			num = calloc(16, sizeof(*num));
			strcpy(name, subexpr -> children[0] -> value.str);
			sprintf(num, "[%s]", getExpressionString(subexpr -> children[1]));
			strcat(name, num);
			return name;
		default:
			free(name);
			return "??NoName??";
	}
}

/*
	Function: evaluateType
		Evaluates and returns the type of an expression.
*/
SymbolDataType evaluateType( ASTNode * expr) {
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
				return evaluateType(expr -> children[0]);
			case VAR_ARRAY_ELEMENT:
				/* evaluate the type of the element access */
				t1 = evaluateType(expr -> children[1]);
				temp = HashTable_get(enclosingScope, expr -> children[0] -> value.str);

				if (temp) {
					return SymbolDataType_parentType(temp -> datatype);
				} else return TYPE_INT;
			case EXPRESSION:
				t1 = evaluateType(expr->children[0]);
				t2 = evaluateType(expr->children[1]);

				if (expr -> value.operation == ASSIGN) {
					return t1;
				} else return TYPE_INT;
			default:
				break;
		}
	}
	return TYPE_INT;
}

/*
	Function: getArglistSignature
		Returns the signature of the provided ARGUMENT_LIST.
*/
SymbolDataType * getArglistSignature(ASTNode * arglist) {
	SymbolDataType * sig = calloc(11, sizeof(*sig));
	if (arglist && arglist -> type == ARGUMENT_LIST) {
		int i = 0;
		for (i=0 ; i<25 && arglist->children[i] ; i++) {
			sig[i] = evaluateType(arglist->children[i]);
		}	
	}
	return sig;
}

/*
	Function: isReturableFrom
		Returns true if all possible paths in a statement reach a return 
		statement.
*/
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




