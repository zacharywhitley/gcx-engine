/*
 | Author: Stefanie Scherzinger;
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
 *  @brief LEXER.
 *  @details LEXER.
 *  @author Stefanie Scherzinger
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
%option yylineno

%{

using namespace std;

#include <sstream>
#include <vector>
#include <list>
#include "typeenums.h"
#include "parseexception.h"

// forward declarations
class Expression;
class PathExpression;
class PathStepExpression;
class VarExpression;
class ConstExpression;
class CondExpression;
class OperandExpression;
class VarStepExpression;
class AggregateFunctExpression;
class RoundingExpression;

/*! @typedef list< pair<VarExpression*, Expression*>> var_list_type
 *  @brief The underlying list for parsing comma-separated short cutted ForExpression.
 *  @details The underlying list for parsing comma-separated short cutted ForExpression.
 */
typedef list< pair<VarExpression*, Expression*> > var_list_type;

#include "query_parser.h"

/*! @fn void yyerror(const char* text)
 *  @brief YACC parser method.
 *  @details YACC parser method for printing errors.
 *  @param[in] text Pointer to a char object (error message to be printed).
 *  @retval void
 */
void yyerror(const char *text);

/*! @var vector<int> stack
 *  @brief Stack (with different semantics from yy_push_state) for maintaining lexer start conditions.
 *  @details Stack (with different semantics from yy_push_state) for maintaining lexer start conditions.
 */
vector<int> stack;

/*! @var int comment_level
 *  @brief XQuery comment level/depth counter.
 *  @details XQuery comment level/depth counter.
 */
int comment_level = 0;

/*! @var ostringstream comment
 *  @brief XQuery comment appending.
 *  @details XQuery comment appending.
 */
ostringstream comment;

/*! @fn void push_state(int state)
 *  @brief YACC parser method.
 *  @details YACC parser method.
 *  @param[in] state State.
 *  @retval void
 */
void push_state(int state);

/*! @fn void pop_state()
 *  @brief YACC parser method.
 *  @details YACC parser method.
 *  @retval void
 */
void pop_state();

/*! @fn char* extract_const_string(const char* text)
 *  @brief Returns a string without quotes.
 *  @details Returns a string without quotes, i.e.
 *  		returns from a string of the form <tt>\"string\"</tt> a string
 *  		of the form <tt>string</tt>.
 *	@param[in] text Pointer to a char object (string to be converted).
 *  @retval char* Pointer to a char object (converted string without quotes).
 */
char * extract_const_string(const char * text);

%}

/* Lexer rules adapted from Galax */

/* Whitespace */

space           [ \t\r\n]
whitespace      {space}*
whitespaceplus  {space}+
WS              {whitespace}

/* XML character sets */

base_char       [a-zA-Z]
extender       '\183'
digit          [0-9]

/* XML names */

letter          {base_char}
nmstart         {letter}|'_'
nmchar          {letter}|{extender}|{digit}|"."|"-"|"_"

ncname          {nmstart}{nmchar}*
qname           {ncname}
varname         {qname}

/* Literals */

CharRef                   "&#"[0-9]+";"|"&#x"[0-9a-fA-F]+";"

PredefinedEntityRef       "&"("lt"|"gt"|"amp"|"quot"|"apos")";"

