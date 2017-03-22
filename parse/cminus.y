%{
	#include "parse/ast.h"
	#include "parse/parse.h"
	#include "scan/lex.h"
	#include "errors/ErrorManager.h"
	#define YYSTYPE ASTNode *

	static ASTNode * node;
	extern FILE * yyin;
	extern YYSTYPE yylval;
	extern char* yytext;

	char tokenString[50];
	int linenum;
	ErrorList * errs = NULL;

	int yyparse(void);

	void yyerror(const char *str) {
		char buf[128];
		sprintf(buf, "Unexpected token '%s' found.", yytext);
		ErrorList_insert( errs, new_Error(buf, linenum, 1) );
	}

	static int yylex(void){ 
		Token * tok = getToken();

		int tok_type = Token_getType(tok);
		strcpy(tokenString, Token_getValue(tok));
		linenum = tok -> linenum;

		destroy_Token(tok);

		return tok_type;
	}

	ASTNode * parse(FILE * fin) {
		yyin = fin; 
		errs = new_ErrorList();
	 	yyparse();

	 	if (!ErrorList_isEmpty(errs)) {
	 		ErrorList_print(errs);
	 	}

	  	return node;
	}
%}

%token ID_TOK NUM_TOK
%token ASSIGN_TOK ENDSTMT_TOK COMMA_TOK LBRACE_TOK RBRACE_TOK LBRACKET_TOK RBRACKET_TOK LCURL_TOK RCURL_TOK
%token OPENCMT_TOK CLOSECMT_TOK WHITESPACE_TOK
%token LE_TOK LT_TOK GE_TOK GT_TOK EQ_TOK NOTEQ_TOK 
%token MINUS_TOK PLUS_TOK DIV_TOK MUL_TOK
%token INT_TOK VOID_TOK IF_TOK ELSE_TOK WHILE_TOK RETURN_TOK
%token ERROR_TOK 


%%
/* C- Grammar */

program : declaration_list 
			{ 
				node = $1; 
			}
		;

declaration_list : declaration_list declaration 
					{
						$$ = Program( $1 );
						Program_appendDeclaration( $$, $2 );
					}
				 | declaration 
				 	{ 
				 		$$ = Program( NULL );
				 		Program_appendDeclaration( $$, $1 ); 
				 	}
				 ;

declaration : var_declaration { $$ = $1; }
			| fun_declaration { $$ = $1; }
			;

var_declaration : type_specifier id ENDSTMT_TOK
					{
						$$ = VariableDeclaration();
						VariableDeclaration_setType( $$, $1 );
						VariableDeclaration_setIdentifier( $$, $2 );
					}
				| type_specifier id LBRACKET_TOK num RBRACKET_TOK ENDSTMT_TOK
					{
						$$ = VariableArrayDeclaration();
						VariableArrayDeclaration_setType( $$, $1 );
						VariableArrayDeclaration_setIdentifier( $$, $2 );
						VariableArrayDeclaration_setSize( $$, $4 );
					}
				| type_specifier id LBRACKET_TOK error RBRACKET_TOK ENDSTMT_TOK 
					{
						$$ = VariableArrayDeclaration();
						VariableArrayDeclaration_setType( $$, $1 );
						VariableArrayDeclaration_setIdentifier( $$, $2 );
						VariableArrayDeclaration_setSize( $$, NULL );
						ErrorList_insert(errs, new_Error("Array sizes must be numbers.", ASTNode_getLineNum($1), 0));
					}
				;

id : ID_TOK 
		{
			$$ = Identifier( tokenString );
			ASTNode_setLineNum($$, linenum);
		}
		;

num : NUM_TOK
		{
			$$ = Number( tokenString );
			ASTNode_setLineNum($$, linenum);
		}
		;

type_specifier : INT_TOK 
					{
						$$ = Type("int");
						ASTNode_setLineNum($$, linenum);
					}
			   | VOID_TOK 
			   		{
			   			$$ = Type("void");
			   			ASTNode_setLineNum($$, linenum);
			   		}
			   ;

