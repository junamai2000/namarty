#ifndef __PASER_NAMARTY_H
#define __PASER_NAMAERY_H

#define YYPRINT(file, type, value)   yyprint(file, type, value)
#define YY_EXTRA_TYPE void *
//#define YYDEBUG 1

typedef void* yyscan_t;
class namarty_core;

extern "C" {
	extern int namarty_tplwrap();
	extern int namarty_tplparse(namarty_core *nc, yyscan_t yyscanner);
	void yyerror(namarty_core* nc, yyscan_t yyscanner, char const *msg);
/*
	YYPARSE_PARAM は使わない
	extern int namarty_tplparse(void *YYPARSE_PARAM);
	extern FILE *namarty_tplin;       // for the file opened by fopen.
	extern char *yytext;     // (optional, include this line if it is used in your test driver)
	char *yyget_text ( yyscan_t scanner );
	int yyget_leng ( yyscan_t scanner );
	FILE *yyget_in ( yyscan_t scanner );
	FILE *yyget_out ( yyscan_t scanner );
	int yyget_lineno ( yyscan_t scanner );
	int  yyget_debug ( yyscan_t scanner );

	void yyset_debug ( int flag, yyscan_t scanner );
	void yyset_in  ( FILE * in_str , yyscan_t scanner );
	void yyset_out  ( FILE * out_str , yyscan_t scanner );
	void yyset_lineno ( int line_number , yyscan_t scanner );
	void yyset_extra ( YY_EXTRA_TYPE user_defined , yyscan_t scanner );
*/
}

extern YY_EXTRA_TYPE yyget_extra ( yyscan_t scanner );
extern int namarty_tpllex_destroy (yyscan_t yyscanner );
extern int namarty_tpllex_init (yyscan_t* scanner);
extern void namarty_tplset_extra (YY_EXTRA_TYPE user_defined ,yyscan_t yyscanner );
extern void namarty_tplset_in  (FILE * in_str ,yyscan_t yyscanner );
extern void namarty_tplrestart (FILE *input_file ,yyscan_t yyscanner );
//extern void namarty_tpl_delete_buffer (YY_BUFFER_STATE b ,yyscan_t yyscanner );

#endif
