%{
//#undef YY_INPUT
//#define YY_INPUT(buf, result, max_size) (result = my_yyinput(buf, max_size))

//#define DEBUG

#include "namarty_core.h"
#include "parser.tab.h"


//#undef YYSTYPE
//#define YYSTYPE void* 
//#define YYSTYPE ((namarty_core*)param)->yyscanner
//typedef void* yyscan_t;

#undef YY_DECL
#define YY_DECL int yylex(YYSTYPE *lvalp, namarty_core* nc, yyscan_t yyscanner)
#define YY_USER_ACTION nc->setReadByte(yyleng);
//#define YY_USER_ACTION nc->setReadByte(yyleng); printf("`%d, %d, %s`\n", yyg->yy_start, yy_act, yytext);

/*
if (((namarty_core*)param)->clear_buffer) {\
namarty_tpl_delete_buffer(YY_CURRENT_BUFFER);\
((namarty_core*)param)->clear_buffer = false;\
REJECT;\
printf("clear buffer\n");\	
}
*/

// http://flex.sourceforge.net/manual/Code_002dLevel-And-API-Options.html#Code_002dLevel-And-API-Options
// I don't know how to use, but it might help me to develop re-entrant parser
//%option bison-bridge
%}
%option reentrant
%option yylineno
%option prefix="namarty_tpl"


%x IN_COMMAND
%x IN_DOUBLE_QUOTES
%x IN_ATTRIBUTE

%x IN_LETERAL
%x IN_STATIC_STRING
%x IN_DYNAMIC_STRING

 // Command Mode
%x IN_FOR_ATTRIBUTE
%x IN_IF_STATEMENT