fun_declaration : type_specifier id LBRACE_TOK params RBRACE_TOK compound_statement
					{
						$$ = Function();

						Function_setReturnType( $$, $1 );
						Function_setIdentifier( $$, $2 );
						Function_setParameters( $$, $4 );
						Function_setDefinition( $$, $6 );
						ASTNode_setLineNum($$, $2 -> linenum);
					}
				| type_specifier id LBRACE_TOK RBRACE_TOK compound_statement
					{
						$$ = Function();
						Function_setReturnType( $$, $1 );
						Function_setIdentifier( $$, $2 );
						Function_setParameters( $$, new_ASTNode(SYNTAX_ERROR) );
						Function_setDefinition( $$, $5);
						ErrorList_insert(errs, new_Error("Function parameters are empty (use 'void' instead).", ASTNode_getLineNum($1),0));
						ASTNode_setLineNum($$, $2 -> linenum);
					}
				;

params : param_list
			{
				$$ = $1;
			} 
	   | VOID_TOK
	   		{
	   			$$ = ParameterList( NULL );
	   		}
	   	;

param_list : param_list COMMA_TOK param 
				{
					$$ = ParameterList( $1 );
					ParameterList_append( $$, $3 );
				}
		   | param 
		   		{
		   			$$ = ParameterList( NULL );
		   			ParameterList_append( $$, $1 );
		   		}
		   	;

param : type_specifier id
			{
				$$ = Parameter();
				Parameter_setType( $$, $1 );
				Parameter_setIdentifier( $$, $2 );
			}
      | type_specifier id LBRACKET_TOK RBRACKET_TOK
      		{
      			$$ = ArrayParameter();
      			ArrayParameter_setType( $$, $1 );
				ArrayParameter_setIdentifier( $$, $2 );
      		}
      ;


local_declarations : local_declarations var_declaration
						{

							$$ = LocalVariables( $1 );
							LocalVariables_append( $$, $2 );
						} 
				   | 
				   		{
				   			$$ = NULL;
				   		}
				   	;

statement_list : statement_list statement  
					{
						$$ = StatementList( $1 );
						StatementList_append( $$, $2 );
					}
			   | 
			   		{
			   			$$ = StatementList( NULL );
			   		}
			   	;

statement : expression_statement 
				{
					$$ = $1;
				}
		  | compound_statement 
		  		{
		  			$$ = $1;
		  		}
		  | selection_statement 
		  		{
		  			$$ = $1;
		  		}
		  | iteration_statement 
		  		{
		  			$$ = $1;
		  		}
		  | return_statement
		  		{
		  			$$ = $1;
		  		}
		  | error
		  		{
		  			$$ = new_ASTNode(SYNTAX_ERROR);
		  			ErrorList_insert(errs, new_Error("Expected statement.", linenum,0 ));
		  		}
		  ;

expression_statement : expression ENDSTMT_TOK 
					  	{
					  		$$ = $1;
					  	}
					 | ENDSTMT_TOK
					 	{
					 		$$ = Expression();
					 	}
					 | expression error
					 	{
					 		$$ = $1;
					 		ErrorList_insert(errs, new_Error("Statement terminator is missing (';').", ASTNode_getLineNum($1), 0));
					 	}
					 ;

compound_statement : lcurl local_declarations statement_list RCURL_TOK
				   		{
				   			$$ = CompoundStatement();
				   			ASTNode_setLineNum( $$, ASTNode_getLineNum($1) );
				   			CompoundStatement_setLocalVars( $$, $2 );
				   			CompoundStatement_setStatements( $$, $3 );
				   		}
				   	;

lcurl : LCURL_TOK
			{
				$$ = new_ASTNode(_TOKEN);
				ASTNode_setLineNum($$, linenum);
			}


