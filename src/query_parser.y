/*
 | Author: Michael Schmidt;
 |         Stefanie Scherzinger;
 |         Gunnar Jehl 
 |
 | ************************* SOFTWARE LICENSE AGREEMENT ***********************
 | This source code is published under the BSD License.
 |
 | See file 'LICENSE.txt' that comes with this distribution or
 | http://dbis.informatik.uni-freiburg.de/index.php?project=GCX/license.php
 | for the full license agreement.
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
 | ****************************************************************************
*/
/*! @file
 * 	@brief PARSER.
 * 	@details PARSER.
 * 	@author Stefanie Scherzinger
 * 	@author Gunnar Jehl
 * 	@version 2.1
 * 	@license Software License Agreement (BSD License)
 */
%{

//! @namespace std
using namespace std;

#include <iostream>
#include <vector>
#include <cstdio>
#include <utility>
#include <list>
#include <math.h>
#include <sstream>
#include "streammanager.h"
#include "debug.h"
#include "expression.h"
#include "docexpression.h"
#include "commentexpression.h"
#include "emptyexpression.h"
#include "constexpression.h"
#include "stringconstexpression.h"
#include "numericconstexpression.h"
#include "sequenceexpression.h"
#include "forexpression.h"
#include "varstepexpression.h"
#include "varexpression.h"
#include "nodeconstructexpression.h"
#include "ifexpression.h"
#include "whereexpression.h"
#include "signoffexpression.h"
#include "operandexpression.h"
#include "condexpression.h"
#include "andcondexpression.h"
#include "orcondexpression.h"
#include "notcondexpression.h"
#include "existscondexpression.h"
#include "emptycondexpression.h"
#include "varstepcondexpression.h"
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
#include "aggregatefunctmedianexpression.h"
#include "roundingexpression.h"
#include "functabsexpression.h"
#include "functceilingexpression.h"
#include "functcoverexpression.h"
#include "functfloorexpression.h"
#include "functroundexpression.h"
#include "functroundhalftoevenexpression.h"
#include "functtruncateexpression.h"
#include "typeenums.h"
#include "parseexception.h"
#include "varname.h"
#include "miscfunctions.h"

/* -----------------------------------------*/
#ifdef DBG_YACC
/*! @def YYDEBUG
 *  @brief YACC Parser debug option.
 *  @details YACC Parser debug option.
 */
#define YYDEBUG 1
/*! @def YYPRINT(file, type, value)
 *  @brief YACC Parser debug print method.
 *  @details YACC Parser debug print method.
 */
// #define YYPRINT(file, type, value) cout << value << endl;
#endif
/* -----------------------------------------*/

/*! @var extern FILE* yyin
 *  @brief XQuery input file.
 *  @details XQuery input file.
 */
extern FILE* yyin; // reference to query file

/*! @var DocExpression* doc
 *  @brief XQuery function <tt>fn:doc</tt>.
 *  @details XQuery function <tt>fn:doc</tt>.
 */
DocExpression* doc;

/*! @fn int parse_query(DocExpression* _doc)
 *  @brief Input (X)Query parsing.
 *  @details Input (X)Query parsing.
 *  @param[in] _doc Pointer to a DocExpression object.
 *  @retval int <tt>0</tt> if input (X)Query has been parsed correctly,
 *  		<tt>1</tt> otherwise.
 */
int parse_query(DocExpression* _doc);

/*! @fn int yylex()
 *  @brief YACC parser method.
 *  @details YACC parser method.
 *  @retval int
 */
int yylex();

/*! @fn int yylex_destroy()
 *  @brief YACC parser method.
 *  @details YACC parser method.
 *  @retval int
 */
int yylex_destroy();

/*! @fn int yyparse()
 *  @brief YACC parser method.
 *  @details YACC parser method.
 *  @retval int
 */
int yyparse();

/*! @fn extern void yyerror(const char* text)
 *  @brief YACC parser method.
 *  @details YACC parser method for printing errors.
 *  @param[in] text Pointer to a char object (error message to be printed).
 *  @retval void
 */
extern void yyerror(const char *text);

/*! @var Expression* parsed_query
 *  @brief Parsed (X)Query input query.
 *  @details Parsed (X)Query input query.
 */
Expression* parsed_query;
 
/*! @var PathExpression* p
 *  @brief Path to append parsed in path step expressions.
 *  @details Path to append parsed in path step expressions.
 */
PathExpression* p;

/*! @typedef list< pair<VarExpression*, Expression*>> var_list_type
 *  @brief The underlying list for parsing comma-separated short cutted ForExpression.
 *  @details The underlying list for parsing comma-separated short cutted ForExpression.
 */
typedef list< pair<VarExpression*, Expression*> > var_list_type;

/*! @fn Expression* make_for_expression(var_list_type* var_list, Expression* return_expr)
 *  @brief Returns constructed for-clauses (with return-clause) from comma-separated short cutted ForExpression.
 *  @details Returns constructed for-clauses (with return-clause) from comma-separated short cutted ForExpression.
 *  @param[in] var_list Pointer to the underlying list for parsing comma-separated short cutted ForExpression.
 *  @param[in] return_expr Pointer to a Expression object (return-clause).
 *  @retval Expression* Pointer to a Expression object.
 */
Expression* make_for_expression(var_list_type* var_list, Expression* return_expr);

/*! @fn void ensure_wellformedness(const char* start_tag, const char* end_tag)
 *  @brief Returns if the input (X)Query is wellformed in respect to the XML definition.
 *  @details Returns if the input (X)Query is wellformed in respect to the XML definition.
 *  @param[in] start_tag Pointer to a char object (opening tag of an XML element).
 *  @param[in] end_tag Pointer to a char object (closing tag of an XML element).
 *  @retval void
 */
void ensure_wellformedness(const char* start_tag, const char* end_tag);

%}

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
  RoundingExpression* rounding;
  COMP_TYPE comptype;
  var_list_type* varlist;
}

