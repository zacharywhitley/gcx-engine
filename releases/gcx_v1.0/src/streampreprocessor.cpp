/*
 | Author: Michael Schmidt
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
#include "streampreprocessor.h"
#include "intmultiset.h"
#include "intsingleset.h"
#include "error.h"
#include <sstream>

StreamPreProcessor* StreamPreProcessor::instance=NULL;

void StreamPreProcessor::initInstance(Buffer* _buffer,
                                        const char* _yyxmlin,
                                        ProjectionDFA* _p_dfa) {
    instance=new StreamPreProcessor(_buffer,
									_yyxmlin,
									_p_dfa);
}

StreamPreProcessor* StreamPreProcessor::getInstance() {
    return instance; // should have been initialized before
}

StreamPreProcessor::StreamPreProcessor(Buffer* _buffer,
                                        const char* _yyxmlin,
                                        ProjectionDFA* _p_dfa)
	: tagmap(TagMap::getInstance()), st(_p_dfa,_buffer) {

    yyxmlin = fopen(_yyxmlin,"r");
    if (yyxmlin == 0) {
		std::ostringstream s;
		s << "Can not open file '" << yyxmlin << "'";
		Error::getInstance()->throwError(s.str().c_str());
    }
}

StreamPreProcessor::~StreamPreProcessor() {
	if (data!=NULL)
		free((char*)data);
}

void StreamPreProcessor::processTag(const char* _tag) {
    //cout <<  "READING TAG <" << _tag << ">" << endl;
	st.processOpeningTag(_tag);
	//cout << "PROCESSED OPENING TAG" << endl;
}

void StreamPreProcessor::processPCData(const char* _data) {
    //cout << "READING DATA '" << _data << "'" << endl;
	st.processPCData(_data);
	//cout << "PROCESSED DATA" << endl;
}

void StreamPreProcessor::closeTag() {
    //cout << "READING CLOSING TAG" << endl;
	st.processClosingTag();
	//cout << "PROCESSED CLOSING TAG" << endl;
}

void StreamPreProcessor::readNext() {
	//cout << "begin readNext()" << endl;
    if (!saxparse()) {
		st.processClosingTag();
	}
	//cout << "end readNext()" << endl;
}

void StreamPreProcessor::readAll() {
	while (saxparse()) {
	}
	st.processClosingTag(); // close the root
}
