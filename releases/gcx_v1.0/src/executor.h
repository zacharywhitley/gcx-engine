/*
 | Author: Michael Schmidt, modified by Steffi Scherzinger
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
#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "stdio.h"
#include <iostream>
#include <unistd.h>
#include "pathstepexpression.h"
#include "pathenvironment.h"
#include "buffer.h"
#include "streampreprocessor.h"
#include "environment.h"
#include "fsamap.h"
#include "cmdline.h"
#include "projectiontree.h"
#include "projectiondfa.h"
#include "projectionnfa.h"





using namespace std;

class Executor {

public:
    Executor(const char* _query_file,const char* _xml_file,
            CmdLine* _cmd);
    virtual ~Executor();

    void start();

private:
    
    void parseQuery();
    void optimizeQuery();
	void collectQueryInformation();
    void buildProjectionTree();
	void buildProjectionNFA();
	void buildInitialProjectionDFA();
    void initStreamParser();
    void rewriteQuery();
    void insertUpdates(vector<UpdateExpression*>& updates);
    void evalQuery();
	void preprocessStream();
	void finalDebugPrint(bool sppmode);
	
    const char* query_file;
    const char* xml_file;

    Expression* query;
    Buffer* buffer;
    FSAMap* fsamap;
	PathEnvironment* penv;
    ProjectionTree* pt;
    ProjectionNFA* p_nfa;
    ProjectionDFA* p_dfa;

    CmdLine* cmd;

};

#endif // EXECUTOR_H
