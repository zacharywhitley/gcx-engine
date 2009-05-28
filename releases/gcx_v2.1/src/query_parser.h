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
     OR = 258,
     AND = 259,
     NOT = 260,
     SLASH = 261,
     DSLASH = 262,
     NT_STAR = 263,
     EXISTS = 264,
     EMPTY = 265,
     XTRUE = 266,
     XFALSE = 267,
     CHILD = 268,
     DESCENDANT = 269,
     FOR = 270,
     IN = 271,
     XRETURN = 272,
     IF = 273,
     THEN = 274,
     ELSE = 275,
     WHERE = 276,
     RELOP_LT = 277,
     RELOP_LEQ = 278,
     RELOP_EQ = 279,
     RELOP_GT = 280,
     RELOP_GEQ = 281,
     RELOP_NEQ = 282,
     VAR_SIGN = 283,
     LTSLASH = 284,
     GTSLASH = 285,
     STRINGCONST = 286,
     NUMERICCONST = 287,
     QNAME = 288,
     VARNAME = 289,
     COMMA = 290,
     CBOPEN = 291,
     CBCLOSE = 292,
     BOPEN = 293,
     BCLOSE = 294,
     ELEMENTCONTENT = 295,
     XP_TEXT = 296,
     XP_NODE = 297,
     SUM = 298,
     AVG = 299,
     MIN = 300,
     MAX = 301,
     COUNT = 302,
     STDDEV_SAMP = 303,
     STDDEV_POP = 304,
     VAR_SAMP = 305,
     VAR_POP = 306,
     MEDIAN = 307,
     ABS = 308,
     CEILING = 309,
     COVER = 310,
     FLOOR = 311,
     ROUND = 312,
     ROUNDHALFTOEVEN = 313,
     TRUNCATE = 314,
     COMMENT_START = 315,
     COMMENTCONTENT = 316,
     DOC = 317
   };
#endif
/* Tokens.  */
#define OR 258
#define AND 259
#define NOT 260
#define SLASH 261
#define DSLASH 262
#define NT_STAR 263
#define EXISTS 264
#define EMPTY 265
#define XTRUE 266
#define XFALSE 267
#define CHILD 268
#define DESCENDANT 269
#define FOR 270
#define IN 271
#define XRETURN 272
#define IF 273
#define THEN 274
#define ELSE 275
#define WHERE 276
#define RELOP_LT 277
#define RELOP_LEQ 278
#define RELOP_EQ 279
#define RELOP_GT 280
#define RELOP_GEQ 281
#define RELOP_NEQ 282
#define VAR_SIGN 283
#define LTSLASH 284
#define GTSLASH 285
#define STRINGCONST 286
#define NUMERICCONST 287
#define QNAME 288
#define VARNAME 289
#define COMMA 290
#define CBOPEN 291
#define CBCLOSE 292
#define BOPEN 293
#define BCLOSE 294
#define ELEMENTCONTENT 295
#define XP_TEXT 296
#define XP_NODE 297
#define SUM 298
#define AVG 299
#define MIN 300
#define MAX 301
#define COUNT 302
#define STDDEV_SAMP 303
#define STDDEV_POP 304
#define VAR_SAMP 305
#define VAR_POP 306
#define MEDIAN 307
#define ABS 308
#define CEILING 309
#define COVER 310
#define FLOOR 311
#define ROUND 312
#define ROUNDHALFTOEVEN 313
#define TRUNCATE 314
#define COMMENT_START 315
#define COMMENTCONTENT 316
#define DOC 317




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 211 "query_parser.y"
{
  unsigned tok;
  unsigned ct;
  char* str;
  Expression* query;
  PathExpression* path;
  PathStepExpression* pathstep;
  VarExpression* varexpr;
  ConstExpression* constexpr;
  CondExpression* condexpr;
  OperandExpression* operandexpr;
  VarStepExpression* varstep;
  AggregateFunctExpression* aggfunct;
  RoundingExpression* rounding;
  COMP_TYPE comptype;
  var_list_type* varlist;
}
/* Line 1529 of yacc.c.  */
#line 191 "query_parser.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

