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
#include "tagmap.h"
#include "buffernodestack.h"
#include "bufferdescendantiterator.h"
#include "tagnode.h"

BufferDescendantIterator::BufferDescendantIterator(BufferNode* _base,
													TAG _nodetest)
    : BufferIterator(_base,_nodetest), lastmatch(NULL) {

	stack.push(base);
    /*
    cout << endl << "CREATING NEW DESCENDANT ITERATOR FROM ";
    base->debugPrint();
    cout << endl;
    */
}

BufferDescendantIterator::~BufferDescendantIterator() {}
    
BufferNode* BufferDescendantIterator::next() {

    if (cur==NULL)
        return NULL;

	BufferNode* start_search=NULL;
	BufferNode* cur_base=stack.top();

	while (cur_base) {

		bool matched=getNextMatch(cur_base,start_search);
		if (matched) {
			return cur;
		} else {
			start_search=stack.top();
			stack.pop(); // climb up
			cur_base=stack.top();
		}
	}

	cur=NULL;
	return cur;
}
	
bool BufferDescendantIterator::getNextMatch(BufferNode* _cur,
											BufferNode* start_with_child) {

	TagNode* t=(TagNode*)_cur->node;

	bool start=start_with_child==NULL;

	// read an initial child if any
	while ((int)t->children.isEmpty() && !(_cur->isClosed())) {
		spp->readNext();
	}

    for (BufferNode* it=t->children.getFront(); it; it=it->r_sibling) {

		if (!start) {
			if (it==start_with_child)
				start=true;
		} else {

			BufferNode* cur_child=it;
			if (cur_child->type==TYPE_TAG) {
				stack.push(cur_child);
				if (((TagNode*)cur_child->node)->tag==nodetest 
							|| nodetest==TAGID_STAR) {
					cur=cur_child;
					lastmatch=cur;
					return true;
				}
	
				// try to match a descendant
				if (getNextMatch(cur_child,NULL))
					return true;

				stack.pop();
			}
		}

		while (it->r_sibling==NULL && !(_cur->isClosed())) {
			spp->readNext();
		}
	}

	return false; // no success
}
