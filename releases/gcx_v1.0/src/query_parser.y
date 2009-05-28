/*
 | Author: Stefanie Scherzinger
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
 |  following disclaimer.
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
%{

using namespace std;

#include <iostream>
#include <vector>
#include <stdio.h>
#include <pair.h>
#include <list.h>
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
#include "someexpression.h"
#include "updateexpression.h"
#include "condexpression.h"
#include "andcondexpression.h"
#include "orcondexpression.h"
#include "notcondexpression.h"
#include "existscondexpression.h"
#include "nodeconsttestcondexpression.h"
#include "nodenodetestcondexpression.h"
#include "truecondexpression.h"
#include "pathstepexpression.h"
#include "pathstepnodeexpression.h"
#include "pathsteptextexpression.h"
#include "expenums.h"
#include "error.h"


/* -----------------------------------------*/
#ifdef DBG_YACC 
#define YYDEBUG 1 
//#define YYPRINT(file, type, value)   cout << value << endl;
#endif 
/* -----------------------------------------*/


extern FILE *yyin; // reference to query file
const char * xquery_file; // name of query file


int parse_query(const char *);

int yyparse();
int yylex();
extern void yyerror(const char *);

 Expression * the_query; // abstract syntax tree of XQuery


typedef list< pair<VarExpression*, VarStepExpression*> > var_list_type;

/*
  statically transform syntactic sugar
  of the form "for $x in /a, $y in $x/b, $z in $y/c return ..."
  to          "for $x in /a return ( for $y in $x/b return (for $z in $y/c return ...) )
*/ 
Expression * make_for_expression(var_list_type * var_list, Expression * return_expr);

// ensure "start_tag == end_tag"
void ensure_wellformedness(const char * start_tag, const char * end_tag);

%}


/* %nonassoc AND OR */

%left OR
%left AND
%right NOT


%union
{
  unsigned tok;
  unsigned ct;
  char *str;
  Expression* query;
  PathStepExpression * pathstep;
  VarExpression * varexpr;
  ConstExpression * constexpr;
  CondExpression * condexpr;
  VarStepExpression * varstep;
  COMP_TYPE comptype;
  var_list_type * varlist;
}

%token SLASH DSLASH QUADRUCOLON NT_STAR NT_TEXT AND OR EXISTS NOT XTRUE XFALSE
%token CHILD DESCENDANT 
%token LT LEQ EQ GEQ GT
%token FOR IN XRETURN IF THEN ELSE SOME SATISFIES WHERE
%token RELOP_LEQ RELOP_NEQ RELOP_LT RELOP_EQ RELOP_GT 
%token RELOP_GEQ VAR_SIGN LTSLASH GTSLASH
%token CONSTSTRING LABEL QNAME VARNAME
%token COMMA CBOPEN CBCLOSE BOPEN BCLOSE 
%token ELEMENTCONTENT XP_TEXT XP_NODE // WHITESPACE 

%type <query> Query VarExpr FWRExpr ReturnQExpr QExpr QExprSingle IfExpr
%type <query> NestedXMLExpr ElementContents XMLExpr

//TrueCondExpr NNTCondExpr NCTCondExpr AndCondExpr OrCondExpr NotCondExpr
%type <str> CONSTSTRING LABEL QNAME VARNAME ELEMENTCONTENT QName
%type <str> StartTag EndTag BachelorTag

 //%type <ct> RelOp
%type <varexpr> VarRef
%type <pathstep> PathStepExpr NodeTest
%type <constexpr> ConstString
%type <condexpr> Condition ComparisonExpr QuantifiedExpr WhereClause
%type <varstep> VarAxisExpr
%type <comptype> RelOp
%type <varlist> ExprAppendix ForClause

%%


