%{

#include <stdio.h>
#include "lex.h"
#include "parse.h"

SyntaxTree * tree;

void yyerror(const char *str) {
        fprintf(stderr,"error: %s\n",str);
}

static int yylex(void){ 
	return getToken(); 
}

SyntaxTree * parse(void) { 
  yyparse();
  return tree;
}

void initParser() {
	tree = new_SyntaxTree();
}

%}

%token ID_TOK NUM_TOK
%token ASSIGN_TOK ENDSTMT_TOK COMMA_TOK LBRACE_TOK RBRACE_TOK LBRACKET_TOK RBRACKET_TOK LCURL_TOK RCURL_TOK
%token OPENCMT_TOK CLOSECMT_TOK WHITESPACE_TOK
%token LE_TOK LT_TOK GE_TOK GT_TOK EQ_TOK NOTEQ_TOK 
%token MINUS_TOK PLUS_TOK DIV_TOK MUL_TOK
%token INT_TOK VOID_TOK IF_TOK ELSE_TOK WHILE_TOK RETURN_TOK
%token ERROR_TOK END

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

/* C- Grammar */

program : declaration_list;

declaration_list : declaration_list declaration | declaration;
declaration : var_declaration | fun_declaration;
var_declaration: type_specifier ID_TOK ENDSTMT_TOK | type_specifier ID_TOK NUM_TOK ENDSTMT_TOK;
type_specifier: INT_TOK | VOID_TOK;
fun_declaration: type_specifier ID_TOK LBRACE_TOK params RBRACE_TOK compound_statement;
params : param_list | VOID_TOK;
param_list : param_list COMMA_TOK param | param
param : type_specifier ID_TOK | type_specifier ID_TOK LBRACKET_TOK RBRACKET_TOK
compound_statement : LCURL_TOK local_declarations statement_list RCURL_TOK
local_declarations : local_declarations var_declaration | ;
statement_list : statement_list statement | ;
statement : expression_statement | compound_statement | selection_statement | iteration_statement | return_statement;
expression_statement : expression ENDSTMT_TOK | ENDSTMT_TOK;
selection_statement : IF_TOK LBRACE_TOK expression RBRACE_TOK statement %prec LOWER_THAN_ELSE|
					  IF_TOK LBRACE_TOK expression RBRACE_TOK statement ELSE_TOK statement;
iteration_statement : WHILE_TOK LBRACE_TOK expression RBRACE_TOK statement;
return_statement : RETURN_TOK ENDSTMT_TOK | RETURN_TOK expression ENDSTMT_TOK;
expression : var ASSIGN_TOK expression | simple_expression;
var : ID_TOK | ID_TOK LBRACKET_TOK expression RBRACKET_TOK;
simple_expression : additive_expression relop additive_expression | additive_expression;
relop : GT_TOK | GE_TOK | LT_TOK | LE_TOK | EQ_TOK | NOTEQ_TOK;
additive_expression : additive_expression addop term | term;
addop : PLUS_TOK | MINUS_TOK;
term : term mulop factor | factor;
mulop : MUL_TOK | DIV_TOK;
factor : LBRACE_TOK
expression RBRACE_TOK | var | call | NUM_TOK;
call : ID_TOK LBRACE_TOK args RBRACE_TOK
args : arg_list | ;
arg_list : arg_list COMMA_TOK expression | expression;
