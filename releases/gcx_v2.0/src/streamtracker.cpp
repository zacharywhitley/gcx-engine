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
 | * Redistributions of source code must retain the abovef
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
 * 	@brief Implementations of header file streamtracker.h.
 * 	@details Implementations of constructors, destructor and functions of the corresponding header file streamtracker.h.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#include "streamtracker.h"

StreamTracker::StreamTracker(ProjectionDFA* _pdfa, Buffer* _buffer) :
cur(_pdfa->getInitialState()), buffer(_buffer), tagmap(TagMap::getInstance()), 
skip_subtree_depth(0), keep_subtree_depth(0) {}

StreamTracker::~StreamTracker() {}

void StreamTracker::processOpeningTag(const char* _tag) {

	if (skip_subtree_depth>0) {
		skip_subtree_depth++;
	} else if (keep_subtree_depth>0) {
        buffer->appendTag(tagmap->insertTagWithOffset(_tag,1,1));
		keep_subtree_depth++;
	} else {
		TAG tag=tagmap->insertTagWithOffset(_tag,1,1);
		short success=TRANSITION_UNKNOWN;
		cur=cur->takeTransition(tag,success);
		
		switch (success) {
			case TRANSITION_REGULAR:
				if (cur->keepNode()) {
	                buffer->appendTag(tag,cur->getCumulativeRoles(),
    	                                cur->getNonCumulativeRoles());
				}
				break;

			case TRANSITION_SKIP_SUBTREE:
				skip_subtree_depth++;
				break;

			case TRANSITION_KEEP_SUBTREE:
				buffer->appendTag(tag);
				keep_subtree_depth++;
				break;
		}
	}
}

void StreamTracker::processBachelorTag(const char* _tag) {

	if (skip_subtree_depth>0) {
		// nothing to do here
	} else if (keep_subtree_depth>0) {
        buffer->appendTag(tagmap->insertTagWithOffset(_tag,1,2));
		buffer->closeTag();
	} else {
		// keep current transition
		TAG tag=tagmap->insertTagWithOffset(_tag,1,2);
		short success=TRANSITION_UNKNOWN;
		cur=cur->takeTransition(tag,success);
		
		switch (success) {
			case TRANSITION_REGULAR:
				if (cur->keepNode()) {
	                buffer->appendTag(tag,cur->getCumulativeRoles(),
    	                                cur->getNonCumulativeRoles());
					buffer->closeTag();
				}
				cur=cur->traceBack();
				break;

			case TRANSITION_SKIP_SUBTREE:
				break;

			case TRANSITION_KEEP_SUBTREE:
				buffer->appendTag(tag);
				buffer->closeTag();
				break;
		}
	}
}

#ifdef VALIDATION
// this is exactly the same as above; for speed reasons we keep
// two versions, to avoid unnecessary checks and fully exploit
// skip_subtree and keep_subtree performance advantages
void StreamTracker::processClosingTag(const char* _tag) {
	if (skip_subtree_depth>0) {
		skip_subtree_depth--;
	} else if (keep_subtree_depth>0) {
        buffer->closeTag();
		keep_subtree_depth--;
	} else {
		TAG tag=TagMap::getInstance()->insertTagWithOffset(_tag,2,1);
		if (cur->keepNode()) {
			if (buffer->getCurrent() &&
				buffer->getCurrent()->type==TYPE_TAG &&
				((TagNode*)buffer->getCurrent()->node)->tag==tag) {
	       		buffer->closeTag();
			} else {
				Error::getInstance()->throwError("XML Parsing Error: XML Document Not Well-Formed.");
			}
		}
	    cur=cur->traceBack();
	}
}

void StreamTracker::processClosingTag(TAG tag) {
	if (skip_subtree_depth>0) {
		skip_subtree_depth--;
	} else if (keep_subtree_depth>0) {
        buffer->closeTag();
		keep_subtree_depth--;
	} else {
		if (cur->keepNode()) {
			if (buffer->getCurrent()->type==TYPE_TAG &&
				((TagNode*)buffer->getCurrent()->node)->tag==tag) {
	       		buffer->closeTag();
			} else {
				Error::getInstance()->throwError("XML Parsing Error: XML Document Not Well-Formed.");
			}
		}
	    cur=cur->traceBack();
    }
}
#else
void StreamTracker::processClosingTag() {
	if (skip_subtree_depth>0) {
		skip_subtree_depth--;
	} else if (keep_subtree_depth>0) {
        buffer->closeTag();
		keep_subtree_depth--;
	} else {
		if (cur->keepNode()) {
			buffer->closeTag();
		}
	    cur=cur->traceBack();
	}
}
#endif // #ifdef VALIDATION

void StreamTracker::processPCData(const char* data) {    

	if (keep_subtree_depth>0) {
        buffer->appendPCData(data);
	} else if (skip_subtree_depth>0) {
		// ignore
	} else {
		ProjectionDFAState* text_state=cur->takeTextTransition();
		if (text_state) {
			buffer->appendPCData(data,text_state->getCumulativeRoles(),
	                                text_state->getNonCumulativeRoles());
			// we did not change cur, so trace back is implicit
		}
	} // else ignore text node
}