selection_statement : if_t LBRACE_TOK expression RBRACE_TOK statement
						{
							$$ = IfStatement();
							IfStatement_setCondition( $$, $3 );
							IfStatement_setBody( $$, $5 );
						} 
					| if_t LBRACE_TOK expression RBRACE_TOK statement ELSE_TOK statement
						{
							$$ = IfStatement();
							IfStatement_setCondition( $$, $3 );
							IfStatement_setBody( $$, $5 );
							IfStatement_setElseBody( $$, $7 );
						}
					| if_t LBRACE_TOK RBRACE_TOK statement ELSE_TOK statement
						{
							$$ = IfStatement();
							IfStatement_setCondition( $$, new_ASTNode(SYNTAX_ERROR));
							IfStatement_setBody( $$, $4 );
							IfStatement_setElseBody( $$, $6 );
							ErrorList_insert(errs, new_Error("Empty if condition.", ASTNode_getLineNum($1),0));
						}
					| if_t LBRACE_TOK RBRACE_TOK statement 
						{
							$$ = IfStatement();
							IfStatement_setCondition( $$, new_ASTNode(SYNTAX_ERROR));
							IfStatement_setBody( $$, $4 );
							ErrorList_insert(errs, new_Error("Empty if condition.", ASTNode_getLineNum($1),0));
						}
					;

if_t : IF_TOK
		{
			$$ = new_ASTNode(_TOKEN);
			ASTNode_setLineNum($$,linenum);
		}

iteration_statement : while_t LBRACE_TOK expression RBRACE_TOK statement
					 	{
					 		$$ = WhileLoop();
					 		WhileLoop_setCondition( $$, $3 );
					 		WhileLoop_setBody( $$, $5 );
					 	}
					 | while_t LBRACE_TOK RBRACE_TOK statement 
					 	{
					 		$$ = WhileLoop();
					 		WhileLoop_setCondition( $$, new_ASTNode(SYNTAX_ERROR) );
					 		WhileLoop_setBody( $$, $4 );
					 		ErrorList_insert(errs, new_Error("Empty loop condition.", ASTNode_getLineNum($1),0));
					 	}
					;
while_t : WHILE_TOK
			{
				$$ = new_ASTNode(_TOKEN);
				ASTNode_setLineNum($$,linenum);
			}

return_statement : return_t ENDSTMT_TOK 
				 	{
				 		$$ = ReturnStatement();
				 		ASTNode_setLineNum($$,linenum);
				 	}
				 | return_t expression ENDSTMT_TOK
				 	{
				 		$$ = ReturnStatement();
				 		ReturnStatement_setReturnValue( $$, $2 );
				 		ASTNode_setLineNum($$, linenum);
				 	}
				 | return_t expression error 
				 	{
				 		$$ = ReturnStatement();
				 		ReturnStatement_setReturnValue( $$, $2 );
				 		ErrorList_insert(errs, new_Error("Statement terminator is missing (';').", ASTNode_getLineNum($1), 0));
				 	}
				 | return_t error 
				 	{
				 		$$ = ReturnStatement();
				 		ErrorList_insert(errs, new_Error("Statement terminator is missing (';').", ASTNode_getLineNum($1), 0));
				 	}
				 ;

return_t : RETURN_TOK 
			{
				$$ = new_ASTNode(_TOKEN);
				ASTNode_setLineNum($$,linenum);
			}

expression : var assign expression
				{
					$$ = Expression();
					ASTNode_setLineNum( $$, ASTNode_getLineNum($1) );
					Expression_setType( $$, $2 );
					Expression_setVariable( $$, $1 );
					Expression_setValue( $$, $3 );
				} 
		   | simple_expression 
		   		{
		   			$$ = $1;
		   		}
		   	|var assign error
		   		{
		   			$$ = Expression();
		   			ASTNode_setLineNum( $$, ASTNode_getLineNum($1) );
		   			Expression_setType($$, $2);
		   			ErrorList_insert(errs, new_Error("Invalid assignment.", ASTNode_getLineNum($1), 0));
		   		}
		   ;

assign : ASSIGN_TOK 
			{
				$$ = Operation( tokenString );
			}

