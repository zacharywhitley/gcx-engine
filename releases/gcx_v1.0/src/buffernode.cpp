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
#include "buffernode.h"
#include "pathstepexpression.h"
#include "pathexpression.h"
#include "tagmap.h"
#include "charnode.h"
#include "clobnode.h"
#include "tagnode.h"
#include "error.h"

#ifdef USE_CLOBS
BufferNode::BufferNode(BufferNode* _parent)
	: type(TYPE_CLOB), parent(_parent), r_sibling(NULL),
	node((void*)new CLOBNode()) {}
#endif // USE_CLOBS
BufferNode::BufferNode(const char* data, BufferNode* _parent)
	: type(TYPE_PCDATA), parent(_parent), r_sibling(NULL),
	node((void*)new CharNode(data)) {}

BufferNode::BufferNode(TAG tag, BufferNode* _parent) 
	: type(TYPE_TAG), parent(_parent), r_sibling(NULL),
	 node((void*)new TagNode(tag)) {}
	 
	 
BufferNode::~BufferNode() {

	if (type==TYPE_TAG) {
		TagNode* t=(TagNode*)node;
		delete t;
	} else if (type==TYPE_PCDATA) {
		CharNode* c=(CharNode*)node;
		delete c;
	} else if (type==TYPE_CLOB) {
		CLOBNode* b=(CLOBNode*)node;
		delete b;
	}
}

bool BufferNode::isRoot() {
    return type==TYPE_TAG && ((TagNode*)node)->tag==TAGID_ROOT;
}

void BufferNode::addChild(BufferNode* child) {
    TagNode* t=(TagNode*)node;

	/*
    if (!t->children.isEmpty())
        t->children[t->children.size()-1]->r_sibling=child;
	*/
    t->children.push_back(child);
	
}

void BufferNode::close() {
 
   ((TagNode*)node)->is_closed=true;
}

bool BufferNode::isClosed() {
    return ((TagNode*)node)->is_closed;
}

void BufferNode::matchingNodes(PathExpression* p,
								vector<BufferNode*>* matches) {
    if (type!=TYPE_TAG)
        return;

    if (p->isEmptyPath()) {
		matches->push_back(this); // we're done
				
	} else {

        // in descendant case we descend recursively...
    	if (p->getPathStepAt(0)->getAxisType()==e_descendant) {
			for (BufferNode* it=((TagNode*)node)->children.getFront(); it; it=it->r_sibling) {
				it->matchingNodes(p,matches);
			}
		}

        // get head and tail
        PathStepExpression* prefix=p->getPrefixPathStep();
        PathExpression* suffix=new PathExpression();
        for (unsigned i=1; i<p->getPathSize(); i++) {
            suffix->addPathStep(p->getPathStepAt(i));
        }

        // check for matches
		for (BufferNode* it=((TagNode*)node)->children.getFront(); it; it=it->r_sibling) {
            if (it->type==TYPE_TAG) {
                if (prefix->getNodeTest()==((TagNode*)it->node)->tag
                    || prefix->getNodeTest()==TAGID_STAR) {
                    it->matchingNodes(suffix,matches);
                }
            }
        }
        delete suffix;
    }
}

const char* BufferNode::getChildPCData() {

	std::ostringstream s;
	if (type==TYPE_TAG) {
		TagNode* cur=(TagNode*)node;
		for (BufferNode* it=cur->children.getFront(); it; it=it->r_sibling) {
			if (it->type==TYPE_PCDATA) {
				s << ((CharNode*)it->node)->data;
			}
		}
	}
	
	// ATTENTION: COPY TO BE DELETED AGAIN
	return strdup(s.str().c_str());
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
	} else if (type==TYPE_TAG) {
		TagNode* tag=(TagNode*)node;
		for (BufferNode* it=tag->children.getFront(); it; it=it->r_sibling) {
			it->getPCDataRepresentation(s);
		}
	} else {
		Error::getInstance()->throwError(
						"Illegal context for method getPCDataRepresentation()"); 
	}

}

void BufferNode::lock() {
	if (type==TYPE_TAG)
	    ((TagNode*)node)->is_locked=true;
}

void BufferNode::unlock() {
	if (type==TYPE_TAG) {
	    ((TagNode*)node)->is_locked=false;
	    if (((TagNode*)node)->children.isEmpty()) {
	        clear();
	    }
	}
}

void BufferNode::distributeIndexUpdate(unsigned id) {

    if (type!=TYPE_TAG) {
			Error::getInstance()->throwError(
						"Index update only applicable on tag nodes");
    }

    TagNode* cur=((TagNode*)node);

    for (unsigned i=0; i<cur->cumulative_indices.size(); i++) {
        if (cur->cumulative_indices[i]==id) {
            vector<unsigned>::iterator it=cur->cumulative_indices.begin();
            cur->cumulative_indices.erase(it+i,it+i+1);
            clear();
            return;
        }
    }
	
    for (unsigned i=0; i<cur->noncumulative_indices.size(); i++) {
        if (cur->noncumulative_indices[i]==id) {
            vector<unsigned>::iterator it=cur->noncumulative_indices.begin();
            cur->noncumulative_indices.erase(it+i,it+i+1);
            clear();
			return;
        }
    }
}

