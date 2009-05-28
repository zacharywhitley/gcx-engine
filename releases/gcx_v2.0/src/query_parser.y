/*
 | Author: Stefanie Scherzinger;
 |         Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 ===========================================================================
 ===========================================================================
 | Software License Agreement (BSD License)
 | 
 | Copyright (c) 2006-2007, Saarland University Database Group 
 | All rights reserved.
 |
 | Redistribution and use of this software in source and binary forms,
 | with or without modification, are permitted provided that the following
 | conditions are met:
 | 
 | * Redistributions of source code must retain the above
 |   copyright notice, this list of conditions and the
 |   following disclaimer.
 |
 | * Redistributions in binary form must reproduce the above
 |   copyright notice, this list of conditions and the
 |   following disclaimer in the documentation and/or other
 |   materials provided with the distribution.
 |
 | * Neither the name the of Saarland University Database Group nor the names 
 |   of its contributors may be used to endorse or promote products derived 
 |   from this software without specific prior written permission of the
 |   Saarland University Database Group.
 | 
 | THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 | AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 | IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 | ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 | LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 | CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 | SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 | INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 | CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 | ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 | POSSIBILITY OF SUCH DAMAGE.
*/
/*! @file
 * 	@brief PARSER.
 * 	@details PARSER.
 * 	@author Stefanie Scherzinger
 * 	@author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
%{

using namespace std;

#include <iostream>
#include <vector>
#include <stdio.h>
#include <pair.h>
#include <list.h>
#include <math.h>
#include <sstream>
#include "debug.h"
#include "expression.h"
#include "emptyexpression.h"
#include "constexpression.h"
#include "sequenceexpression.h"
#include "forexpression.h"
#include "varstepexpression.h"
#include "varexpression.h"
#include "nodeconstructexpression.h"
#include "ifexpression.h"
#include "signoffexpression.h"
#include "operandexpression.h"
#include "condexpression.h"
#include "andcondexpression.h"
#include "orcondexpression.h"
#include "notcondexpression.h"
#include "existscondexpression.h"
#include "condoperandexpression.h"
#include "truecondexpression.h"
#include "falsecondexpression.h"
#include "pathexpression.h"
#include "pathsteptagexpression.h"
#include "pathstepstarexpression.h"
#include "pathstepnodeexpression.h"
#include "pathsteptextexpression.h"
#include "aggregatefunctexpression.h"
#include "aggregatefunctsumexpression.h"
#include "aggregatefunctavgexpression.h"
#include "aggregatefunctminexpression.h"
#include "aggregatefunctmaxexpression.h"
#include "aggregatefunctcountexpression.h"
#include "aggregatefunctstddevsampexpression.h"
#include "aggregatefunctstddevpopexpression.h"
#include "aggregatefunctvarsampexpression.h"
#include "aggregatefunctvarpopexpression.h"
#include "aggregatefunctlistexpression.h"
#include "typeenums.h"
#include "error.h"

/* -----------------------------------------*/
#ifdef DBG_YACC
#define YYDEBUG 1
//#define YYPRINT(file, type, value) cout << value << endl;
#endif
/* -----------------------------------------*/

extern FILE* yyin; // reference to query file
const char* xquery_file; // name of query file

int parse_query(const char *);

int yylex();
int yylex_init();
int yylex_destroy();
int yyparse();
extern void yyerror(const char *);

Expression* the_query; // abstract syntax tree of XQuery
 
PathExpression* p;

typedef list< pair<VarExpression*, Expression*> > var_list_type;

/*
  statically transform syntactic sugar of the form 
  "for $x in /a, $y in $x/b, $z in $y/c return ..." to
  "for $x in /a return ( for $y in $x/b return (for $z in $y/c return ...) )
*/
Expression* make_for_expression(var_list_type* var_list, Expression* return_expr);

// ensure "start_tag == end_tag"
void ensure_wellformedness(const char* start_tag, const char* end_tag);

%}

/* %nonassoc AND OR */

%left OR
%left AND
%right NOT

%union
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
  COMP_TYPE comptype;
  var_list_type* varlist;
}

%token SLASH DSLASH NT_STAR AND OR EXISTS NOT XTRUE XFALSE
%token CHILD DESCENDANT
%token FOR IN XRETURN IF THEN ELSE WHERE
%token RELOP_LT RELOP_LEQ RELOP_EQ RELOP_GT RELOP_GEQ RELOP_NEQ
%token VAR_SIGN LTSLASH GTSLASH
%token CONSTSTRING QNAME VARNAME
%token COMMA CBOPEN CBCLOSE BOPEN BCLOSE
%token ELEMENTCONTENT XP_TEXT XP_NODE // WHITESPACE
%token SUM AVG MIN MAX COUNT STDDEV_SAMP STDDEV_POP VAR_SAMP VAR_POP LIST