StringLiteral	   	("\""({PredefinedEntityRef}|{CharRef}|("\"\"")|[^"&])*"\"")|("'"({PredefinedEntityRef}|{CharRef}|("''")|[^'&])*"'")

NumericLiteral      [0-9]+(\.[0-9]+)?

UnterminatedString      (\"|"''")[^\"\n]*$

elementcontent     ({CharRef}|{PredefinedEntityRef}|[^{}<&]|"{{"|"}}")*


%x COMMENT OPERATOR S_VARNAME START_TAG ELEMENT_CONTENT END_TAG
%x QUOT_ATTRIBUTE_CONTENT APOS_ATTRIBUTE_CONTENT

%%

"fn:doc"    {
              return DOC;
            }

"fn:not"    {
              return NOT;
            }

"fn:exists" {
              return EXISTS;
            }
            
"fn:empty"  {
              return EMPTY;
            }            

"fn:true()"|"true()"    {
                          BEGIN(OPERATOR);
                          return XTRUE;
                        }

"fn:false()"|"false()"    {
                            BEGIN(OPERATOR);
                            return XFALSE;
                          }

"fn:sum"    {
              return SUM;
            }
		
"fn:avg"    {
              return AVG;
            }
				
"fn:min"    {
              return MIN;
            }
				
"fn:max"    {
              return MAX;
            }
				
"fn:count"    {
                return COUNT;
              }
				
"fn:stddev_samp"    {
                      return STDDEV_SAMP;
                    }
			
"fn:stddev_pop"    {
                     return STDDEV_POP;
                   }
	
"fn:var_samp"    {
                   return VAR_SAMP;
                 }
			
"fn:var_pop"    {
                  return VAR_POP;
                }
					
"fn:median"    {
               return MEDIAN;
             }
              
"fn:abs"    {
              return ABS;
            }
            
"fn:ceiling"    {
                  return CEILING;
                }
                
"fn:cover"    {
                return COVER;
              }
              
"fn:floor"    {
                return FLOOR;
              }
              
"fn:round"    {
                return ROUND;
              }

"fn:round-half-to-even"    {
                             return ROUNDHALFTOEVEN;
                           }
                           
"fn:truncate"    {
                   return TRUNCATE;
                 }                                                                                                                           																													

"text()"    {
              BEGIN(OPERATOR);
              return XP_TEXT;
            }

"node()"    {
              BEGIN(OPERATOR);
              return XP_NODE;
            }
            
"(:"        {
              BEGIN(COMMENT);
              comment_level++;
              return COMMENT_START;
            }

":)"        {
              yyerror("Comment Not Properly Opened");
            }                                 

{whitespaceplus}    {
                      /* ignore whitespace */
                    }

{qname}    {
             BEGIN(OPERATOR);
             yylval.str = strdup(yytext);
             return QNAME;
           }

{StringLiteral}    {
                     BEGIN(OPERATOR);
                     yylval.str = extract_const_string(yytext);
                     return STRINGCONST;
                   }

{NumericLiteral}   {
                     BEGIN(OPERATOR);
                     yylval.str = strdup(yytext);
                     return NUMERICCONST;
                   }

")"    {
         BEGIN(OPERATOR);
         return BCLOSE;
       }

"*"    {
         BEGIN(OPERATOR);
         return NT_STAR;
       }

"$"    {
         BEGIN(S_VARNAME);
         return VAR_SIGN;   
       }

"for"{whitespaceplus}"$"    {
                              /* BEGIN(S_VARNAME); */
                              unput('$');
                              return FOR;
                            }

"<"    {
         push_state(OPERATOR);
         BEGIN(START_TAG);
         return RELOP_LT;
       }

"}"    {
         pop_state();
         return CBCLOSE;
       }

"{"    {
         push_state(OPERATOR);
         return CBOPEN;       
       }

","    {
         return COMMA;
       }

"("    {
         return BOPEN;
       }

"if"{whitespace}"("    {
                         unput('(');
                         return IF;
                       }

"/"    {
         return SLASH;
       }

"child::"    {
               return CHILD;
             }

"//"    {
          return DSLASH;
        }

"descendant::"    {
                    return DESCENDANT;
                  }
<COMMENT>{
    ":)"    {  /* end of comment */
              comment_level--;             
              if (comment_level <= 0) {
                BEGIN(INITIAL);                
                yylval.str = strdup(comment.str().c_str());
                // return COMMENT_END;
                return COMMENTCONTENT;                
              } else {
                  comment << yytext;
              }
            }

    "(:"    {
    		  comment << yytext;
              comment_level++;
            }
            
    \n    {
            comment << yytext;
          }     
    
    .    {
           comment << yytext;
         }
    
    {StringLiteral}   {
                        comment << yytext;
                      }    
    
    <<EOF>>    {
                 yyerror("Comment(s) Not Properly Closed At End Of File");
               }
}

<OPERATOR>{
    "(:"   {
             BEGIN(COMMENT);
             comment_level++;
             return COMMENT_START;   
           }
           
    "{"    {
             push_state(OPERATOR);
             BEGIN(0);
             return CBOPEN;
           }

    "then"    {
                BEGIN(0);
                return THEN;
              }

    "else"    {
                BEGIN(0);
                return ELSE;
              }

    "and"    {
               BEGIN(0);
               return AND;
             }

    ","    {
             BEGIN(0);
             return COMMA;
           }

    "="    {
             BEGIN(0);
             return RELOP_EQ;
           }

    "!="    {
              BEGIN(0);
              return RELOP_NEQ;
            }

    "<"    {
             BEGIN(0);
             return RELOP_LT;
           }

    ">"    {
             BEGIN(0);
             return RELOP_GT;
           }

    "<="    {
              BEGIN(0);
              return RELOP_LEQ;
            }

    ">="    {
              BEGIN(0);
              return RELOP_GEQ;
            }

    "in"    {
              BEGIN(0);
              return IN;
            }

    "*"    {
             BEGIN(0);
             return NT_STAR;
           }

    "or"    {
              BEGIN(0);
              return OR;
            }

    "where"    {
                 BEGIN(0);
                 return WHERE;
               }

    "return"    {
                  BEGIN(0);
                  return XRETURN;
                }

    "/"    {
             BEGIN(0);
             return SLASH;
           }

    "child::"    {
                   BEGIN(0);
                   return CHILD;
                 }

    "//"    {
              BEGIN(0);
              return DSLASH;
            }

    "descendant::"    {
                        BEGIN(0);
                        return DESCENDANT;
                      }

    "}"    {
             pop_state();
             return CBCLOSE;
           }

    "$"    {
             BEGIN(S_VARNAME);
             return VAR_SIGN;
           }

    "for"{whitespaceplus}"$"    {
                                  /* BEGIN(S_VARNAME); */
                                  unput('$');
                                  return FOR;
                                }

    ")"    {
             return BCLOSE;
           }
   
    {StringLiteral}    {
                         yylval.str = extract_const_string(yytext);
                         return STRINGCONST;
                       }

    {NumericLiteral}    {
                         yylval.str = strdup(yytext);
                         return NUMERICCONST;
                       }

    {WS}    {
              /* ignore whitespace */
            }    
}

<S_VARNAME>{varname} {
                      BEGIN(OPERATOR);
                      yylval.str = strdup(yytext);
                      return VARNAME;
                     }

<START_TAG>{
    "/>"    {
              pop_state();
              return GTSLASH;
            }

    ">"    {
             BEGIN(ELEMENT_CONTENT);
             return RELOP_GT;
           }

    "\""    {
              BEGIN(QUOT_ATTRIBUTE_CONTENT);
              yyerror("Feature \"Attributes\" Not Yet Implemented");
            }

    "'"    {
             BEGIN(APOS_ATTRIBUTE_CONTENT);
             yyerror("Feature \"Attributes\" Not Yet Implemented");
           }

    "="    {
             yyerror("Feature \"Attributes\" Not Yet Implemented");
           }
 
    {WS}    {
              /* ignore whitespace */
            }

    {qname}    {
                 yylval.str = strdup(yytext);
                 return QNAME;
               }          
}

<ELEMENT_CONTENT>{
    "</"    {
              BEGIN(END_TAG);
              return LTSLASH;
            }

    "{"    {
             push_state(ELEMENT_CONTENT);
             BEGIN(0);
             return CBOPEN;
           }

    "<"    {
             push_state(ELEMENT_CONTENT);
             BEGIN(START_TAG);
             return RELOP_LT;
           }

    {WS}    {
              /* ignore whitespace */
            }

    {elementcontent}    {
                          char * newstr = new char[strlen(yytext) + 1];
                          unsigned lastpos = 0;
                          bool last_skipped = false;

                          for(unsigned i=0; i<strlen(yytext); i++) {
                            if (i==0) {
                              newstr[0] = yytext[0];
                              lastpos = 0;
                            } else {
                              if(!((yytext[i-1]=='{' && yytext[i]=='{') || (yytext[i-1]=='}' && yytext[i]=='}')) || last_skipped) {
                                newstr[++lastpos] = yytext[i];
                                last_skipped = false;
                              } else {
                                last_skipped = true;
                              }
                            }
                          }
                          newstr[lastpos+1] = '\0';

                          yylval.str = newstr;
                          return ELEMENTCONTENT;
                        }               
}

<END_TAG>{
    ">"    {
             pop_state();
             return RELOP_GT;
           }

    {WS}    {
              /* ignore whitespace */
            }

    {qname}    {
                 yylval.str = strdup(yytext);
                 return QNAME;
               }      
}

<*>.    {
          /* report errors */
          yyerror("Call Error");   
        }

<*>{UnterminatedString}    {
                             yyerror("Unterminated String");
                           }

%%

/*! @fn int yywrap(void)
 *  @brief YACC parser method.
 *  @details YACC parser method.
 *  @retval int
 */
int yywrap(void) {
  return 1;
}

/*! @fn void pop_state()
 *  @brief YACC parser method.
 *  @details YACC parser method.
 *  @retval void
 */
void pop_state() {
  int resume_state = stack.back();
  stack.pop_back();
  BEGIN(resume_state);
}

/*! @fn void push_state(int state)
 *  @brief YACC parser method.
 *  @details YACC parser method.
 *  @param[in] state State.
 *  @retval void
 */
void push_state(int state) {
  stack.push_back(state);
}

#include <iostream>

/*! @fn char* extract_const_string(const char* text)
 *  @brief Returns a string without quotes.
 *  @details Returns a string without quotes, i.e.
 *  		returns from a string of the form <tt>\"string\"</tt> a string
 *  		of the form <tt>string</tt>.
 *	@param[in] text Pointer to a char object (string to be converted).
 *  @retval char* Pointer to a char object (converted string without quotes).
 */
char * extract_const_string(const char * text) {
  char * str  = strncpy(new char[strlen(text) - 1 ], yytext+1, strlen(text) - 2);
  str[strlen(text)-2] = '\0';

  return str;
}

/*! @fn void yyerror(const char* text)
 *  @brief YACC parser method.
 *  @details YACC parser method for printing errors.
 *  @param[in] text Pointer to a char object (error message to be printed).
 *  @retval void
 */
void yyerror(const char *text) {
  std::ostringstream o;
  o << "Query Parsing In Line " << yylineno << " => "  << text;
  if (yytext && (int)yytext!=EOF && strcmp(yytext, "")!=0) {
    o << " (\"" << yytext << "\")";
  }
  throw ParseException(o.str().c_str(), eid_parse_query);
}
