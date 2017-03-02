void yyerror(const char *str) {
        fprintf(stderr,"error: %s\n",str);
}

static int yylex(void){ 
	return getToken(); 
}

%token ID_TOK NUM_TOK
%token ENDSTMT_TOK COMMA_TOK LBRACE_TOK RBRACE_TOK LBRACKET_TOK RBRACKET_TOK
%token INT_TOK

%%

/* C- Grammar */

program : declaration_list;

declaration_list : declaration_list declaration | declaration;
declaration : var_declaration | fun_declaration;
var_declaration: type_specifier ID_TOK ENDSTMT_TOK | type_specifier ID_TOK NUM_TOK ENDSTMT_TOK;
type_specifier: INT_TOK | VOID_TOK;
fun_declaration: type_specifier ID_TOK LBRACE_TOK params RBRACE_TOK compount_stmt;
params : param_list | VOID_TOK;
param_list : param_list COMMA_TOK param | param
param : type_specifier ID_TOK | type_specifier ID_TOK LBRACKET_TOK RBRACKET_TOK
compound_stmt : LCURL_TOK local_declarations statement_list RCURL_TOK
local_declarations : local_declarations var_declaration | ;
statement_list : statment_list statement | ;
statement : expression_statment | compound_statement | selection_statement | iteration_statement | return_statement;
expression_statement : expression ENDSTMT_TOK | ENDSTMT_TOK;
selection_statement : IF_TOK LBRACE_TOK expression RBRACE_TOK statement |
					  IF_TOK LBRACE_TOK expression RBRACE_TOK statement ELSE_TOK statement;
iteration_statement : WHILE_TOK LBRACE_TOK expression RBRACE_TOK statement;
return_statement : RETURN_TOK ENDSTMT | RETURN_TOK expression ENDSTMT;
expression : var ASSIGN_TOK expression | simple_expression;
var : ID_TOK | ID_TOK LBRACKET_TOK expression RBRACKET_TOK;
simple_expression : additive_expression relop additivie_expression | additive_expression;
relop : GT_TOK | GE_TOK | LT_TOK | LE_TOK | EQ_TOK | NOTEQ_TOK;
additive_expression : additive_expression addop term | term;
addop : PLUS_TOK | MINUS_TOK;
term : term mulop factor | factor;
mulop : MUL_TOK | DIV_TOK;
factor : LBRACE expression RBRACE | var | call | NUM_TOK;
call : ID_TOK LBRACE_TOK args RBRACE_TOK
args : arg_list | ;
arg_list : arg_list COMMA_TOK expression | expression;