void BufferNode::clear() {

    if (type!=TYPE_TAG)
        return;

    clearSubtree();

    // we're not allowed to delete something more
    if (((TagNode*)node)->is_locked)
        return;

    if (((TagNode*)node)->children.isEmpty() && !isRoot()) {
        parent->clear();
    }
}

void BufferNode::clearSubtree() {
    //cout << "ENTERING clearSubtree()";

    if (type!=TYPE_TAG)
        return;

    TagNode* cur=((TagNode*)node);

    if (!(isCumulativeMarked() || hasCumulativeMarkedAncestor())) {
		
		BufferNode* c_prev=NULL;
		bool deleted=true;
		for (BufferNode* c=cur->children.getFront(); c && deleted; ) {

			BufferNode* c_next=c->r_sibling;
			if (!c->hasMarkedDescendant() &&
				!(c->type==TYPE_TAG && !c->isClosed())) {
				cur->children.deleteElem(c,c_prev);
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
    return type==TYPE_TAG && ((TagNode*)node)->isMarked();
}

bool BufferNode::isCumulativeMarked() {
    return type==TYPE_TAG && ((TagNode*)node)->isCumulativeMarked();
}

bool BufferNode::hasMarkedDescendant() {
    return type==TYPE_TAG && 
			((TagNode*)node)->hasMarkedDescendant();
}

bool BufferNode::hasCumulativeMarkedAncestor() {
    if (isRoot())
        return false;
    else
        return parent->isCumulativeMarked() || parent->hasCumulativeMarkedAncestor();

}

void BufferNode::countNodes(unsigned& nodes,
							unsigned& tag,
							unsigned& pcdata) {
	nodes++; // add this one
	#ifndef USE_CLOBS
	if (type==TYPE_PCDATA)
	#else
	if (type==TYPE_PCDATA || type==TYPE_CLOB)
	#endif // USE_CLOBS
		pcdata++;
    else {
        tag++;
		for (BufferNode* it=((TagNode*)node)->children.getFront(); it; it=it->r_sibling) {
            it->countNodes(nodes,tag,pcdata);
		}
	}
}

void BufferNode::debugPrint() {
    TagMap* tagmap=TagMap::getInstance();
    if (type==TYPE_TAG) {
        cout << "(<" << tagmap->getTag(((TagNode*)node)->tag) << ">";
        if (((TagNode*)node)->is_locked)
            cout << "[l],{";
        else
            cout << "[f],{";
        for (unsigned i=0; i<((TagNode*)node)->cumulative_indices.size(); i++) {
            if (i>0)
                cout << ",";
            cout << ((TagNode*)node)->cumulative_indices[i];
        }
        cout << "},{";
        for (unsigned i=0; i<((TagNode*)node)->noncumulative_indices.size(); i++) {
            if (i>0)
                cout << ",";
            cout << ((TagNode*)node)->noncumulative_indices[i];
        }
        cout << "})";
		for (BufferNode* it=((TagNode*)node)->children.getFront(); it; it=it->r_sibling) {
            it->debugPrint();
        }
        if (((TagNode*)node)->is_closed)
            cout << "</" << tagmap->getTag(((TagNode*)node)->tag) << ">";
    } else if (type==TYPE_PCDATA) {
        cout << ((CharNode*)node)->data;
    } else if (type==TYPE_CLOB) {
		((CLOBNode*)node)->print();
	}
}

void BufferNode::printNoSubnodes() {
    TagMap* tagmap=TagMap::getInstance();
    if (type==TYPE_TAG) {
        cout << "<" << tagmap->getTag(((TagNode*)node)->tag) << ">";
    } else if (type==TYPE_PCDATA)  {
        cout << ((CharNode*)node)->data;
    } else if (type==TYPE_CLOB) {
		((CLOBNode*)node)->print();
	}
}

void BufferNode::print() {
    TagMap* tagmap=TagMap::getInstance();
    if (type==TYPE_TAG) {
        if (parent!=NULL)
            cout << "<" << tagmap->getTag(((TagNode*)node)->tag) << ">";
		for (BufferNode* it=((TagNode*)node)->children.getFront(); it; it=it->r_sibling) {
            it->print();
        }
        if (((TagNode*)node)->is_closed && parent!=NULL)
            cout << "</" << tagmap->getTag(((TagNode*)node)->tag) << ">";
    } else if (type==TYPE_PCDATA) {
        cout << ((CharNode*)node)->data;
    } else if (type==TYPE_CLOB) {
		((CLOBNode*)node)->print();
	}
}