var : id 
		{
			$$ = $1;
		}
	| id LBRACKET_TOK expression RBRACKET_TOK
		{
			$$ = VariableArrayElement();
			ASTNode_setLineNum($$, ASTNode_getLineNum($1));
			VariableArrayElement_setParentArray( $$, $1 );
			VariableArrayElement_setIndex( $$, $3 );
		};
	| id LBRACKET_TOK RBRACKET_TOK
		{
			$$=VariableArrayElement();

			char buf[128];
			ErrorList_insert(errs, new_Error("Cannot access array without index.", ASTNode_getLineNum($1), 0));
		}

simple_expression : additive_expression relop additive_expression
				  	{
				  		$$ = Expression();
				  		ASTNode_setLineNum($$, ASTNode_getLineNum($1));
				  		/* $2 gets reduced to a value stored in the Expression */
				  		Expression_setType( $$, $2 );
				  		Expression_setSubExpressions( $$, $1, $3 );
				  	} 
				  | additive_expression
				  	{
				  		$$ = $1	;
				  	}
				  ;


relop : GT_TOK 
			{
				$$ = Operation( tokenString );
			}
	  | GE_TOK 
	  		{
				$$ = Operation( tokenString );
			}
	  | LT_TOK
	  		{
				$$ = Operation( tokenString );
			} 
	  | LE_TOK 
	  		{
				$$ = Operation( tokenString );
			} 
	  | EQ_TOK 
	  		{
				$$ = Operation( tokenString );
			} 
	  | NOTEQ_TOK
	  		{
				$$ = Operation( tokenString );
			} 
	  ;

additive_expression : additive_expression addop term 
						{
							$$ = Expression();
							ASTNode_setLineNum( $$, ASTNode_getLineNum($2) );
							Expression_setType( $$, $2 );
							Expression_setSubExpressions($$, $1, $3);
						}
				    | term 
				    	{
				    		$$ = $1;
				    	}
				    ;

addop : PLUS_TOK
			{
				$$ = Operation( tokenString );
				ASTNode_setLineNum($$, linenum);
			} 
	  | MINUS_TOK
	  		{
				$$ = Operation( tokenString );
				ASTNode_setLineNum($$, linenum);
			} 
	  ;

term : term mulop factor 
		{
			$$ = Expression();
			ASTNode_setLineNum($$,ASTNode_getLineNum($1));
			Expression_setType( $$, $2 );
			Expression_setSubExpressions($$, $1, $3);
		}
	 | factor 
	 	{
	 		$$ = $1;
	 	}
	 ;

mulop : MUL_TOK
		{
			$$ = Operation( tokenString );
			ASTNode_setLineNum($$, linenum);
		} 
	  | DIV_TOK
	  	{
			$$ = Operation( tokenString );
			ASTNode_setLineNum($$, linenum);
		} 

	  ;

factor : LBRACE_TOK expression RBRACE_TOK 
			{
				$$ = $2;
			}
		| var 
			{
				$$ = $1;
			}
		| call 
			{
				$$ = $1;
			}
		| num
			{
				$$ = $1;
			}
		;

call : id LBRACE_TOK args RBRACE_TOK
	 	{
	 		$$ = FunctionCall();
	 		ASTNode_setLineNum($$, ASTNode_getLineNum($1));
	 		FunctionCall_functionCalled( $$, $1 );
	 		FunctionCall_arguments( $$, $3 );
	 	}
	 ;

args : arg_list
		{
			$$ = $1;
		} 
	 | 
	 	{
	 		$$ = ArgumentList( NULL );
	 		ASTNode_setLineNum( $$, linenum );
	 	}
	 ;

arg_list : arg_list COMMA_TOK expression 
			{
				$$ = ArgumentList( $1 );
				ASTNode_setLineNum( $$, ASTNode_getLineNum($1) );
				ArgumentList_append( $$, $3 );
			} 
		 | expression
		 	{
		 		$$ = ArgumentList( NULL );
		 		ASTNode_setLineNum( $$, ASTNode_getLineNum($1) );
		 		ArgumentList_append( $$, $1 );
		 	};
		 ;

