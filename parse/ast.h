#ifndef __SYNATX_TREE_H__
#define __SYNATX_TREE_H__

#include <stdlib.h>
#include <string.h>

#include "../scan/tokens.h"

#define MAX_CHILDREN 5

typedef enum _ASTNodeType {
	PROGRAM,
	VAR_DECLARATION,
	VAR_ARRAY_DECLARATION,
	IDENTIFIER,
	NUMBER,
	FUNCTION,
	PARAMETER_LIST, 
	TYPE,
	PARAMETER,
	ARRAY_PARAMETER,
	LOCAL_VARS,
	STATEMENT_LIST,
	COMPOUND_STATEMENT,
	EXPRESSION,
	IF_STATEMENT,
	WHILE_LOOP,
	RETURN_STATEMENT, 
	_OPERATION, 
	VARIABLE,
	VAR_ARRAY_ELEMENT,
	FUNCTION_CALL,
	ARGUMENT_LIST
} ASTNodeType;

typedef union _ASTNodeValue {
	char * str;
	int num;
	Operator operation;
} ASTNodeValue;

typedef struct _ASTNode {
	ASTNodeType type;
	ASTNodeValue value;
	struct _ASTNode ** children;
} ASTNode;

ASTNode * new_ASTNode( ASTNodeType ntype );
void printSyntaxTree(ASTNode * root, int depth);
void printNodeType(ASTNode * node);

ASTNode * Program( ASTNode * program );
ASTNode * Variable();
ASTNode * VariableDeclaration();
ASTNode * VariableArray();
ASTNode * VariableArrayDeclaration();
ASTNode * Identifier( char * strid );
ASTNode * Number( char * strnum ) ;
ASTNode * Type( char * strtype ) ;
ASTNode * Function();
ASTNode * Parameter() ;
ASTNode * ParameterList();
ASTNode * ArrayParameter() ;
ASTNode * LocalVariables() ;
ASTNode * StatementList() ;
ASTNode * CompoundStatement();
ASTNode * Expression() ;
ASTNode * IfStatement();
ASTNode * WhileLoop();
ASTNode * ReturnStatement();
ASTNode * VariableArrayElement();
ASTNode * FunctionCall() ;
ASTNode * ArgumentList() ;
ASTNode * Operation( char * strop );
ASTNode * Operator_Plus() ;
ASTNode * Operator_Minus() ;
ASTNode * Operator_Div() ;
ASTNode * Operator_Muls();
ASTNode * Operator_Greater();
ASTNode * Operator_GreaterEqual() ;
ASTNode * Operator_Less() ;
ASTNode * Operator_LessEqual() ;
ASTNode * Operator_Assignment() ;
ASTNode * Operator_Equal() ;
ASTNode * Operator_NotEqual() ;

void Program_appendDeclaration( ASTNode * program, ASTNode * declaration);
void Variable_setType( ASTNode * variable, ASTNode * type) ;
void Variable_setIdentifier( ASTNode * variable, ASTNode * id) ;
void VariableArray_setType( ASTNode * variablearr, ASTNode * type) ;
void VariableArray_setIdentifier( ASTNode * variablearr, ASTNode * id) ;
void VariableArray_setSize( ASTNode * variablearr, ASTNode * type) ;
void VariableDeclaration_setType( ASTNode * variable, ASTNode * type) ;
void VariableDeclaration_setIdentifier( ASTNode * variable, ASTNode * id) ;
void VariableArrayDeclaration_setType( ASTNode * variablearr, ASTNode * type) ;
void VariableArrayDeclaration_setIdentifier( ASTNode * variablearr, ASTNode * id) ;
void VariableArrayDeclaration_setSize( ASTNode * variablearr, ASTNode * type) ;
void Function_setReturnType( ASTNode * function, ASTNode * rettype);
void Function_setIdentifier( ASTNode * function, ASTNode * id);
void Function_setParameters( ASTNode * function, ASTNode * params) ;
void Function_setDefinition( ASTNode * function, ASTNode * def);
void ParameterList_append( ASTNode * paramlist, ASTNode * param) ;
void Parameter_setType( ASTNode * param, ASTNode * type);
void Parameter_setIdentifier( ASTNode * param, ASTNode * id) ;
void LocalVariables_append( ASTNode * vars, ASTNode * var) ;
void StatementList_append( ASTNode * stmtlist, ASTNode * stmt) ;
void CompoundStatement_setLocalVars( ASTNode * stmt, ASTNode * vars);
void CompoundStatement_setStatementList( ASTNode * stmt, ASTNode * stmtlist) ;
void IfStatement_setCondition( ASTNode * ifstmt, ASTNode * condition);
void IfStatement_setBody( ASTNode * ifstmt, ASTNode * body) ;
void IfStatement_setElseBody( ASTNode * ifstmt, ASTNode * elsebody);
void WhileLoop_setCondition( ASTNode * whilestmt, ASTNode * condition ) ;
void WhileLoop_setBody( ASTNode * whilestmt, ASTNode * body ) ;
void ReturnStatement_setReturnValue( ASTNode * returnstmt, ASTNode * returnvalue);
void Expression_setType( ASTNode * expression, ASTNode * exptype) ;
void Expression_setVariable( ASTNode * expression, ASTNode * variable) ;
void Expression_setValue( ASTNode * expression, ASTNode * value) ;
void Expression_setSubExpressions( ASTNode * expression, ASTNode * subexpr1, ASTNode * subexpr2);
void VariableArrayElement_setParentArray( ASTNode * arrayelem, ASTNode * parent) ;
void VariableArrayElement_setIndex( ASTNode * arrayelem, ASTNode * index);
void FunctionCall_functionCalled( ASTNode * function, ASTNode * name);
void FunctionCall_arguments(ASTNode * function, ASTNode * args) ;
void ArgumentList_append(ASTNode * arglist, ASTNode * arg);
void ArrayParameter_setType( ASTNode * arrparam, ASTNode * type);
void ArrayParameter_setIdentifier( ASTNode * arrparam, ASTNode * id);

#endif