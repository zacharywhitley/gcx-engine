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
#ifndef BUFFER_H
#define BUFFER_H

#include <iostream>
#include "buffernode.h"
#include "projectiondfa.h"

class Buffer {

public:
    Buffer(ProjectionDFA* p_dfa);
    virtual ~Buffer();

    inline BufferNode* getRoot() { return root; }
    inline BufferNode* getCurrent() { return cur; }

	// standard buffer appending
    void appendTag(TAG tag);
    void appendPCData(const char* data);
    void closeTag();

	// CLOB buffer appending
	#ifdef USE_CLOBS
	void closeCLOB(TAG tag);
	void appendCLOB(TAG tag);
	void appendCLOB(const char* data);
	#endif // #ifdef USE_CLOBS

	// index manipulation
	void appendIndices(vector<unsigned>* _cumulative_indices,
						vector<unsigned>* _non_cumulative_indices);

    void print();
    void printCurrent();
    void debugPrint();

private:
    BufferNode* root;
	BufferNode* cur;

};

#endif // BUFFER_H
