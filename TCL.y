%{
#include "TCL.h"
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern TCLFormula* g_parse_root;



void yyerror(const char *s) {
	cout << "EEK, parse error!  Message: " << s << endl;
	// might as well halt now:
	exit(-1);
}
%}

%union {
	TCLFormula* ptr;
}

%token <ptr> TOKEN_ROOT
%token <ptr> TOKEN_PLUS
%token <ptr> TOKEN_MINUS
%token <ptr> TOKEN_AND
%token <ptr> TOKEN_OR
%token <ptr> TOKEN_UNTIL
%token <ptr> TOKEN_WNTIL
%token <ptr> TOKEN_NEXT
%token <ptr> TOKEN_TRUE
%token <ptr> TOKEN_FALSE
%token <ptr> TOKEN_ATOMIC
%token <ptr> TOKEN_NOT
%token TOKEN_RIGHT
%token TOKEN_LEFT
%type <ptr> state_formula
%type <ptr> expression
%%

state_formula: TOKEN_ROOT TOKEN_LEFT expression TOKEN_RIGHT{
								$1->type=PARSE_ROOT;
								$1->outs.push_back($3);
								$3->ins.push_back($1);
								g_parse_root=$1;
							}
		;



    
expression: TOKEN_PLUS TOKEN_LEFT expression TOKEN_RIGHT{
							$1->type=PLUS;
							$1->outs.push_back($3);
							$3->ins.push_back($1);
							$$=$1;
						} 
						|TOKEN_MINUS TOKEN_LEFT expression TOKEN_RIGHT{
							$1->type=MINUS;
							$1->outs.push_back($3);
							$3->ins.push_back($1);
							$$=$1;
						} 
    | TOKEN_LEFT expression TOKEN_RIGHT{$$=$2;}
    | TOKEN_LEFT expression TOKEN_RIGHT TOKEN_AND TOKEN_LEFT expression TOKEN_RIGHT {
    	$4->type=AND;
    	$4->outs.push_back($2);
    	$4->outs.push_back($6);
    	$2->ins.push_back($4);
    	$6->ins.push_back($4);
    	$$=$4;
    }
    | TOKEN_LEFT expression TOKEN_RIGHT TOKEN_OR TOKEN_LEFT expression TOKEN_RIGHT {
    	$4->type=OR;
    	$4->outs.push_back($2);
    	$4->outs.push_back($6);
    	$2->ins.push_back($4);
    	$6->ins.push_back($4);
    	$$=$4;
    }
    | TOKEN_LEFT expression TOKEN_RIGHT TOKEN_UNTIL TOKEN_LEFT expression TOKEN_RIGHT 
    {
    	$4->type=UNTIL;
    	$4->outs.push_back($2);
    	$4->outs.push_back($6);
    	$2->ins.push_back($4);
    	$6->ins.push_back($4);
    	$$=$4;
    }
    | TOKEN_LEFT expression TOKEN_RIGHT TOKEN_WNTIL TOKEN_LEFT expression TOKEN_RIGHT{
    	$4->type=WNTIL;
    	$4->outs.push_back($2);
    	$4->outs.push_back($6);
    	$2->ins.push_back($4);
    	$6->ins.push_back($4);
    	$$=$4;
    }
    | TOKEN_NEXT TOKEN_LEFT expression TOKEN_RIGHT {
			$1->type=NEXT;
			$1->outs.push_back($3);
			$3->ins.push_back($1);
			$$=$1;
		} 
    | TOKEN_NOT TOKEN_LEFT expression TOKEN_RIGHT{
			$1->type=NOT;
			$1->outs.push_back($3);
			$3->ins.push_back($1);
			$$=$1;
		} 
    | TOKEN_TRUE{
			$1->type=TRUE_NODE;
			$$=$1;
    }
    | TOKEN_FALSE{
			$1->type=FALSE_NODE;
			$$=$1;
    }
    | TOKEN_ATOMIC{
    	$1->type=ATOMIC;
    	$$=$1;    
    }
    ;


%%




