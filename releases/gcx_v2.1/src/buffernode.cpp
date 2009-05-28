
/*
 | Author: Michael Schmidt;
 |         Gunnar Jehl 
 |
 | ************************* SOFTWARE LICENSE AGREEMENT ***********************
 | This source code is published under the BSD License.
 |
 | See file 'LICENSE.txt' that comes with this distribution or
 | http://dbis.informatik.uni-freiburg.de/index.php?project=GCX/license.php
 | for the full license agreement.
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
 | ****************************************************************************
*/

/*! @file
 *  @brief Implementations of header file buffernode.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file buffernode.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "buffernode.h"
#include "outputstream.h"
#include <cstring>
#include <cstdlib>

BufferNode::BufferNode(const char *_data,
                       BufferNode * _parent):type(TYPE_PCDATA),
parent(_parent),
r_sibling(NULL),
#ifdef ROLE_REFCOUNT
cumulative_roles(0),
non_cumulative_roles(0),
#else
cumulative_roles(NULL),
non_cumulative_roles(NULL),
#endif
           // #ifdef ROLE_REFCOUNT
node((void *) new CharNode(_data)),
is_locked(false) {
}

BufferNode::BufferNode(TAG _tag, BufferNode * _parent):type(TYPE_TAG),
parent(_parent),
r_sibling(NULL),
#ifdef ROLE_REFCOUNT
cumulative_roles(0),
non_cumulative_roles(0),
#else
cumulative_roles(NULL),
non_cumulative_roles(NULL),
#endif                          // #ifdef ROLE_REFCOUNT
node((void *) new TagNode(_tag)),
is_locked(false) {
}

BufferNode::BufferNode(const char *_data, BufferNode * _parent,
                       vector < unsigned >*_cumulative_roles,
                       vector <
                       unsigned >*_non_cumulative_roles):type(TYPE_PCDATA),
parent(_parent),
r_sibling(NULL),
#ifdef ROLE_REFCOUNT
cumulative_roles(0),
non_cumulative_roles(0),
#else
cumulative_roles(NULL),
non_cumulative_roles(NULL),
#endif
           // #ifdef ROLE_REFCOUNT
node((void *) new CharNode(_data)),
is_locked(false) {
    appendRoles(_cumulative_roles, _non_cumulative_roles);
}

BufferNode::BufferNode(TAG _tag, BufferNode * _parent,
                       vector < unsigned >*_cumulative_roles,
                       vector <
                       unsigned >*_non_cumulative_roles):type(TYPE_TAG),
parent(_parent),
r_sibling(NULL),
#ifdef ROLE_REFCOUNT
cumulative_roles(0),
non_cumulative_roles(0),
#else
cumulative_roles(NULL),
non_cumulative_roles(NULL),
#endif
           // #ifdef ROLE_REFCOUNT
node((void *) new TagNode(_tag)),
is_locked(false) {
    appendRoles(_cumulative_roles, _non_cumulative_roles);
}

BufferNode::~BufferNode() {
    if (type == TYPE_TAG) {
        TagNode *t = (TagNode *) node;
        delete t;
    } else if (type == TYPE_PCDATA) {
        CharNode *c = (CharNode *) node;
        delete c;
    }
#ifndef ROLE_REFCOUNT
    delete cumulative_roles;
    delete non_cumulative_roles;
#endif // #ifndef ROLE_REFCOUNT
}

void BufferNode::addChild(BufferNode * child) {
    TagNode *t = (TagNode *) node;

    t->children.push_back(child);
}

void BufferNode::appendRoles(vector < unsigned >*_cumulative_roles,
                             vector < unsigned >*_non_cumulative_roles) {
#ifdef ROLE_REFCOUNT
    cumulative_roles = _cumulative_roles->size();
    non_cumulative_roles = _non_cumulative_roles->size();
#else
    cumulative_roles = new IntMultiSet(_cumulative_roles);
    non_cumulative_roles = new IntMultiSet(_non_cumulative_roles);
#endif // #ifdef ROLE_REFCOUNT
}

void BufferNode::print(OutputStream & eos) {
    TagMap *tagmap = TagMap::getInstance();

    if (type == TYPE_TAG) {
        if (((TagNode *) node)->children.isEmpty()) {
            if (parent != NULL)
                eos << "<" << tagmap->getTag(((TagNode *) node)->tag) << "/>";
        } else {
            if (parent != NULL)
                eos << "<" << tagmap->getTag(((TagNode *) node)->tag) << ">";
            for (BufferNode * it = ((TagNode *) node)->children.getFront();
                 it; it = it->r_sibling) {
                it->print(eos);
            }
            if (((TagNode *) node)->is_closed && parent != NULL)
                eos << "</" << tagmap->getTag(((TagNode *) node)->tag) << ">";
        }
    } else if (type == TYPE_PCDATA) {
        eos << ((CharNode *) node)->data;
    }
}

void BufferNode::printNoSubnodes(OutputStream & dos) {
    TagMap *tagmap = TagMap::getInstance();

    if (type == TYPE_TAG) {
        dos << "<" << tagmap->getTag(((TagNode *) node)->tag) << ">";
    } else if (type == TYPE_PCDATA) {
        dos << ((CharNode *) node)->data;
    }
}

void BufferNode::debugPrint(OutputStream & dos) {
    TagMap *tagmap = TagMap::getInstance();

    if (type == TYPE_TAG) {
        dos << "(<" << tagmap->getTag(((TagNode *) node)->tag) << ">";
    } else if (type == TYPE_PCDATA) {
        dos << "(" << ((CharNode *) node)->data;
    }

    (is_locked) ? dos << "[locked],{" : dos << "{";
#ifdef ROLE_REFCOUNT
    dos << "cc=" << cumulative_roles << ",nc=" << non_cumulative_roles;
#else
    if (cumulative_roles) {
        cumulative_roles->print(dos);
    }
    dos << "},{";
    if (non_cumulative_roles) {
        non_cumulative_roles->print(dos);
    }
#endif // #ifdef ROLE_REFCOUNT
    dos << "})";

    // print children, if any
    if (type == TYPE_TAG) {
        for (BufferNode * it = ((TagNode *) node)->children.getFront(); it; it
             = it->r_sibling) {
            it->debugPrint(dos);
        }
        if (((TagNode *) node)->is_closed)
            dos << "</" << tagmap->getTag(((TagNode *) node)->tag) << ">";
    }
    dos << NEWLINE;
}

void BufferNode::debugPrintCompleteBuffer(OutputStream & dos) {
    parent == NULL ? debugPrint(dos) : parent->debugPrintCompleteBuffer(dos);
}

// requires the node to be fully read
const char *BufferNode::getPCDataRepresentation() {
    std::ostringstream s;
    getPCDataRepresentation(s);

    // ATTENTION: COPY TO BE DELETED AGAIN
    return strdup(s.str().c_str());
}

void BufferNode::getPCDataRepresentation(std::ostringstream & s) {
    if (type == TYPE_PCDATA) {
        s << ((CharNode *) node)->data;
    } else {                    // type==TYPE_TAG
        TagNode *tag = (TagNode *) node;

        for (BufferNode * it = tag->children.getFront(); it; it = it->r_sibling) {
            it->getPCDataRepresentation(s);
        }
    }
}

bool BufferNode::isMarked() {
#ifdef ROLE_REFCOUNT
    return (bool) (cumulative_roles + non_cumulative_roles);
#else
    return (!non_cumulative_roles->isEmpty())
        || (!cumulative_roles->isEmpty());
#endif // #ifdef ROLE_REFCOUNT
}

bool BufferNode::isCumulativeMarked() {
#ifdef ROLE_REFCOUNT
    return (bool) cumulative_roles;
#else
    return !cumulative_roles->isEmpty();
#endif // #ifdef ROLE_REFCOUNT
}

bool BufferNode::hasCumulativeMarkedAncestor() {
    return isRoot()? false : parent->isCumulativeMarked() ||
        parent->hasCumulativeMarkedAncestor();
}

bool BufferNode::hasNoMarkedAndNoLockedDos() {
    // return false if the current node is marked or locked
    if (isMarked() || is_locked)
        return false;

    if (type == TYPE_PCDATA) {
        return true;            // the current node is neither locked nor marked
    } else {
        TagNode *t = (TagNode *) node;

        for (BufferNode * it = t->children.getFront(); it; it = it->r_sibling) {
            if (!it->hasNoMarkedAndNoLockedDos()) {
                return false;
            }
        }
        return true;
    }
}

void BufferNode::removeRoleFromNode(Role * role) {
#ifdef ROLE_REFCOUNT
    role->isDosRole()? cumulative_roles-- : non_cumulative_roles--;
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
    if (is_locked)
        return;

    switch (type) {
        case TYPE_TAG:
            if (((TagNode *) node)->children.isEmpty() && !isRoot()) {
                parent->clear();
            }
            break;

        case TYPE_PCDATA:
            parent->clear();
            break;
    }
}

void BufferNode::clearSubtreeIfPossible() {
    if (type != TYPE_TAG)
        return;

    TagNode *cur = ((TagNode *) node);

    if (!(isCumulativeMarked() || hasCumulativeMarkedAncestor())) {

        BufferNode *c_prev = NULL;
        bool deleted = true;

        for (BufferNode * c = cur->children.getFront(); c && deleted;) {

            BufferNode *c_next = c->r_sibling;

            if (c->hasNoMarkedAndNoLockedDos() &&
                !(c->type == TYPE_TAG && !c->isClosed())) {
                cur->children.removeElem(c, c_prev);
            } else {
                deleted = false;
                c_prev = c;
            }
            //c=c->r_sibling;
            c = c_next;
        }
    }
}
