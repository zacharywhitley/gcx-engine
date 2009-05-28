/*
 | Author: Michael Schmidt;
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
 * 	@brief Implementations of header file streampreprocessor.h.
 * 	@details Implementations of constructors, destructor and functions of the corresponding header file streampreprocessor.h.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#include "streampreprocessor.h"

StreamPreProcessor* StreamPreProcessor::instance=NULL;

void StreamPreProcessor::initInstance(Buffer* _buffer, const char* _xml_file,
                                      ProjectionDFA* _pdfa) {
    instance=new StreamPreProcessor(_buffer, _xml_file, _pdfa);
}

StreamPreProcessor* StreamPreProcessor::getInstance() {
    return instance;
}

StreamPreProcessor::StreamPreProcessor(Buffer* _buffer, const char* _xml_file,
                                       ProjectionDFA* _pdfa) :
tagmap(TagMap::getInstance()), st(_pdfa, _buffer) {
    yyxmlin = fopen(_xml_file, "r");
    if (yyxmlin==0) {
        std::ostringstream s;
        s << "XML File Error: Unable To Open File \"" << _xml_file << "\".";
        Error::getInstance()->throwError(s.str().c_str());
    }
}

StreamPreProcessor::~StreamPreProcessor() {
	fclose(yyxmlin);
	destroyLexer();
}

void StreamPreProcessor::readNext() {
    if (!saxparse()) {
		#ifdef VALIDATION
        st.processClosingTag((TAG)TAGID_ROOT); // close the root 
		#else
        st.processClosingTag(); // close the root 
		#endif // #ifdef VALIDATION
    }
}

void StreamPreProcessor::readAll() {
    while (saxparse()) {}

	#ifdef VALIDATION
    st.processClosingTag((TAG)TAGID_ROOT); // close the root
	#else
    st.processClosingTag(); // close the root
	#endif // #ifdef VALIDATION
}
