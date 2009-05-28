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
 
#include<iostream>

void yyerror(const char *);

void saxBeginElement(const char* _tag);
void saxEndElement();
void saxCharacters(const char* _text);

%}

%x TAG

%%


\<[\!\?][^\>]+\>					{}

[^\<]+								{
		        					saxCharacters(yytext);
									return 1;
		        					}
			
\<[A-Za-z\_][A-Za-z0-9\_\-\.]* 		{
									saxBeginElement(yytext + 1);
                        			BEGIN(TAG);
                        			}
			
\<\/[A-Za-z\_][A-Za-z0-9\_\-\.]*\> 	{
                        			saxEndElement();
									return 1;
                        			}
			
<TAG>\/\>							{
									saxEndElement();
									BEGIN(INITIAL);
									return 1;
            						}
			
<TAG>\>								{
									BEGIN(INITIAL);
									return 1;
            						}
			
<TAG>.		{}

<TAG>[ \t\n]	{}

.			{ cerr << "Misparsed \"" << yytext << "\"" << endl; }



%%


int yywrap(void) { return 1; }

void yyerror(const char *text)
{
   cout << "XML parse error: " << text << endl;
   exit(1);
}