NUMBER  [0-9]+
LNUM    [0-9]+
DNUM    ([0-9]*[\.][0-9]+)|([0-9]+[\.][0-9]*)
EXPONENT_DNUM   (({LNUM}|{DNUM})[eE][+-]?{LNUM})
HNUM    "0x"[0-9a-fA-F]+
LABEL   [a-zA-Z0-9_\x7f-\xff\.]*
WHITESPACE [ \n\r\t]+
TABS_AND_SPACES [ \t]*
TOKENS [;:,.\[\]()|^&+-/*=%!~$<>?@]
ENCAPSED_TOKENS [\[\]{}$]
ESCAPED_AND_WHITESPACE [\n\t\r #'.:;,()|^&+-/*=%!~<>?@]+
ANY_CHAR (.|[\n])
NEWLINE ("\r"|"\n"|"\r\n")
SQ '
NOT_SQ ^'
DQ \"
NOT_DQ ^\"

%%
<INITIAL>"{"	{
	BEGIN(IN_COMMAND);
}

<INITIAL>{LABEL} {
	lvalp->str = strdup(yytext);
	DMSG("INITIAL->N_PRINT_STRING");
	return N_PRINT_STRING;
}

<INITIAL>{ESCAPED_AND_WHITESPACE}|{NEWLINE}	{
	lvalp->str = strdup(yytext);
	DMSG("INITIAL->N_PRINT_STRING");
	return N_PRINT_STRING;
}

<INITIAL>{ANY_CHAR}|{TOKENS} {
	lvalp->str = strdup(yytext);
	DMSG("INITIAL->N_PRINT_STRING");
	return N_PRINT_STRING;
}

 /*************************************/
 /*                                   */
 /*	For statement                     */
 /*                                   */
 /*************************************/

<IN_COMMAND>"foreach"	{
	BEGIN(IN_FOR_ATTRIBUTE);
	nc->setTmpStackPoint();
	DMSG("IN_COMMAND->IN_FOR_ATTRIBUTE");
	return N_START_FOREACH;
}

<IN_FOR_ATTRIBUTE>{WHITESPACE}	{
	; /* ignor */
}

<IN_FOR_ATTRIBUTE>{TOKENS}	{
	//printf("TOKEN: `%c`\n", yytext[0]);
	return yytext[0];
}

<IN_FOR_ATTRIBUTE>"}"	{
	BEGIN(INITIAL);
}

<IN_FOR_ATTRIBUTE>{LABEL}	{
	lvalp->str = strdup(yytext);
	DMSG("IN_FOR_ATTRIBUTE->N_STRING");
	return N_STRING;
}

<IN_COMMAND>"foreachelse"	{
	return N_FOREACHLESE;
}

<IN_COMMAND>"/foreach"	{
	return N_END_FOREACH;
}

 /*************************************/
 /*                                   */
 /*	If statement                      */
 /*                                   */
 /*************************************/
<IN_COMMAND>"if" {
	BEGIN(IN_IF_STATEMENT);
	DMSG("N_START_IF->IN_ATTRIBUTE");
	return N_START_IF;
}

<IN_COMMAND>"else" {
	DMSG("N_ELSE");
	return N_ELSE;
}

<IN_COMMAND>"elseif" {
	BEGIN(IN_IF_STATEMENT);
	DMSG("N_ELSE_IF->IN_ATTRIBUTE");
	return N_ELSE_IF;
}

<IN_IF_STATEMENT>"=="|"eq" {
	DMSG("N_IS_EQUAL");
	return N_IS_EQUAL;
}

<IN_IF_STATEMENT>"!="|"ne"|"neq" {
	DMSG("N_IS_NOT_EQUAL");
	return N_IS_NOT_EQUAL;
}

<IN_IF_STATEMENT>">"|"gt" {
	return N_IS_GREATER;
}

<IN_IF_STATEMENT>">="|"ge"|"gte" {
	return N_IS_GREATER_OR_EQUAL;
}

<IN_IF_STATEMENT>"<"|"lt" {
	return N_IS_SMALLER;
}

<IN_IF_STATEMENT>"<="|"lte"|"le" {
	return N_IS_SMALLER_OR_EQUA;
}

<IN_IF_STATEMENT>"&"|"and" {
	return N_BOOLEAN_AND;
}

<IN_IF_STATEMENT>"|"|"or" {
	return N_BOOLEAN_OR;
}

<IN_IF_STATEMENT>"!"|"not" {
	return N_BOOLEAN_NOT;
}

<IN_IF_STATEMENT>"%"|"mod" {
	return N_MOD;
}

<IN_IF_STATEMENT>{NUMBER} {
	lvalp->number = atol(yytext);
	DMSG("N_NUMBER");
	return N_NUMBER;
}

<IN_IF_STATEMENT>{TOKENS} {
	return yytext[0];
}

<IN_IF_STATEMENT>"$"{LABEL}	{
	lvalp->str = strdup(yytext+1);
	DMSG("N_VARIABLE_NAME");
	return N_VARIABLE_NAME;
}

<IN_IF_STATEMENT>{WHITESPACE}	{
	; /* ignor */
}

<IN_IF_STATEMENT>"}"	{
	BEGIN(INITIAL);
}

<IN_IF_STATEMENT>{SQ} { 
	BEGIN(IN_STATIC_STRING);
	return yytext[0];
}

<IN_STATIC_STRING>{SQ} { 
	BEGIN(IN_STATIC_STRING);
	return yytext[0];
}

<IN_STATIC_STRING>{NOT_SQ} {
	BEGIN(IN_IF_STATEMENT);
	lvalp->str = strdup(yytext);
	return N_STRING;
}

<IN_IF_STATEMENT>{DQ} { 
	BEGIN(IN_DYNAMIC_STRING);
}

<IN_DYNAMIC_STRING>{DQ} { 
	BEGIN(IN_DYNAMIC_STRING);
	return yytext[0];
}

<IN_DYNAMIC_STRING>{NOT_DQ} { 
	BEGIN(IN_IF_STATEMENT);
	lvalp->str = strdup(yytext);
	return N_STRING;
}
 
 /*************************************/
 /*                                   */
 /*	Not yet statement                 */
 /*                                   */
 /*************************************/
<IN_COMMAND>"include"	{
	BEGIN(IN_ATTRIBUTE);
	return N_INCLUDE;
}

<IN_COMMAND>"include_php"	{
	BEGIN(IN_ATTRIBUTE);
	return N_INCLUDE_PHP;
}

<IN_COMMAND>"config_load"	{
	BEGIN(IN_ATTRIBUTE);
	return N_CONFIG_LOAD;
}
<IN_ATTRIBUTE>{SQ} { 
	BEGIN(IN_STATIC_STRING);
	return yytext[0];
}

<IN_ATTRIBUTE>{DQ} { 
	BEGIN(IN_DYNAMIC_STRING);
}

<IN_STATIC_STRING>{NOT_SQ} {
	BEGIN(IN_ATTRIBUTE);
	lvalp->str = strdup(yytext);
	return N_STRING;
}

<IN_STATIC_STRING>{SQ} { 
	BEGIN(IN_ATTRIBUTE);
	return yytext[0];
}

<IN_COMMAND>"/if"	{
	BEGIN(IN_ATTRIBUTE);
	DMSG("N_END_IF (IN_ATTRIBUTE)");
	return N_END_IF;
}

<IN_COMMAND>"capture"	{
	BEGIN(IN_ATTRIBUTE);
	return N_START_CAPTURE;
}

<IN_COMMAND>"/capture"	{
	BEGIN(IN_ATTRIBUTE);
	return N_END_CAPTURE;
}

<IN_COMMAND>"ldelim"	{
	BEGIN(IN_ATTRIBUTE);
	return N_LDELIM;
}

<IN_COMMAND>"rdelim"	{
	BEGIN(IN_ATTRIBUTE);
	return N_RDELIM;
}

<IN_COMMAND>"section"	{
	BEGIN(IN_ATTRIBUTE);
	return N_START_SECTION;
}

<IN_COMMAND>"sectionelse"	{
	BEGIN(IN_ATTRIBUTE);
	return N_SECTIONELSE;
}

<IN_COMMAND>"/section"	{
	BEGIN(IN_ATTRIBUTE);
	return N_END_SECTION;
}





<IN_COMMAND>"strip"	{
	BEGIN(IN_ATTRIBUTE);
	return N_START_STRIP;
}

<IN_COMMAND>"/strip"	{
	BEGIN(IN_ATTRIBUTE);
	return N_END_STRIP;
}

<IN_COMMAND>"php"	{
	BEGIN(IN_ATTRIBUTE);
	return N_START_PHP;
}

<IN_COMMAND>"/php"	{
	BEGIN(IN_ATTRIBUTE);
	return N_END_PHP;
}

<IN_COMMAND>"insert"	{
	BEGIN(IN_ATTRIBUTE);
	return N_INSERT;
}

<IN_COMMAND>"block"	{
	BEGIN(IN_ATTRIBUTE);
	return N_BLOCK;
}

<IN_COMMAND>{TOKENS}	{
	return yytext[0];
}

<IN_COMMAND>"$"{LABEL} 	{
	lvalp->str = strdup(yytext+1);
	return N_VARIABLE_NAME;
}

<IN_COMMAND>"}"	{
	BEGIN(INITIAL);
}

<IN_ATTRIBUTE>{WHITESPACE}	{
	; /* ignor */
}

<IN_ATTRIBUTE>{LABEL}	{
	lvalp->str = strdup(yytext);
	//DMSG("IN_ATTRIBUTE->N_STRING");
	return N_STRING;
}

<IN_ATTRIBUTE>{TOKENS}	{
	return yytext[0];
}

<IN_ATTRIBUTE>"file"	{
	return N_A_FILE;
}
<IN_ATTRIBUTE>"assign"	{
	return N_A_ASSIGN;
}
<IN_ATTRIBUTE>"once"	{
	return N_A_ONCE;
}
<IN_ATTRIBUTE>"script"	{
	return N_A_SCRIPT;
}
<IN_ATTRIBUTE>"name"	{
	return N_A_NAME;
}
<IN_ATTRIBUTE>"loop"	{
	return N_A_LOOP;
}
<IN_ATTRIBUTE>"start"	{
	return N_A_START;
}
<IN_ATTRIBUTE>"step"	{
	return N_A_STEP;
}
<IN_ATTRIBUTE>"max"	{
	return N_A_MAX;
}
<IN_ATTRIBUTE>"show"	{
	return N_A_SHOW;
}
<IN_ATTRIBUTE>"from"	{
	DMSG("N_A_FROM");
	return N_A_FROM;
}
<IN_ATTRIBUTE>"item"	{
	DMSG("N_A_ITEM");
	return N_A_ITEM;
}
<IN_ATTRIBUTE>"key"	{
	return N_A_KEY;
}
<IN_ATTRIBUTE>"section"	{
	return N_A_SECTION;
}
<IN_ATTRIBUTE>"scope"	{
	return N_A_SCOPE;
}
<IN_ATTRIBUTE>"global"	{
	return N_A_GLOBAL;
}

<IN_ATTRIBUTE>"}"	{
	BEGIN(INITIAL);
}

<IN_ATTRIBUTE>{TOKENS}	{
	DMSG("TOKENS{");
	DMSG(yytext);
	DMSG("}");
	return yytext[0];
}

<IN_ATTRIBUTE>"\""	{
	BEGIN(IN_DOUBLE_QUOTES);
	DMSG("N_DOUBLE_QUOTES");
	return N_DOUBLE_QUOTES;
}

<IN_ATTRIBUTE>{LABEL}	{
	lvalp->str = strdup(yytext);
	//DMSG("IN_ATTRIBUTE->N_STRING");
	return N_STRING;
}

<IN_DOUBLE_QUOTES>{LABEL}	{
	lvalp->str = strdup(yytext);
	DMSG("IN_DOUBLE_QUOTES->N_STRING");
	return N_STRING;
}
<IN_DOUBLE_QUOTES>{TOKENS}	{
	lvalp->str = strdup(yytext);
	DMSG("N_STRING");
	return N_STRING;
}

<IN_DOUBLE_QUOTES>{WHITESPACE}	{
	DMSG("WHITESPACE");
	/* ignore */
	;
}
<IN_DOUBLE_QUOTES>"\""	{
	BEGIN(IN_ATTRIBUTE);
	DMSG("N_DOUBLE_QUOTES");
	return N_DOUBLE_QUOTES;
}

<INITIAL>"{literal}"	{
	BEGIN(IN_LETERAL);
}

<IN_LETERAL>"{/literal}"	{
	BEGIN(INITIAL);
}

<IN_LETERAL>{LABEL} {
	lvalp->str = strdup(yytext);
	DMSG("N_PRINT_STRING");
	return N_PRINT_STRING;
}

<IN_LETERAL>{ESCAPED_AND_WHITESPACE}|{NEWLINE}	{
	lvalp->str = strdup(yytext);
	DMSG("{ESCAPED_AND_WHITESPACE}|{NEWLINE}");
	return N_PRINT_STRING;
}

<IN_LETERAL>{ANY_CHAR}|{TOKENS} {
	lvalp->str = strdup(yytext);
	DMSG("{ANY_CHAR}|{TOKENS}");
	return N_PRINT_STRING;
}

<IN_COMMAND>{ANY_CHAR} {
	;
}

%%