%token SLASH DSLASH NT_STAR AND OR EXISTS EMPTY NOT XTRUE XFALSE
%token CHILD DESCENDANT
%token FOR IN XRETURN IF THEN ELSE WHERE
%token RELOP_LT RELOP_LEQ RELOP_EQ RELOP_GT RELOP_GEQ RELOP_NEQ
%token VAR_SIGN LTSLASH GTSLASH
%token STRINGCONST NUMERICCONST QNAME VARNAME
%token COMMA CBOPEN CBCLOSE BOPEN BCLOSE
%token ELEMENTCONTENT XP_TEXT XP_NODE // WHITESPACE
%token SUM AVG MIN MAX COUNT STDDEV_SAMP STDDEV_POP VAR_SAMP VAR_POP MEDIAN
%token ABS CEILING COVER FLOOR ROUND ROUNDHALFTOEVEN TRUNCATE 
%token COMMENT_START COMMENTCONTENT // COMMENT_END
%token DOC 

%type <query> Query VarExpr FWRExpr ReturnQExpr QExpr QExprSingle IfExpr
%type <query> NestedXMLExpr ElementContents XMLExpr

%type <str> COMMENTCONTENT STRINGCONST NUMERICCONST QNAME VARNAME ELEMENTCONTENT QName
%type <str> StartTag EndTag BachelorTag

%type <varexpr> VarRef
%type <path> PathExpr
%type <pathstep> PathStepExpr NodeTest
%type <constexpr> StringConst NumericConst
%type <condexpr> Condition ComparisonExpr WhereClause
%type <operandexpr> OperandExpr
%type <varstep> VarAxisExpr
%type <aggfunct> AggregateFunct
%type <rounding> Rounding
%type <comptype> RelOp
%type <varlist> ExprAppendix ForClause

%%

start: Query {
  parsed_query = $1;
  
  vector<unsigned> x;
  x.push_back(TAGID_ROOT);
  vector<unsigned> introduced_vars;
  introduced_vars.push_back(VarName::getInstance()->insertVarname(ROOTVAR));
  vector<unsigned> violating_vars;
  parsed_query->scopeCheck(x,introduced_vars,violating_vars);

  if (violating_vars.size()>0) {
    std::ostringstream os;
    os << "Scope Check Failed For The Following Variable(s): ";
    for (unsigned i=0;i<violating_vars.size();i++) {
      if (i>0) os << ", ";
        os << "$" << VarName::getInstance()->getVarname(violating_vars[i]);
      }
    yyerror(os.str().c_str());    
  }
}
;

Query : 

/* epsilon */
{
  SequenceExpression* seq = new SequenceExpression(new CommentExpression(NULL), doc);
  seq->insertExp(new EmptyExpression()); 
  $$ = seq;
}

