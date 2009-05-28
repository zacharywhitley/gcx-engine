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
#ifndef BUFFERNODE_H
#define BUFFERNODE_H

#define TYPE_TAG		0
#define TYPE_PCDATA		1
#define TYPE_CLOB		2

#include "tagmap.h"
#include <vector>
#include <iostream>
#include "buffernode.h"
#include "buffernodelist.h"

class PathStepExpression;
class PathExpression;

using namespace std;

// fw declaration
struct BufferNode;


struct BufferNode {

	// this is the CLOB buffer node constructor
	#ifdef USE_CLOBS
    BufferNode(BufferNode* _parent);
	#endif // #ifdef USE_CLOB
    BufferNode(const char* data,BufferNode* _parent);
    BufferNode(TAG tag,BufferNode* _parent);
	
    virtual ~BufferNode();

	// data carried by the node   
    char type; 

	BufferNode* parent;
	BufferNode* r_sibling;

	// misc methods 
    bool isRoot();
	void addChild(BufferNode* child);
	void close();
	bool isClosed();
	
	// search
	void matchingNodes(PathExpression* p,
						vector<BufferNode*>* matches);
	const char* getChildPCData();
	const char* getPCDataRepresentation();
	void getPCDataRepresentation(std::ostringstream& s);

	// cleanup
	void lock();
	void unlock();
	void distributeIndexUpdate(unsigned id);
	void clear();
	void clearSubtree();
	
	// marking
	bool isMarked();
	bool isCumulativeMarked();
	bool hasMarkedDescendant();
	bool hasCumulativeMarkedAncestor();
	
	// output
	void countNodes(unsigned& nodes,
					unsigned& tag,
					unsigned& pcdata);
	void debugPrint();
	void printNoSubnodes();
	void print();
	
    // a buffer node is either a tag node or a character data node
	// or a CLOB node (if used)
    // (the ROOT-node is represented by a tag node)
    void* node; // either of type TagNode or of type CharNode
    
};

#endif // BUFFERNODE_H
