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
#include "tagnode.h"
#include "buffernode.h"

#ifdef GENERATE_STATS
#include "statistics.h"
#endif // GENERATE_STATS

////////////// TagNode
TagNode::TagNode(TAG _tag)
        : tag(_tag), is_closed(false), is_locked(false) {
		
	#ifdef GENERATE_STATS
	Statistics::getInstance()->addTagNode();
	#endif // GENERATE_STATS
}

TagNode::~TagNode() {

	#ifdef GENERATE_STATS
	Statistics::getInstance()->removeTagNode();
	#endif // GENERATE_STATS
}

bool TagNode::isMarked() {
	return cumulative_indices.size()>0 || noncumulative_indices.size()>0;
}

bool TagNode::isCumulativeMarked() {
	return cumulative_indices.size()>0;
}

bool TagNode::hasMarkedDescendant() {
	bool ret=isMarked();
	
	for (BufferNode* it=children.getFront(); it && !ret; it=it->r_sibling) {
		ret=it->isMarked() || it->hasMarkedDescendant();
	}
	return ret;
}
