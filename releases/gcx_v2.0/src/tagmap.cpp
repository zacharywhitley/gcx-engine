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
 | * Redistributions of source code must retain the above
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
 * 	@brief Implementations of header file tagmap.h.
 * 	@details Implementations of constructors, destructor and functions of the corresponding header file tagmap.h.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#include "tagmap.h"

TagMap* TagMap::instance=NULL;

TagMap* TagMap::getInstance() {
    if (instance==NULL) {
        instance=new TagMap();
    }

    return instance;
}

TagMap::TagMap() {
	char* root=new char[strlen("ROOT")+1];
	strcpy(root, "ROOT");
	tags.push_back(root); // TAGID_ROOT
}

TagMap::~TagMap() {
    for (unsigned i=0; i<tags.size(); i++) {
        delete[] tags[i];
    }
}

unsigned TagMap::insertTag(const char* tag) {
    unsigned hash_index=sh.hash(tag);
    if (hash_index==tags.size()) {
        char* tag_copy = new char[strlen(tag)+1];
        strcpy(tag_copy, tag);
        tags.push_back(tag_copy);
    }

    return hash_index;
}

unsigned TagMap::insertTagWithOffset(const char* tag,unsigned start_offset,
										unsigned end_offset) {

    unsigned hash_index=sh.hash(tag,start_offset,end_offset);
    if (hash_index==tags.size()) {
		// store the tag in the tag map if not yet done
		unsigned total_offset=start_offset+end_offset;
    	char* tag_copy=new char[strlen(tag)-total_offset+1];
	    strncpy(tag_copy,tag+start_offset,strlen(tag)-total_offset);
	    tag_copy[strlen(tag)-(start_offset+end_offset)]='\0';
        tags.push_back(tag_copy);
    }

    return hash_index;
}

void TagMap::print() {
    for (unsigned i=0; i<tags.size(); i++) {
        cout << i << " -> <" << tags[i] << "></" << tags[i] << ">" << endl;
    }
}
