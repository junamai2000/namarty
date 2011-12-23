%{

#include "namarty_core.h"
#include "parser.namarty.h"

typedef void* yyscan_t;

%}
// see
// http://www.gnu.org/software/bison/manual/html_mono/bison.html#Bison-Options
%pure_parser
%name-prefix="namarty_tpl"

%parse-param {namarty_core* nc}
%lex-param   {namarty_core* nc}

%parse-param {yyscan_t yyscanner}
%lex-param   {yyscan_t yyscanner}

%union {
	long number;
	char *str;
}

%{

extern int yylex(YYSTYPE *lvalp, namarty_core* nc, yyscan_t yyscanner);

static void yyprint (FILE *file, int type, YYSTYPE value)
{
	fprintf(stderr, "yyprint - %s", value.str);
}

%}
/************************
	command 
*************************/
/* function */
%token N_INCLUDE
%token N_INCLUDE_PHP
%token N_START_CAPTURE
%token N_END_CAPTURE
%token N_LDELIM
%token N_RDELIM
%token N_START_SECTION
%token N_SECTIONELSE
%token N_END_SECTION
%token N_START_FOREACH
%token N_FOREACHLESE
%token N_END_FOREACH
%token N_START_STRIP
%token N_END_STRIP
%token N_INSERT
%token N_CONFIG_LOAD
%token N_BLOCK
%token N_START_LITERAL
%token N_END_LITERAL
%token N_START_PHP
%token N_END_PHP
%token N_START_IF
%token N_ELSE_IF
%token N_ELSE
%token N_END_IF

/* variable */
%token <str> N_VARIABLE_NAME
%token <number> N_NUMBER 

/* attribute */
%token N_A_FILE
%token N_A_ASSIGN
%token N_A_ONCE 
%token N_A_SCRIPT
%token N_A_NAME
%token N_A_LOOP
%token N_A_START
%token N_A_STEP
%token N_A_MAX
%token N_A_SHOW
%token N_A_FROM
%token N_A_ITEM
%token N_A_KEY
%token N_A_SECTION
%token N_A_SCOPE
%token N_A_GLOBAL

%token <str> N_PRINT_STRING

/* ???? */
%token N_DOUBLE_QUOTES
%token <str> N_STRING

/* condition */
%token N_IS_EQUAL
%token N_IS_NOT_EQUAL
%token N_IS_SMALLER_OR_EQUA
%token N_IS_GREATER_OR_EQUAL
%token N_IS_SMALLER
%token N_IS_GREATER
%token N_MOD
%token N_BOOLEAN_AND
%token N_BOOLEAN_OR
%token N_BOOLEAN_NOT

%%

namarty:
	| namarty statements
	{
		;
	}
	;

statements:
	N_PRINT_STRING
	{
		nc->buffer($1);
		free($1);
	}
	|
	N_VARIABLE_NAME
	{
		nc->buffer(nc->get($1)); 
		free($1);
	}
	| foreach {nc->startForeach();} namarty  foreachelse_stm N_END_FOREACH  {nc->endForeach();}
	{
		;
	}
	|  N_START_IF expr  namarty  elseif_stm N_END_IF 
	{
		;
	}
	|  N_START_IF expr  namarty  elseif_stm N_ELSE  namarty  N_END_IF 
	{
		;
	}
	| error
	{
		printf("command error\n");
	}

foreachelse_stm:
	| N_FOREACHLESE {nc->elseForeach();} namarty
	{
		;
	}

elseif_stm:
	| N_ELSE_IF expr  namarty elseif_stm
	{
		;
	}

expr:
	N_VARIABLE_NAME N_IS_EQUAL N_NUMBER 
	{
		;
	}
	;

foreach:
	N_START_FOREACH foreach_attribute
	{
		;
	}
	;

foreach_attribute:
	|
	N_STRING '=' N_STRING foreach_attribute
	{
		nc->setAttribute($1, $3);
		free($1);
		free($3);
	}
	| N_STRING '=' '$' N_STRING foreach_attribute
	{
		nc->setAttribute(string(nc->getDepth(1) + string($1)).c_str(), $4);
		free($1);
		free($4);
	}
	;

%%
/*
int main(void)
{
	namarty_val v;
	v.filename = "test.tpl";
	v.itms.insert(pair<string, string>("FirstName", "NAMAI"));
	v.itms.insert(pair<string, string>("LastName", "JUNYA"));

	FILE *fp;
	fp = fopen(v.filename, "r");

	yyin = fp;
	yyrestart(yyin);
	if (yyparse((void*) &v)) {
		exit(1);
	}
}
*/

int namarty_tplwrap()
{
	return 1;
}

void yyerror(namarty_core* nc, yyscan_t yyscanner, char const *msg)
{
    fprintf(stderr, "namarty compile error: %s\n", msg);
}
