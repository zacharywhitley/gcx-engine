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
#include "bufferstdchilditerator.h"
#include "streampreprocessor.h"
#include "tagnode.h"


BufferStdChildIterator::BufferStdChildIterator(BufferNode* base,
										unsigned short _iterator_type) 
	: BufferIterator(base), iterator_type(_iterator_type), initial(true) {
}

BufferStdChildIterator::~BufferStdChildIterator() {}

BufferNode* BufferStdChildIterator::next() {
	
	if (cur==NULL)
		return NULL;

	if (initial) {
		initial=false;
		if (!setFirstChild()) return NULL;

		// if the first child matches, we return it
		if (iterator_type==ITERATOR_TYPE_NODE ||
			(iterator_type==ITERATOR_TYPE_TEXT && cur->type==TYPE_PCDATA)) {
			return cur;
		}
	}

	computeNextMatchingNode(); // case no success
	return cur;
}

bool BufferStdChildIterator::setFirstChild() {

	while (((TagNode*)base->node)->children.isEmpty() && !base->isClosed()) {
		spp->readNext();
	}

	if (!((TagNode*)base->node)->children.isEmpty()) {
		cur=((TagNode*)base->node)->children[0];
		return true; // success
	}

	// no (projected) child exists
	cur=NULL;
	return false;
}

void BufferStdChildIterator::computeNextMatchingNode() {
	
	while (cur->r_sibling!=NULL || !base->isClosed()) {

		if (cur->r_sibling!=NULL) {
			
			// set next sibling ...
			cur=cur->r_sibling;
			// ... and check for condition
			if (iterator_type==ITERATOR_TYPE_NODE ||
				(iterator_type==ITERATOR_TYPE_TEXT && 
				 cur->type==TYPE_PCDATA))
				return; // report match
		} else {
			spp->readNext();
		}
	}

	cur=NULL;
}
