%{
	#include "parse/ast.h"
	#include "parse/parse.h"
	#include "scan/lex.h"
	#define YYSTYPE ASTNode *

	static ASTNode * node;
	extern int yychar;

	char tokenString[50];

	int yyparse(void);

	void yyerror(const char *str) {
	    fprintf(stderr,"error: %s\n",str);
	    fprintf(stderr,"Current token: ");
	  	printToken(yychar);

	  	printf("\n\n");
	}

	static int yylex(void){ 
		Token * tok = getToken();

		int tok_type = Token_getType(tok);
		strcpy(tokenString, Token_getValue(tok));

		/*
		printf("%s\n", tokenString);
		*/

		destroy_Token(tok);

		return tok_type;
	}

	ASTNode * parse(void) { 
	  yyparse();
	  return node;
	}

	/*
	extern int yydebug;
	int yydebug = 1;
	#define YYDEBUG 1
	*/
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
				;

id : ID_TOK 
		{
			$$ = Identifier( tokenString );
		}
		;

num : NUM_TOK
		{
			$$ = Number( tokenString );
		}
		;

type_specifier : INT_TOK 
					{
						$$ = Type("int");
					}
			   | VOID_TOK 
			   		{
			   			$$ = Type("void");
			   		}
			   ;

fun_declaration : type_specifier id LBRACE_TOK params RBRACE_TOK compound_statement
					{
						$$ = Function();
						Function_setReturnType( $$, $1 );
						Function_setIdentifier( $$, $2 );
						Function_setParameters( $$, $4 );
						Function_setDefinition( $$, $6 );
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
				   			$$ = LocalVariables( NULL );
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
		  ;

expression_statement : expression ENDSTMT_TOK 
					  	{
					  		$$ = $1;
					  	}
					 | ENDSTMT_TOK
					 	{
					 		$$ = Expression();
					 	}
					 		;

compound_statement : LCURL_TOK local_declarations statement_list RCURL_TOK
				   		{
				   			$$ = CompoundStatement();
				   			CompoundStatement_setLocalVars( $$, $2 );
				   			CompoundStatement_setStatementList( $$, $3 );
				   		}
				   ;

selection_statement : IF_TOK LBRACE_TOK expression RBRACE_TOK statement
						{
							$$ = IfStatement();
							IfStatement_setCondition( $$, $3 );
							IfStatement_setBody( $$, $5 );
						} 
					| IF_TOK LBRACE_TOK expression RBRACE_TOK statement ELSE_TOK statement
						{
							$$ = IfStatement();
							IfStatement_setCondition( $$, $3 );
							IfStatement_setBody( $$, $5 );
							IfStatement_setElseBody( $$, $7 );
						}
					;

iteration_statement : WHILE_TOK LBRACE_TOK expression RBRACE_TOK statement
					 	{
					 		$$ = WhileLoop();
					 		WhileLoop_setCondition( $$, $3 );
					 		WhileLoop_setBody( $$, $5 );
					 	}
					;

return_statement : RETURN_TOK ENDSTMT_TOK 
				 	{
				 		$$ = ReturnStatement();
				 	}
				 | RETURN_TOK expression ENDSTMT_TOK
				 	{
				 		$$ = ReturnStatement();
				 		ReturnStatement_setReturnValue( $$, $2 );
				 	}
				 ;

expression : var assign expression
				{
					$$ = Expression();
					Expression_setType( $$, $2 );
					Expression_setVariable( $$, $1 );
					Expression_setValue( $$, $3 );
				} 
		   | simple_expression 
		   		{
		   			$$ = $1;
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
			VariableArrayElement_setParentArray( $$, $1 );
			VariableArrayElement_setIndex( $$, $3 );
		};

simple_expression : additive_expression relopp additive_expression
				  	{
				  		$$ = Expression();
				  		/* $2 gets reduced to a value stored in the Expression */
				  		Expression_setType( $$, $2 );
				  		Expression_setSubExpressions( $$, $1, $3 );
				  	} 
				  | additive_expression
				  	{
				  		$$ = $1	;
				  	};

relopp : relop
	   		{
	   			$$ = $1;
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
			} 
	  | MINUS_TOK
	  		{
				$$ = Operation( tokenString );
			} 
	  ;

term : term mulop factor 
		{
			$$ = Expression();
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
		} 
	  | DIV_TOK
	  	{
			$$ = Operation( tokenString );
		} 

	  ;

factor : LBRACE_TOK expression RBRACE_TOK 
			{
				$$ = $1;
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
	 	}
	 ;

arg_list : arg_list COMMA_TOK expression 
			{
				$$ = ArgumentList( $1 );
				ArgumentList_append( $$, $3 );
			} 
		 | expression
		 	{
		 		$$ = ArgumentList( NULL );
		 		ArgumentList_append( $$, $1 );
		 	};
		 ;

