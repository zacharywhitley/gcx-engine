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
#include "clobnode.h"
#include <string>
  
#ifdef GENERATE_STATS
#include "statistics.h"
#endif // GENERATE_STATS

CLOBNode::CLOBNode() :  s(NULL) {

        strstr = new stringstream();
	
	#ifdef GENERATE_STATS
	Statistics::getInstance()->addClobNode();	
	#endif // GENERATE_STATS

}

CLOBNode::~CLOBNode() {
	if (s)
	  delete s;

	if (strstr)
	  delete strstr;
	
	#ifdef GENERATE_STATS
	Statistics::getInstance()->removeClobNode();	
	#endif // GENERATE_STATS
}

void CLOBNode::appendCLOB(TAG tag) {

	(*strstr) <<  "<" << tag << "<"; // opening tag decoding=<#TAG<

}

void CLOBNode::appendCLOB(const char* data) {

	(*strstr) << data;

}

void CLOBNode::closeCLOB(TAG tag) {

	(*strstr) <<  "<" << tag << ">"; // closing tag decoding <#TAG>
}


void CLOBNode::finishCLOB() {
  
  //cout << "\n finish clob" << (void *) this << endl;

	s=new char[strlen(strstr->str().c_str())+1];
	strcpy(s,(strstr->str()).c_str());
	

	delete strstr;
	strstr = NULL;
}

void CLOBNode::print() {

  //  finishCLOB();

	TagMap* tagmap=TagMap::getInstance();
		
	unsigned length=strlen(s);
	bool cascaded=false;
	for (unsigned i=0; i<length; i++) {

		// we read an opening tag
		if (s[i]=='<' && !cascaded) {
				
			cout << "<"; 
			ostringstream oss;
			while (s[++i]!='<' && s[i]!='>') {
				oss << s[i];
			}
			if (s[i]=='>')
				cout << "/";
			cout << tagmap->getTag(atoi(oss.str().c_str()));
			cout << ">";
		
		// a backslash may be used to cascade the '<' character,
		// it should be ignored in this case (this is, simply output)
		} else if (s[i]=='\\') {
			cascaded=!cascaded;
			cout << s[i];
		} else {
			if (cascaded)
				cascaded=false;
			cout << s[i];
		}
	}
}
