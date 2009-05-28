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
#include "varname.h"
#include <sstream>
#include <iostream>

#include "expenums.h"

VarName* VarName::instance=NULL;

VarName* VarName::getInstance() {
    if (instance==NULL) {
        instance=new VarName();
    }
    return instance;
}

VarName::VarName() 
	: index(1) {
  char* root=new char[strlen(ROOTVAR)+1];
	strcpy(root,ROOTVAR);
    varnames.push_back(root);
}

VarName::~VarName() {
    for (unsigned i=0; i<varnames.size(); i++) {
        delete[] varnames[i];
    }
}

unsigned VarName::addVar(const char* varname,bool force) {

	if (!force) {
	    for (unsigned i=0; i<varnames.size(); i++) {
	        if (strcmp(varname,varnames[i])==0) {
	            return i;
	        }
	    }
	}

	char* var_cpy=new char[strlen(varname)+1];
	strncpy(var_cpy,varname,strlen(varname));
	var_cpy[strlen(varname)]='\0';
	
    varnames.push_back(var_cpy);
    return varnames.size()-1;
}

const char* VarName::getVar(unsigned id) {
    return varnames[id];
}

const char* VarName::getFreshVarname() {
	std::ostringstream o;
	o << "_" << index++; // << index;
	//char* x=new char[strlen(o.str().c_str())+1];
	char* cpy=new char[strlen(o.str().c_str())+1];
	strcpy(cpy,o.str().c_str());
	return cpy;
}