%type <query> Query VarExpr FWRExpr ReturnQExpr QExpr QExprSingle IfExpr
%type <query> NestedXMLExpr ElementContents XMLExpr

%type <str> CONSTSTRING QNAME VARNAME ELEMENTCONTENT QName
%type <str> StartTag EndTag BachelorTag

%type <varexpr> VarRef
%type <path> PathExpr
%type <pathstep> PathStepExpr NodeTest
%type <constexpr> ConstString
%type <condexpr> Condition ComparisonExpr WhereClause
%type <operandexpr> OperandExpr
%type <varstep> VarAxisExpr
%type <aggfunct> AggregateFunct
%type <comptype> RelOp
%type <varlist> ExprAppendix ForClause

%%

start: Query {
  the_query = $1;
  
  vector<unsigned> x;
  x.push_back(TAGID_ROOT);
  vector<unsigned> introduced_vars;
  introduced_vars.push_back(VarName::getInstance()->insertVarname(ROOTVAR));
  if (!the_query->scopeCheck(x,introduced_vars)) {
    Error::getInstance()->throwError("Scope Check Failed!");
  }
}
;

Query : 

/* epsilon */
{
  $$ = new EmptyExpression();
}

| XMLExpr
{
  $$ = $1;
}
;

StartTag :

RELOP_LT QName RELOP_GT
{
  $$ = $2;
}
;

EndTag :

LTSLASH QName RELOP_GT
{
  $$ = $2;
}
;

BachelorTag :

RELOP_LT QName GTSLASH
{
  $$ = $2;
}
;

XMLExpr : 

StartTag NestedXMLExpr EndTag
{
  ensure_wellformedness($1, $3);
  $$ = new NodeConstructExpression($1, $2);
  free((char*)$1);
  free((char*)$3);
}

| BachelorTag
{
  $$ = new NodeConstructExpression($1, new EmptyExpression());
  free((char*)$1);
}

| StartTag EndTag
{
  $$ = new NodeConstructExpression($1, new EmptyExpression());
  free((char*)$1);
  free((char*)$2);
}
;

NestedXMLExpr :

CBOPEN QExpr CBCLOSE
{
  $$ = $2;
}

| ElementContents
{
  $$ = $1;
}

| XMLExpr 
{
  $$ = $1;
}

| NestedXMLExpr NestedXMLExpr
{
  $$ = new SequenceExpression($1, $2);
}
;

ElementContents :

ELEMENTCONTENT
{
  char* content = $1;
  $$ = new ConstExpression(content);
}
; 

QExpr :

ReturnQExpr
{
  $$ = $1;
}

| QExpr COMMA QExpr
{
  $$ = new SequenceExpression($1, $3);
}
;

QExprSingle :

ConstString
{
  $$ = $1;
}

| FWRExpr
{
  $$ = $1;
}

| IfExpr
{
  $$ = $1;
}

| VarExpr
{
  $$ = $1;
}

| AggregateFunct
{
  $$ = $1;
}

| NestedXMLExpr 
{
  $$ = $1;
}
;

ConstString :

CONSTSTRING
{
  char* str = $1;
  $$ = new ConstExpression(str);
}
;

FWRExpr :

ForClause XRETURN ReturnQExpr
{
  Expression* for_expr = make_for_expression($1, $3);
  $$ = for_expr;
}

| ForClause WhereClause XRETURN ReturnQExpr
{
  Expression* return_expr = new IfExpression($2, $4, new EmptyExpression());
  $$ = make_for_expression($1, return_expr);
}
;

ReturnQExpr :

QExprSingle
{
  $$ = $1;
}

| BOPEN QExpr BCLOSE
{
  $$ = $2;
}

| BOPEN BCLOSE
{
  $$ = new EmptyExpression();
};

ForClause :

FOR VarRef IN VarExpr ExprAppendix
{
  if ($4->getType()==et_var) {
    $5->push_back(pair<VarExpression*, Expression*>($2, (VarExpression*) $4));
  } else {
    $5->push_back(pair<VarExpression*, Expression*>($2, (VarStepExpression*) $4));
  }
  $$ = $5;
}
;

WhereClause:

WHERE Condition
{
  $$ = $2;
}
;

Condition :