start: Query 
      {
	the_query = $1;

         vector<unsigned> x;
         x.push_back(0);
         vector<unsigned> introduced_vars;
         introduced_vars.push_back(
			VarName::getInstance()->addVar(ROOTVAR));
         if (!the_query->scopeCheck(x,introduced_vars)) {
           cout << "SCOPE CHECK FAILED!" << endl;
           exit(-1);
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
}

| BachelorTag
{
  $$ = new NodeConstructExpression($1, new EmptyExpression());
}

| StartTag EndTag
{
  $$ = new NodeConstructExpression($1, new EmptyExpression());
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
  char * content = $1; 
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

| NestedXMLExpr 
{
  $$ = $1;
}
;


ConstString :

CONSTSTRING
{
  char * str = $1; 
  $$ = new ConstExpression(str);
}
;



FWRExpr :

ForClause XRETURN ReturnQExpr 
{
  Expression * for_expr = make_for_expression($1, $3);
  $$ = for_expr;
}

| ForClause WhereClause XRETURN ReturnQExpr
{
  Expression * return_expr = new IfExpression($2, $4, new EmptyExpression());

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

FOR VarRef IN VarAxisExpr ExprAppendix
{

  $5->push_back(pair<VarExpression*, VarStepExpression*>($2, $4));
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
  $$ = new NotCondExpression(new TrueCondExpression());
}

| EXISTS BOPEN VarAxisExpr BCLOSE
{
  $$ = new ExistsCondExpression($3);
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


| QuantifiedExpr
{
  $$ = $1;
}

;


ComparisonExpr :

VarExpr RelOp VarExpr
{
  //  $$ = new NodeNodeTestCondExpression($1, $2, $3);
    
  const EXP_TYPE e1 = $1->getType();
  const EXP_TYPE e2 = $3->getType();

  if (e1 == e_typevar)
    
    if (e2 == e_typevar)
      $$ = new NodeNodeTestCondExpression((VarExpression*) $1, $2, (VarExpression*) $3);
  
    else
      $$ = new NodeNodeTestCondExpression((VarExpression*) $1, $2, (VarStepExpression*) $3);

  else // e1= e_typevarstep

    if (e2 == e_typevar)
      $$ = new NodeNodeTestCondExpression((VarStepExpression*) $1, $2, (VarExpression*) $3);
  
    else
      $$ = new NodeNodeTestCondExpression((VarStepExpression*) $1, $2, (VarStepExpression*) $3);

}

| VarExpr RelOp CONSTSTRING
{
  const EXP_TYPE e = $1->getType();
  char * str = $3; 


  if (e == e_typevar)
    $$ = new NodeConstTestCondExpression((VarExpression*)$1, $2, str);

  else
    $$ = new NodeConstTestCondExpression((VarStepExpression*)$1, $2, str);

}

| CONSTSTRING RelOp VarExpr
{
  const EXP_TYPE e = $3->getType();
  char * str = $1; 


  if (e == e_typevar)
    $$ = new NodeConstTestCondExpression((VarExpression*)$3, $2, str);

  else
    $$ = new NodeConstTestCondExpression((VarStepExpression*)$3, $2, str);
}

| CONSTSTRING RelOp CONSTSTRING
{
  char * str1 = $1; 
  char * str2 = $3; 


  // evaluate statically to true or false

  int comp_result = strcmp(str1, str2);
  bool bool_result = false;

  switch($2)
    {
    case e_lt : 
      bool_result = (comp_result < 0);

    case e_leq :
      bool_result = (comp_result <= 0);

    case e_gt :
      bool_result = (comp_result > 0);

    case e_geq :
      bool_result = (comp_result >= 0);

    case e_neq :
      bool_result = (comp_result != 0);

    case e_eq:
      bool_result = (comp_result == 0);
    };


  if (bool_result)
    $$ = new TrueCondExpression();

  else
    $$ = new NotCondExpression(new TrueCondExpression());

}
;


QuantifiedExpr :

SOME VarRef IN VarAxisExpr ExprAppendix SATISFIES Condition
{
  
  var_list_type * var_list = $5;
  var_list_type::const_iterator it;
  
  CondExpression * sub_expression = $7;
  
  for(it=var_list->begin(); it!=var_list->end(); it++)
    sub_expression = new SomeExpression(it->first, it->second, sub_expression);
  
  delete var_list;

  $$ = new SomeExpression($2, $4, sub_expression);

}
;


ExprAppendix :

/* epsilon */
{
  $$ = new var_list_type();
}

| COMMA VarRef IN VarAxisExpr ExprAppendix
{
  $5->push_back( pair<VarExpression*,VarStepExpression*>($2, $4)) ;

  $$ = $5;
}

;



RelOp :

RELOP_EQ
{
  $$ = e_eq;
}

| RELOP_LT
{
  $$ = e_lt;
}

| RELOP_GT 
{
  $$ = e_gt;
}

| RELOP_GEQ
{
  $$ = e_geq;
}

| RELOP_LEQ
{
  $$ = e_leq;
}

| RELOP_NEQ
{
  $$ = e_neq;
}
;


PathStepExpr :

BOPEN SLASH BCLOSE
{
  $$ = new PathStepNodeExpression(e_self);
}


| 
SLASH
{
  $$ = new PathStepNodeExpression(e_self);
}

| ChildAxis NodeTest
{
  $$ = $2;
}

| DescendantAxis NodeTest
{
  PathStepExpression * ps = $2;
  ps->setAxisType(e_descendant); // override default "e_child"
  $$ = ps;
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
  $$ = new PathStepExpression(PATHSTEP_STAR);
}

| QName
{
  $$ = new PathStepExpression($1);
}
;



QName : 

QNAME
{
  char * qname = $1; 
  $$ = qname;
}
;



IfExpr :  

IF BOPEN Condition BCLOSE THEN ReturnQExpr ELSE ReturnQExpr
{
  $$ = new IfExpression($3, $6, $8);
}
;


VarExpr:

VarRef
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
  char * varname = $2; 
  $$ = new VarExpression(varname);
}
;



VarAxisExpr : 

VarRef PathStepExpr
{
  $$ = new VarStepExpression($1, $2);
}

| PathStepExpr
{
  // absolute path expression -> starts from root variable

  VarExpression * v = new VarExpression(ROOTVAR);
  $$ = new VarStepExpression(v, $1);
}


;


%% /* ------------------------------------------ */


int parse_query(const char * query_file)
{
  xquery_file = query_file;

  // parse query
  yyin = fopen(query_file,"r");
  if (yyin == 0) {
    std::ostringstream s;
    s << "Cannot open query '" << query_file << "'";
    Error::getInstance()->throwError(s.str().c_str());
  }

  return yyparse();
}


Expression * make_for_expression(var_list_type * var_list, Expression * sub_expression)
{
  var_list_type::const_iterator it;
  
  for(it=var_list->begin(); it!=var_list->end(); it++)
    sub_expression = new ForExpression(it->first, it->second, sub_expression);

  delete var_list;

  return sub_expression;
}



void ensure_wellformedness(const char * start_tag, const char * end_tag)
{
  if( strcmp(start_tag, end_tag) != 0)
    {
      cerr << "malformed: '" << start_tag << "' and '" << end_tag << "'" << endl;
      yyerror("Attempt to create malformed output!");
    }
}
