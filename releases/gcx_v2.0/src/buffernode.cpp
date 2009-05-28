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
 *     @brief Implementations of header file buffernode.h.
 *     @details Implementations of constructors, destructor and functions of the corresponding header file buffernode.h.
 *     @author Michael Schmidt
 *     @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
#include "buffernode.h"

BufferNode::BufferNode(const char* _data, BufferNode* _parent) :
type(TYPE_PCDATA), parent(_parent), r_sibling(NULL),
    #ifdef ROLE_REFCOUNT 
    cumulative_roles(0), non_cumulative_roles(0),
    #else
    cumulative_roles(NULL), non_cumulative_roles(NULL),
    #endif // #ifdef ROLE_REFCOUNT
    node((void*)new CharNode(_data)), is_locked(false) {}

BufferNode::BufferNode(TAG _tag, BufferNode* _parent) :
type(TYPE_TAG), parent(_parent), r_sibling(NULL),
    #ifdef ROLE_REFCOUNT 
    cumulative_roles(0), non_cumulative_roles(0),
    #else
    cumulative_roles(NULL), non_cumulative_roles(NULL),
    #endif // #ifdef ROLE_REFCOUNT
    node((void*)new TagNode(_tag)), is_locked(false) {}

BufferNode::BufferNode(const char* _data, BufferNode* _parent,
                        vector<unsigned>* _cumulative_roles,
                        vector<unsigned>* _non_cumulative_roles) :
type(TYPE_PCDATA), parent(_parent), r_sibling(NULL),
    #ifdef ROLE_REFCOUNT 
    cumulative_roles(0), non_cumulative_roles(0),
    #else
    cumulative_roles(NULL), non_cumulative_roles(NULL),
    #endif // #ifdef ROLE_REFCOUNT
    node((void*)new CharNode(_data)), is_locked(false) {
    appendRoles(_cumulative_roles,_non_cumulative_roles);
}

BufferNode::BufferNode(TAG _tag, BufferNode* _parent,
                        vector<unsigned>* _cumulative_roles,
                        vector<unsigned>* _non_cumulative_roles) :
type(TYPE_TAG), parent(_parent), r_sibling(NULL),
    #ifdef ROLE_REFCOUNT 
    cumulative_roles(0), non_cumulative_roles(0),
    #else
    cumulative_roles(NULL), non_cumulative_roles(NULL),
    #endif // #ifdef ROLE_REFCOUNT
    node((void*)new TagNode(_tag)), is_locked(false) {
    appendRoles(_cumulative_roles,_non_cumulative_roles);
}

BufferNode::~BufferNode() {
    if (type==TYPE_TAG) {
        TagNode* t=(TagNode*)node;
        delete t;
    } else if (type==TYPE_PCDATA) {
        CharNode* c=(CharNode*)node;
        delete c;
    }
    #ifndef ROLE_REFCOUNT
    delete cumulative_roles;
    delete non_cumulative_roles;
    #endif // #ifndef ROLE_REFCOUNT
}


void BufferNode::appendRoles(vector<unsigned>* _cumulative_roles,
                                vector<unsigned>* _non_cumulative_roles) {
    #ifdef ROLE_REFCOUNT
    cumulative_roles=_cumulative_roles->size();
    non_cumulative_roles=_non_cumulative_roles->size();
    #else
    cumulative_roles=new IntMultiSet(_cumulative_roles);
    non_cumulative_roles= new IntMultiSet(_non_cumulative_roles);
    #endif // #ifdef ROLE_REFCOUNT
}

void BufferNode::addChild(BufferNode* child) {
    TagNode* t=(TagNode*)node;
    t->children.push_back(child);
}

// requires the node to be fully read
const char* BufferNode::getPCDataRepresentation() {
    std::ostringstream s;
    getPCDataRepresentation(s);

    // ATTENTION: COPY TO BE DELETED AGAIN
    return strdup(s.str().c_str());
}

void BufferNode::getPCDataRepresentation(std::ostringstream& s) {
    if (type==TYPE_PCDATA) {
        s << ((CharNode*)node)->data;
    } else { // type==TYPE_TAG
        TagNode* tag=(TagNode*)node;
        for (BufferNode* it=tag->children.getFront(); it; it=it->r_sibling) {
            it->getPCDataRepresentation(s);
        }
    }
}

void BufferNode::removeRoleFromNode(Role* role) {
    #ifdef ROLE_REFCOUNT
    role->isDosRole()?cumulative_roles--:non_cumulative_roles--;
    #else
    // will be applied to exactly one of the role sets, but does not
    // harm the other, so we can safely apply the role removal to both
    cumulative_roles->removeElem(role->getId());
    non_cumulative_roles->removeElem(role->getId());
    #endif // #ifdef ROLE_REFCOUNT
    clear();
}