XTRUE
{
  $$ = new TrueCondExpression();
}

| XFALSE
{
  $$ = new FalseCondExpression();
}

| EXISTS BOPEN VarExpr BCLOSE
{
  if ($3->getType()==et_var) {
    $$ = new ExistsCondExpression((VarExpression*) $3);
  } else {
    $$ = new ExistsCondExpression((VarStepExpression*) $3);
  }
}

| BOPEN Condition BCLOSE
{
  $$ = $2;
}

| ComparisonExpr
{
  $$ = $1;
}

| NOT Condition
{
  $$ = new NotCondExpression($2);
}

| Condition AND Condition
{
  $$ = new AndCondExpression($1, $3);
}

| Condition OR Condition
{
  $$ = new OrCondExpression($1, $3);
}
;

ComparisonExpr :

OperandExpr RelOp OperandExpr
{
  $$ = new CondOperandExpression($1, $2, $3);
}
;

OperandExpr :

VarExpr
{
  if ($1->getType()==et_var) {
    $$ = new OperandExpression((VarExpression*) $1);
  } else {
    $$ = new OperandExpression((VarStepExpression*) $1);
  }
}

| AggregateFunct
{
  $$ = new OperandExpression((AggregateFunctExpression*) $1);
}

| CONSTSTRING
{
  ConstExpression* cexp = new ConstExpression((char*) $1);
  $$ = new OperandExpression((ConstExpression*) cexp);
}
;

ExprAppendix :

/* epsilon */
{
  $$ = new var_list_type();
}

| COMMA VarRef IN VarExpr ExprAppendix
{
  if ($4->getType()==et_var) {
    $5->push_back( pair<VarExpression*, Expression*>($2, (VarExpression*) $4));
  } else {
    $5->push_back( pair<VarExpression*, Expression*>($2, (VarStepExpression*) $4));
  }
  $$ = $5;
}
;

RelOp :

RELOP_EQ
{
  $$ = ct_eq;
}

| RELOP_LT
{
  $$ = ct_lt;
}

| RELOP_GT
{
  $$ = ct_gt;
}

| RELOP_GEQ
{
  $$ = ct_geq;
}

| RELOP_LEQ
{
  $$ = ct_leq;
}

| RELOP_NEQ
{
  $$ = ct_neq;
}
;

AggregateFunct :

SUM BOPEN VarExpr BCLOSE
{
  if ($3->getType()==et_var) {
    $$ = new AggregateFunctSumExpression((VarExpression*) $3);
  } else {
    $$ = new AggregateFunctSumExpression((VarStepExpression*) $3);
  }  
}

| AVG BOPEN VarExpr BCLOSE
{
  if ($3->getType()==et_var) {
    $$ = new AggregateFunctAvgExpression((VarExpression*) $3);
  } else {
    $$ = new AggregateFunctAvgExpression((VarStepExpression*) $3);
  }
}

| MIN BOPEN VarExpr BCLOSE
{
  if ($3->getType()==et_var) {
    $$ = new AggregateFunctMinExpression((VarExpression*) $3);
  } else {
    $$ = new AggregateFunctMinExpression((VarStepExpression*) $3);
  }
}

| MAX BOPEN VarExpr BCLOSE
{
  if ($3->getType()==et_var) {
    $$ = new AggregateFunctMaxExpression((VarExpression*) $3);
  } else {
    $$ = new AggregateFunctMaxExpression((VarStepExpression*) $3);
  }
}

| COUNT BOPEN VarExpr BCLOSE
{
  if ($3->getType()==et_var) {
    $$ = new AggregateFunctCountExpression((VarExpression*) $3);
  } else {
    $$ = new AggregateFunctCountExpression((VarStepExpression*) $3);
  }
}

| STDDEV_SAMP BOPEN VarExpr BCLOSE
{
  if ($3->getType()==et_var) {
    $$ = new AggregateFunctStdDevSampExpression((VarExpression*) $3);
  } else {
    $$ = new AggregateFunctStdDevSampExpression((VarStepExpression*) $3);
  }
}

| STDDEV_POP BOPEN VarExpr BCLOSE
{
  if ($3->getType()==et_var) {
    $$ = new AggregateFunctStdDevPopExpression((VarExpression*) $3);
  } else {
    $$ = new AggregateFunctStdDevPopExpression((VarStepExpression*) $3);
  }
}

| VAR_SAMP BOPEN VarExpr BCLOSE
{
  if ($3->getType()==et_var) {
    $$ = new AggregateFunctVarSampExpression((VarExpression*) $3);
  } else {
    $$ = new AggregateFunctVarSampExpression((VarStepExpression*) $3);
  }
}

