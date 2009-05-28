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
 *  @brief LEXER.
 *  @details LEXER.
 *  @author Stefanie Scherzinger
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 *  @license Software License Agreement (BSD License)
 */
%option yylineno

%{

using namespace std;

#include <sstream>
#include <vector>
#include <list>
#include "typeenums.h"
#include "error.h"

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

typedef list< pair<VarExpression*, Expression*> > var_list_type;

#include "query_parser.h"

extern const char * xquery_file;

void yyerror(const char *);

/* own stack for maintaining lexer start conditions */
/* (different semantics from yy_push_state); */
vector<int> stack;

void push_state(int state);
void pop_state();

/* transform "string" to string */
char * extract_const_string(const char * text);

/* error message if a feature not implemented */
void feature_not_implemented(const char * text);

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

/* String Literals */

CharRef                   "&#"[0-9]+";"|"&#x"[0-9a-fA-F]+";"
PredefinedEntityRef       "&"("lt"|"gt"|"amp"|"quot"|"apos")";"

StringLiteral 	   	("\""({PredefinedEntityRef}|{CharRef}|("\"\"")|[^"&])*"\"")|("'"({PredefinedEntityRef}|{CharRef}|("''")|[^'&])*"'")

UnterminatedString      (\"|"''")[^\"\n]*$

elementcontent     ({CharRef}|{PredefinedEntityRef}|[^{}<&]|"{{"|"}}")*

%x OPERATOR S_VARNAME START_TAG ELEMENT_CONTENT END_TAG
%x QUOT_ATTRIBUTE_CONTENT APOS_ATTRIBUTE_CONTENT

%%

"fn:not"    {
              return NOT;
            }

"fn:exists" {
              return EXISTS;
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
					
"fn:list"    {
               return LIST;
             }																														

"text()"    {
              BEGIN(OPERATOR);
              return XP_TEXT;
            }

"node()"    {
              BEGIN(OPERATOR);
              return XP_NODE;
            }

{whitespaceplus}    {
                    }

{qname}    {
             BEGIN(OPERATOR);
             yylval.str = strdup(yytext);
             return QNAME;
           }

{StringLiteral}    {
                     BEGIN(OPERATOR);
                     yylval.str = extract_const_string(yytext);
                     return CONSTSTRING;
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

<OPERATOR>{
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
                         return CONSTSTRING;
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
              feature_not_implemented("attributes");
            }

    "'"    {
             BEGIN(APOS_ATTRIBUTE_CONTENT);
             feature_not_implemented("attributes");
           }

    "="    {
             feature_not_implemented("attributes");
           }
 
    {WS}    {
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

int yywrap(void) {
  return 1;
}

void pop_state() {
  /* if (! stack.empty()) */
  /* yyerror("Syntax Error"); */

  int resume_state = stack.back();
  stack.pop_back();
  BEGIN(resume_state);
}

void push_state(int state) {
  stack.push_back(state);
}

char * extract_const_string(const char * text) {
  char * str  = strncpy(new char[strlen(text) - 1 ], yytext+1, strlen(text) - 2);
  str[strlen(text)-2] = '\0';

  return str;
}

void yyerror(const char * text) {
  std::ostringstream o;
  o << "Query File \"" << xquery_file << "\" Error: In Line " << yylineno << " => "  << text
       << " (\"" << yytext << "\")." << endl;
  Error::getInstance()->throwError(o.str().c_str());
}

void feature_not_implemented(const char * text) {
  std::ostringstream o;
  o << "Query File \"" << xquery_file << "\" Error: In Line " << yylineno << " => Feature \""
       << text << "\" Not Yet Implemented.";
  Error::getInstance()->throwError(o.str().c_str());
}
