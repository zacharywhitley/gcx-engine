/*
 | Author: Michael Schmidt; modified by Steffi Scherzinger;
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
 *  @brief SAX-PARSER.
 *  @details SAX-PARSER.
 *  @author Michael Schmidt
 *  @author Stefanie Scherzinger
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
%option yylineno
 
%{

//! @namespace std
using namespace std;
 
#include <sstream>
#include "parseexception.h"

/*! @fn void yyerror(const char* text)
 *  @brief Error reporting with output.
 *  @details Error reporting with output.
 *  @param[in] _text Error message.
 *  @retval void
 */
void yyerror(const char *text);

/*! @fn void saxCharacters(const char* _text)
 *  @brief PCDATA processing.
 *  @details PCDATA processing.
 *  @param[in] _text The PCDATA content.
 *  @retval void
 */
void saxCharacters(const char* _text);

/*! @fn void saxBeginElement(const char* _tag)
 *  @brief Opening tag processing.
 *  @details Opening tag processing.
 *  @param[in] _tag The opening tag.
 *  @retval void
 */
void saxBeginElement(const char* _tag);
#ifdef VALIDATION
/*! @fn void saxEndElement(const char* _tag)
 *  @brief Closing tag processing.
 *  @details Closing tag processing.
 *  @param[in] _tag The closing tag.
 *  @retval void
 */
void saxEndElement(const char* _tag);
#else
/*! @fn void saxEndElement()
 *  @brief Closing tag processing.
 *  @details Closing tag processing.
 *  @retval void
 */
void saxEndElement();
#endif // #ifdef VALIDATION
/*! @fn void saxBeginEndElement(const char* _tag)
 *  @brief Bachelor tag processing.
 *  @details Bachelor tag processing
 *  @param[in] _tag The bachelor tag.
 *  @retval void
 */
void saxBeginEndElement(const char* _tag);

%}

%%

\<[\!\?][^\>]+\>                    {}

[^\<]+                              {
                                      saxCharacters(yytext);
                                      return 1;
                                    }

\<[A-Za-z\_][A-Za-z0-9\_\-\.]*\>    {
									  saxBeginElement(yytext);
									  return 1;
                                    }

\<\/[A-Za-z\_][A-Za-z0-9\_\-\.]*\>  {
									  #ifdef VALIDATION
                                      saxEndElement(yytext);
									  #else
                                      saxEndElement();
									  #endif // #ifdef VALIDATION
                                      return 1;
                                    }

\<[A-Za-z\_][A-Za-z0-9\_\-\.]*\/>  	{
									  saxBeginEndElement(yytext);
                                      return 1;
                                    }

.                                   {
                                      yyerror("Misparsed Charcter");
                                    }

%%

/*! @fn int yywrap(void)
 *  @brief YACC Parser method invoked after reading the whole input stream.
 *  @details YACC Parser method invoked after reading the whole input stream.
 *  @retval int Return code.
 */
int yywrap(void) { return 1; }

/*! @fn void yyerror(const char* text)
 *  @brief Error reporting with output.
 *  @details Error reporting with output.
 *  @param[in] _text Error message.
 *  @retval void
 */
void yyerror(const char *text) {
  std::ostringstream o;
  o << "XML Parsing In Line " << yylineno << " => " << text;
  if (yytext && (int)yytext!=EOF && strcmp(yytext, "")!=0) {
    o << " (\"" << yytext << "\")";    
  }
  throw ParseException(o.str().c_str(), eid_parse_xml); 
}
