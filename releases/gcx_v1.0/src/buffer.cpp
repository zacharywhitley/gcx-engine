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
#include "buffer.h"
#include "tagnode.h"
#include "clobnode.h"


Buffer::Buffer(ProjectionDFA* p_dfa) {
    root=new BufferNode((TAG)0,NULL);
    cur=root;

	ProjectionDFAState* is=p_dfa->getInitialState();
	appendIndices(is->getCumulativeIndices(),
					is->getNoncumulativeIndices());
}

Buffer::~Buffer() {
    delete root;
}

void Buffer::appendTag(TAG tag) {
#ifdef USE_CLOBS
if (cur->type==TYPE_CLOB)
	cur=cur->parent;
#endif
	
    BufferNode* t=new BufferNode(tag,cur);
    cur->addChild(t);
    cur=t;
}

void Buffer::appendPCData(const char* data) {
#ifdef USE_CLOBS
if (cur->type==TYPE_CLOB)
	cur=cur->parent;
#endif
    BufferNode* t=new BufferNode(data,cur);
    cur->addChild(t);
}

void Buffer::closeTag() {
#ifdef USE_CLOBS
if (cur->type==TYPE_CLOB)
  {
    ((CLOBNode*)(cur->node))->finishCLOB();
    cur=cur->parent;
  }
#endif
    cur->close();
	cur=cur->parent;
}

void Buffer::print() {
    root->print();
}

void Buffer::printCurrent() {
    cur->print();
}

void Buffer::debugPrint() {
    root->debugPrint();
}

void Buffer::appendIndices(vector<unsigned>* _cumulative_indices,
							vector<unsigned>* _noncumulative_indices) {

	for (unsigned i=0; i<_cumulative_indices->size(); i++) {
		((TagNode*)cur->node)->cumulative_indices.push_back((*_cumulative_indices)[i]);
	}
	for (unsigned i=0; i<_noncumulative_indices->size(); i++) {
		((TagNode*)cur->node)->noncumulative_indices.push_back((*_noncumulative_indices)[i]);
	}
}

#ifdef USE_CLOBS


void Buffer::closeCLOB(TAG tag) {
		
	if (cur->type!=TYPE_CLOB) {
		BufferNode* t=new BufferNode(cur);
		cur->addChild(t);
		cur=t;
	} 

	// and add CLOB data
	((CLOBNode*)(cur->node))->closeCLOB(tag);
	
}

void Buffer::appendCLOB(TAG tag) {
		
	if (cur->type!=TYPE_CLOB) {
		BufferNode* t=new BufferNode(cur);
		cur->addChild(t);
		cur=t;
	} 

	// and add CLOB data
	((CLOBNode*)(cur->node))->appendCLOB(tag);
}

void Buffer::appendCLOB(const char* data) {
	
	if (cur->type!=TYPE_CLOB) {
		BufferNode* t=new BufferNode(cur);
		cur->addChild(t);
		cur=t;
	}

	// and add CLOB data
	((CLOBNode*)(cur->node))->appendCLOB(data);

}
#endif // #ifdef USE_CLOBS
