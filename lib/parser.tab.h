/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     N_INCLUDE = 258,
     N_INCLUDE_PHP = 259,
     N_START_CAPTURE = 260,
     N_END_CAPTURE = 261,
     N_LDELIM = 262,
     N_RDELIM = 263,
     N_START_SECTION = 264,
     N_SECTIONELSE = 265,
     N_END_SECTION = 266,
     N_START_FOREACH = 267,
     N_FOREACHLESE = 268,
     N_END_FOREACH = 269,
     N_START_STRIP = 270,
     N_END_STRIP = 271,
     N_INSERT = 272,
     N_CONFIG_LOAD = 273,
     N_BLOCK = 274,
     N_START_LITERAL = 275,
     N_END_LITERAL = 276,
     N_START_PHP = 277,
     N_END_PHP = 278,
     N_START_IF = 279,
     N_ELSE_IF = 280,
     N_ELSE = 281,
     N_END_IF = 282,
     N_VARIABLE_NAME = 283,
     N_NUMBER = 284,
     N_A_FILE = 285,
     N_A_ASSIGN = 286,
     N_A_ONCE = 287,
     N_A_SCRIPT = 288,
     N_A_NAME = 289,
     N_A_LOOP = 290,
     N_A_START = 291,
     N_A_STEP = 292,
     N_A_MAX = 293,
     N_A_SHOW = 294,
     N_A_FROM = 295,
     N_A_ITEM = 296,
     N_A_KEY = 297,
     N_A_SECTION = 298,
     N_A_SCOPE = 299,
     N_A_GLOBAL = 300,
     N_PRINT_STRING = 301,
     N_DOUBLE_QUOTES = 302,
     N_STRING = 303,
     N_IS_EQUAL = 304,
     N_IS_NOT_EQUAL = 305,
     N_IS_SMALLER_OR_EQUA = 306,
     N_IS_GREATER_OR_EQUAL = 307,
     N_IS_SMALLER = 308,
     N_IS_GREATER = 309,
     N_MOD = 310,
     N_BOOLEAN_AND = 311,
     N_BOOLEAN_OR = 312,
     N_BOOLEAN_NOT = 313
   };
#endif
/* Tokens.  */
#define N_INCLUDE 258
#define N_INCLUDE_PHP 259
#define N_START_CAPTURE 260
#define N_END_CAPTURE 261
#define N_LDELIM 262
#define N_RDELIM 263
#define N_START_SECTION 264
#define N_SECTIONELSE 265
#define N_END_SECTION 266
#define N_START_FOREACH 267
#define N_FOREACHLESE 268
#define N_END_FOREACH 269
#define N_START_STRIP 270
#define N_END_STRIP 271
#define N_INSERT 272
#define N_CONFIG_LOAD 273
#define N_BLOCK 274
#define N_START_LITERAL 275
#define N_END_LITERAL 276
#define N_START_PHP 277
#define N_END_PHP 278
#define N_START_IF 279
#define N_ELSE_IF 280
#define N_ELSE 281
#define N_END_IF 282
#define N_VARIABLE_NAME 283
#define N_NUMBER 284
#define N_A_FILE 285
#define N_A_ASSIGN 286
#define N_A_ONCE 287
#define N_A_SCRIPT 288
#define N_A_NAME 289
#define N_A_LOOP 290
#define N_A_START 291
#define N_A_STEP 292
#define N_A_MAX 293
#define N_A_SHOW 294
#define N_A_FROM 295
#define N_A_ITEM 296
#define N_A_KEY 297
#define N_A_SECTION 298
#define N_A_SCOPE 299
#define N_A_GLOBAL 300
#define N_PRINT_STRING 301
#define N_DOUBLE_QUOTES 302
#define N_STRING 303
#define N_IS_EQUAL 304
#define N_IS_NOT_EQUAL 305
#define N_IS_SMALLER_OR_EQUA 306
#define N_IS_GREATER_OR_EQUAL 307
#define N_IS_SMALLER 308
#define N_IS_GREATER 309
#define N_MOD 310
#define N_BOOLEAN_AND 311
#define N_BOOLEAN_OR 312
#define N_BOOLEAN_NOT 313




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 20 "parser.y"
{
	long number;
	char *str;
}
/* Line 1529 of yacc.c.  */
#line 170 "parser.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