| VAR_POP BOPEN VarExpr BCLOSE
{
  if ($3->getType()==et_var) {
    $$ = new AggregateFunctVarPopExpression((VarExpression*) $3);
  } else {
    $$ = new AggregateFunctVarPopExpression((VarStepExpression*) $3);
  }
}

| LIST BOPEN VarExpr BCLOSE
{
  if ($3->getType()==et_var) {
    $$ = new AggregateFunctListExpression((VarExpression*) $3);
  } else {
    $$ = new AggregateFunctListExpression((VarStepExpression*) $3);
  }
}
;

PathStepExpr :

ChildAxis NodeTest
{
  p = new PathExpression();
  p->addPathStep($2);
}

| DescendantAxis NodeTest
{
  p = new PathExpression();
  PathStepExpression* ps = $2;
  ps->setAxisType(at_descendant); // override default "at_child"
  p->addPathStep(ps);
}

| PathStepExpr ChildAxis NodeTest /* left recursion to avoid stack overflow and to get correct pathsteps order */
{
  p->addPathStep($3);
}

| PathStepExpr DescendantAxis NodeTest /* left recursion to avoid stack overflow and to get correct pathsteps order */
{  
  PathStepExpression* ps = $3;
  ps->setAxisType(at_descendant); // override default "at_child"
  p->addPathStep(ps);
}
;

ChildAxis :

SLASH

| SLASH CHILD
;

DescendantAxis :

DSLASH

| SLASH DESCENDANT
; 

NodeTest:

XP_NODE   /* node() */
{
  $$ = new PathStepNodeExpression();
}

| XP_TEXT /* text() */
{
  $$ = new PathStepTextExpression();
}

| NT_STAR
{
  $$ = new PathStepStarExpression();
}

| QName
{  
  $$ = new PathStepTagExpression($1);
  free((char*)$1);
}
;

QName :

QNAME
{
  char* qname = $1;
  $$ = qname;
}
;

PathExpr :

PathStepExpr
{
  $$ = p;
}
;

IfExpr :

IF BOPEN Condition BCLOSE THEN ReturnQExpr ELSE ReturnQExpr
{
  $$ = new IfExpression($3, $6, $8);
}
;

VarExpr:

BOPEN SLASH BCLOSE
{
  $$ = new VarExpression(strdup(ROOTVAR), true);
}

| SLASH
{
  $$ = new VarExpression(strdup(ROOTVAR), true);
}

| VarRef
{
  $$ = $1;
}

| VarAxisExpr
{
  $$ = $1;
}
;

VarRef :

VAR_SIGN VARNAME
{
  char* varname = $2;
  $$ = new VarExpression(varname, true);
}
;

VarAxisExpr :

VarRef PathExpr
{
  $$ = new VarStepExpression($1, $2);
}

| PathExpr
{
  VarExpression* v = new VarExpression(strdup(ROOTVAR), false);
  $$ = new VarStepExpression(v, $1);
}
;

%% /* ------------------------------------------ */

int parse_query(const char* query_file) {
  xquery_file = query_file;

  // parse query
  yyin = fopen(query_file,"r");
  if (yyin==0) {
    std::ostringstream o;
    o << "Query File Error: Unable To Open File \"" << query_file << "\".";
    Error::getInstance()->throwError(o.str().c_str());
  }

  // TODO: incomment in final version!!!
  // yylex_init();
  int ret=yyparse();
  fclose(yyin);
  // TODO: incomment in final version!!!
  // yylex_destroy();
  return ret;
}

Expression* make_for_expression(var_list_type* var_list, Expression* sub_expression) {
  var_list_type::const_iterator it;
  
  for(it=var_list->begin(); it!=var_list->end(); it++) {
    if (it->second->getType()==et_var) {
      sub_expression = new ForExpression(it->first, (VarExpression*) it->second, sub_expression);
    } else {
      sub_expression = new ForExpression(it->first, (VarStepExpression*) it->second, sub_expression);
    }
  }
  delete var_list;

  return sub_expression;
}

void ensure_wellformedness(const char* start_tag, const char* end_tag) {
  if( strcmp(start_tag, end_tag) != 0) {
    std::ostringstream o;
    o << "Query File \"" << xquery_file << "\" Error: Malformed " << start_tag << " And " << end_tag << " (Attempt To Create Not Well-Formed (XML-)Output).";
    yyerror(o.str().c_str());
  }
}