| XMLExpr
{
  SequenceExpression* seq = new SequenceExpression(new CommentExpression(NULL), doc);
  seq->insertExp($1);
  $$ = seq;
}

| COMMENT_START COMMENTCONTENT
{
  SequenceExpression* seq = new SequenceExpression(new CommentExpression($2), doc);
  seq->insertExp(new EmptyExpression());
  $$ = seq;
}

| COMMENT_START COMMENTCONTENT XMLExpr
{ 
  SequenceExpression* seq = new SequenceExpression(new CommentExpression($2), doc);
  seq->insertExp($3);
  $$ = seq;
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
  $$ = new StringConstExpression($1);
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

StringConst
{
  $$ = $1;
}

| NumericConst
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

| Rounding
{
  $$ = $1;
}

| NestedXMLExpr 
{
  $$ = $1;
}
;

StringConst :

STRINGCONST
{
  $$ = new StringConstExpression($1);
}
;

NumericConst :

NUMERICCONST
{
  $$ = new NumericConstExpression($1);
}

FWRExpr :

ForClause XRETURN ReturnQExpr
{
  $$ = make_for_expression($1, $3);
}

| ForClause WhereClause XRETURN ReturnQExpr
{
  $$ = make_for_expression($1, new WhereExpression($2, $4));
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

VarExpr
{
  if ($1->getType()==et_var) {
    $$ = new VarstepCondExpression((VarExpression*) $1);
  } else {
    $$ = new VarstepCondExpression((VarStepExpression*) $1);
  }
}

| XTRUE
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

| EMPTY BOPEN VarExpr BCLOSE
{
  if ($3->getType()==et_var) {
    $$ = new EmptyCondExpression((VarExpression*) $3);
  } else {
    $$ = new EmptyCondExpression((VarStepExpression*) $3);
  }
}

| NOT Condition
{
  $$ = new NotCondExpression($2);
}

| BOPEN Condition BCLOSE
{
  $$ = $2;
}

| Condition AND Condition
{
  $$ = new AndCondExpression($1, $3);
}

| Condition OR Condition
{
  $$ = new OrCondExpression($1, $3);
}

| ComparisonExpr
{
  $$ = $1;
}
;

ComparisonExpr :

OperandExpr RelOp OperandExpr
{
  EXP_TYPE lop_type = $1->getOperandType();
  EXP_TYPE rop_type = $3->getOperandType();

  bool is_lop_aggfunct = lop_type==et_operandaggregatefunct;                        

  bool is_rop_aggfunct = rop_type==et_operandaggregatefunct;

  bool is_lop_rounding = lop_type==et_operandrounding;

  bool is_rop_rounding = rop_type==et_operandrounding;

  lop_type = $1->getType();
  rop_type = $3->getType();
  
  bool is_lop_string = lop_type==et_stringconst;

  bool is_rop_string = rop_type==et_stringconst;
  
  bool is_lop_numeric = lop_type==et_numericconst;
  
  bool is_rop_numeric = rop_type==et_numericconst;
                        
  if (is_lop_aggfunct && is_rop_string ||
      is_rop_aggfunct && is_lop_string ||
      is_lop_rounding && is_rop_string ||
      is_rop_rounding && is_lop_string ||
      is_lop_string && is_rop_numeric ||
      is_rop_string && is_lop_numeric) {
    yyerror("Cannot Compare Numeric To String Or String To Numeric");
  }
  
  $$ = new CondOperandExpression($1, $2, $3);
}
;

OperandExpr :

VarExpr
{
  $$ = (OperandExpression*)$1;
}

| AggregateFunct
{
  $$ = (OperandExpression*)$1;
}

| Rounding
{
  $$ = (OperandExpression*)$1;
}

| StringConst
{
  $$ = (OperandExpression*)$1;
}

| NumericConst
{
  $$ = (OperandExpression*)$1;
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

| MEDIAN BOPEN VarExpr BCLOSE
{
  if ($3->getType()==et_var) {
    $$ = new AggregateFunctMedianExpression((VarExpression*) $3);
  } else {
    $$ = new AggregateFunctMedianExpression((VarStepExpression*) $3);
  }
}
;

Rounding :

ABS BOPEN AggregateFunct BCLOSE
{
  $$ = new FunctAbsExpression((AggregateFunctExpression*) $3);
}

| CEILING BOPEN AggregateFunct BCLOSE
{
  $$ = new FunctCeilingExpression((AggregateFunctExpression*) $3);
}

| COVER BOPEN AggregateFunct BCLOSE
{
  $$ = new FunctCoverExpression((AggregateFunctExpression*) $3);
}

| FLOOR BOPEN AggregateFunct BCLOSE
{
  $$ = new FunctFloorExpression((AggregateFunctExpression*) $3);
}

| ROUND BOPEN AggregateFunct BCLOSE
{
  $$ = new FunctRoundExpression((AggregateFunctExpression*) $3);
}

| ROUNDHALFTOEVEN BOPEN AggregateFunct BCLOSE
{
  $$ = new FunctRoundHalfToEvenExpression((AggregateFunctExpression*) $3);
}

| TRUNCATE BOPEN AggregateFunct BCLOSE
{
  $$ = new FunctTruncateExpression((AggregateFunctExpression*) $3);
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
  $$ = $1;
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

DOC BOPEN STRINGCONST BCLOSE
{
  if (doc->isEmptyPath()) {
    doc->setPath($3);
  } else {
    if (!doc->isPathEqualTo($3)) {
      delete[] $3;
      yyerror("DocExpression: Paths Not Equal");
    }
    delete[] $3;
  }
  $$ = new VarExpression();
}

| BOPEN SLASH BCLOSE
{
  $$ = new VarExpression();
}

| SLASH
{
  $$ = new VarExpression();
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
  $$ = new VarExpression($2);
}
;

VarAxisExpr :

DOC BOPEN STRINGCONST BCLOSE PathExpr
{
  if (doc->isEmptyPath()) {
    doc->setPath($3);
  } else {
    if (!doc->isPathEqualTo($3)) {
      delete[] $3;
      yyerror("DocExpression: Paths Not Equal");
    }
    delete[] $3;
  }
  VarExpression* var = new VarExpression();
  $$ = new VarStepExpression(var->getId(), $5);
  delete var;
}

| VarRef PathExpr
{  
  $$ = new VarStepExpression($1->getId(), $2);
  delete $1;
}

| PathExpr
{
  VarExpression* var = new VarExpression();
  $$ = new VarStepExpression(var->getId(), $1);
  delete var;
}
;

%% /* ------------------------------------------ */

/*! @fn int parse_query(DocExpression* _doc)
 *  @brief Input (X)Query parsing.
 *  @details Input (X)Query parsing.
 *  @param[in] _doc Pointer to a DocExpression object.
 *  @retval int <tt>0</tt> if input (X)Query has been parsed correctly,
 *  		<tt>1</tt> otherwise.
 */
int parse_query(DocExpression* _doc) {
  doc = _doc;
  InputStream* query_istream = StreamManager::getInstance()->getQueryInputStream();
  
  yyin=query_istream->getParserStream();

  // parse query
  int ret=yyparse();
  
#if defined(LINUX) || defined(linux) || defined(_LINUX) || defined(_linux) || defined(__LINUX__) || defined(__linux__)
  yylex_destroy();
#endif
  
  return ret;
}

/*! @fn Expression* make_for_expression(var_list_type* var_list, Expression* sub_expression)
 *  @brief Returns constructed for-clauses (with return-clause) from comma-separated short cutted ForExpression.
 *  @details Returns constructed for-clauses (with return-clause) from comma-separated short cutted ForExpression.
 *  @param[in] var_list Pointer to the underlying list for parsing comma-separated short cutted ForExpression.
 *  @param[in] sub_expression Pointer to a Expression object (return-clause).
 *  @retval Expression* Pointer to a Expression object.
 */
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

/*! @fn void ensure_wellformedness(const char* start_tag, const char* end_tag)
 *  @brief Returns if the input (X)Query is wellformed in respect to the XML definition.
 *  @details Returns if the input (X)Query is wellformed in respect to the XML definition.
 *  @param[in] start_tag Pointer to a char object (opening tag of an XML element).
 *  @param[in] end_tag Pointer to a char object (closing tag of an XML element).
 *  @retval void
 */
void ensure_wellformedness(const char* start_tag, const char* end_tag) {
  if(strcmp(start_tag, end_tag)!=0) {
    std::ostringstream o;
    o << "Not Well-Formed <" << start_tag << ">...<" << end_tag << "> XML";
    yyerror(o.str().c_str());
  }
}
