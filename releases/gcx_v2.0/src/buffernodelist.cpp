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
 * 	@brief Implementations of header file buffernodelist.h.
 * 	@details Implementations of constructors, destructor and functions of the corresponding header file buffernodelist.h.
 * 	@author Michael Schmidt
 * 	@author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#include "buffernodelist.h"
#include "buffernode.h"

BufferNodeList::BufferNodeList() :
front(NULL), back(NULL) {}

BufferNodeList::~BufferNodeList() {
    while (front) {
        BufferNode* next=front->r_sibling;
        delete front;
        front=next;
    }
}

void BufferNodeList::push_back(BufferNode* node) {
    if (!front) {
        front=node;
        back=node;
    } else {
        back->r_sibling=node;
        back=node;
    }
    back->r_sibling=NULL;
}

void BufferNodeList::removeElem(BufferNode* to_remove, BufferNode* prev) {
    // case 1: deleting first element
    if (prev==NULL) {
        if (back==front) { // list contains 1 element
            front=NULL;
            back=NULL;
        } else { // list contains more than 1 element
            front=front->r_sibling;
        }

    // case 2: deleting other than first element
    } else {
        prev->r_sibling=to_remove->r_sibling; // adjust list pointer
        // front remains unchanges
        if (to_remove==back)
            back=prev; // is we remove the last element, back is changed
    }

    delete to_remove;
}

BufferNode* BufferNodeList::operator[](unsigned index) {
    unsigned _index=0;
    BufferNode* it=NULL;
    for (it=front; _index!=index; it=it->r_sibling) {
        _index++;
    }
    return it;
}

void BufferNodeList::print() {
    for (BufferNode* it=front; it!=NULL; it=it->r_sibling) {
        it->debugPrint();
    }
}