void BufferNode::clear() {

    clearSubtreeIfPossible();

    // we're not allowed to delete something more
    if (is_locked) return;

    switch (type) {
        case TYPE_TAG:
            if (((TagNode*)node)->children.isEmpty() && !isRoot()) {
                parent->clear();
            }
            break;

        case TYPE_PCDATA:
            parent->clear();
            break;
    }
}

void BufferNode::clearSubtreeIfPossible() {
    if (type!=TYPE_TAG)
        return;

    TagNode* cur=((TagNode*)node);

    if (!(isCumulativeMarked() || hasCumulativeMarkedAncestor())) {

        BufferNode* c_prev=NULL;
        bool deleted=true;
        for (BufferNode* c=cur->children.getFront(); c && deleted;) {

            BufferNode* c_next=c->r_sibling;
            if (c->hasNoMarkedAndNoLockedDos() &&
                !(c->type==TYPE_TAG && !c->isClosed())) {
                cur->children.removeElem(c, c_prev);
            } else {
                deleted=false;
                c_prev=c;
            }
            //c=c->r_sibling;
            c=c_next;
        }
    }
}

bool BufferNode::isMarked() {
    #ifdef ROLE_REFCOUNT
    return (bool)(cumulative_roles+non_cumulative_roles);
    #else
    return (!non_cumulative_roles->isEmpty()) || (!cumulative_roles->isEmpty());
    #endif // #ifdef ROLE_REFCOUNT
}

bool BufferNode::isCumulativeMarked() {    
    #ifdef ROLE_REFCOUNT
    return (bool)cumulative_roles;
    #else
    return !cumulative_roles->isEmpty();
    #endif // #ifdef ROLE_REFCOUNT
}

bool BufferNode::hasCumulativeMarkedAncestor() {
    return isRoot()?false:parent->isCumulativeMarked() ||
                            parent->hasCumulativeMarkedAncestor();
}

bool BufferNode::hasNoMarkedAndNoLockedDos() {
    // return false if the current node is marked or locked
    if (isMarked() || is_locked) return false;

    if (type==TYPE_PCDATA) {
        return true; // the current node is neither locked nor marked
    } else {
        TagNode* t=(TagNode*)node;
        for (BufferNode* it=t->children.getFront(); it; it=it->r_sibling) {
            if (!it->hasNoMarkedAndNoLockedDos()) {
                return false;
            }
        }
        return true;
    }
}


void BufferNode::print() {
    TagMap* tagmap=TagMap::getInstance();
    if (type==TYPE_TAG) {
        if (((TagNode*)node)->children.isEmpty()) {
            if (parent!=NULL)
                cout << "<" << tagmap->getTag(((TagNode*)node)->tag) << "/>";
        } else {
            if (parent!=NULL)
                   cout << "<" << tagmap->getTag(((TagNode*)node)->tag) << ">";
            for (BufferNode* it=((TagNode*)node)->children.getFront();
                    it; it=it->r_sibling) {
                it->print();
            }
            if (((TagNode*)node)->is_closed && parent!=NULL)
                cout << "</" << tagmap->getTag(((TagNode*)node)->tag) << ">";
        }
    } else if (type==TYPE_PCDATA) {
        cout << ((CharNode*)node)->data;
    }
}

void BufferNode::printNoSubnodes() {
    TagMap* tagmap=TagMap::getInstance();
    if (type==TYPE_TAG) {
        cout << "<" << tagmap->getTag(((TagNode*)node)->tag) << ">";
    } else if (type==TYPE_PCDATA) {
        cout << ((CharNode*)node)->data;
    }
}

void BufferNode::debugPrint() {
    TagMap* tagmap=TagMap::getInstance();
    if (type==TYPE_TAG) {
        cout << "(<" << tagmap->getTag(((TagNode*)node)->tag) << ">";
    } else if (type==TYPE_PCDATA) {
        cout << "(" << ((CharNode*)node)->data;
    }

    (is_locked)?cout << "[locked],{": cout << "{";
    #ifdef ROLE_REFCOUNT
    cout << "cc=" << cumulative_roles << ",nc=" << non_cumulative_roles;
    #else
    if (cumulative_roles) {
        cumulative_roles->print();
    }
    cout << "},{";
    if (non_cumulative_roles) {
        non_cumulative_roles->print();
    }
    #endif // #ifdef ROLE_REFCOUNT
    cout << "})";

    // print children, if any
    if (type==TYPE_TAG) {
        for (BufferNode* it=((TagNode*)node)->children.getFront(); it; it
                =it->r_sibling) {
            it->debugPrint();
        }
          if (((TagNode*)node)->is_closed)
               cout << "</" << tagmap->getTag(((TagNode*)node)->tag) << ">";
    }
}

void BufferNode::debugPrintCompleteBuffer() {
    parent==NULL?debugPrint():parent->debugPrintCompleteBuffer();
}
