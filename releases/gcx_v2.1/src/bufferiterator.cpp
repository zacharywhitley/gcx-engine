
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
 *  @brief Implementations of header file bufferiterator.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file bufferiterator.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "bufferiterator.h"

BufferIterator::BufferIterator(BufferNode * _base, PathExpression * _path):
base(_base), cur(_base), cur_old(NULL), cur_locked(false),
path(_path), spp(StreamPreProcessor::getInstance()), ps_index(0) {
    if (!path) {
        return;
    }
    // determine if the entered path locates any node, which is not the case if
    // a.) the path has one or more path step of the form /axis::text() within the path or
    //     the path step after /axis::text() is not of the form /dos::node().
    // b.) the base is a textnode and the path step followed the base is not of the
    //     form /dos::node(). It also could be /dos::text() but this axis-node test combination
    //     does not appear in our fragment, because axis dos is neither allowed in any query
    //     nor does node test text() appear as node test of axis dos.
    if ((path->hasInnerTextNodeTest()
         && !path->getPathStepAfterTextNodeTest()->isDosNodeStep())
        || (base->type == TYPE_PCDATA
            && !path->getPathStepAt(0)->isDosNodeStep())) {
        cur = NULL;
    }

    for (unsigned i = 0; i < _path->getPathSize(); i++) {
        // initialize member variable ps_context, which contains/stores for all path steps the
        // current matched node for a path step. On initialization this is for all path steps NULL,
        // becasue of the fact, that no traversal of the XML tree has been processed up to now.
        ps_context.push_back(NULL);

        // initialize member variable ps_context_position, which stores the number of assignments
        // which has been made for a path step. This means, that for every new find of a matching
        // node for a path step (not NULL change of an entry of member variable ps_context) it is
        // incremented and reset to zero if the previous processed path step gets an new assignment.
        // On initialization this is, because of the fact that no traversal has been made
        // up to now, for all path steps zero.
        ps_context_position.push_back(0);

        // initialize member variable ps_skip_subtree, which indicates that it is possible
        // to switch/jump immediately to the next right sibling instead of processing a full
        // search of the whole tree/subtree.
        // Skipping subtrees could be done if:
        // a.) the path step after the base is of the form /child::....
        // b.) the path contains a sequence of /child::... path steps WITHOUT any previous
        //     path step of the form /descendant::... or /dos::....
        if (i == 0 && path->getPathStepAt(i)->getAxisType() == at_child) {
            ps_skip_subtree.push_back(true);
        } else if (!path->hasPreviousDescendantOrDosUpTo(i) &&
                   path->getPathStepAt(i)->getAxisType() == at_child) {
            ps_skip_subtree.push_back(true);
        } else {
            ps_skip_subtree.push_back(false);
        }

        // initialize member variables ps_attribute and ps_attribute_position, which
        // indicate that a path step has an attribute (ps_attribute) and for the only
        // attribute type [position()=n] store n (ps_attribute_position) for this path step.
        // These two member variables are not necessarily needed but, because of rare attribute
        // appearances, avoids checking if a path step has an attribute, which has to be done by
        // checking if the attribute is NULL.
        if (path->getPathStepAt(i)->hasAttribute()) {
            ps_attribute.push_back(true);
            switch (path->getPathStepAt(i)->getAttribute()->getType()) {
                case at_position:
                    ps_attribute_position.
                        push_back(((PathStepAttributePosition *) path->
                                   getPathStepAt(i)->getAttribute())->
                                  getPosition());
                    break;
            }
        } else {
            ps_attribute.push_back(false);
            ps_attribute_position.push_back(0);
        }

        // initialize member variable ps_back_track, which indicates that it is needed to backtrack
        // one or more path steps. This backtracking must be done if a path contains of a sequence
        // of /child::... path steps (except for the first /child::... path step of this sequence or
        // if this sequence contains only of one /child::... path step) WITH a previous path step of
        // the form /descendant::... or /dos::.... This backtracking variable (ps_back_track) remains
        // true as long as there is no further /descendant::... or /dos::... path step.
        // For example:
        // path: //.../... has false; false and
        // path: //.../.../... has false; false; true and
        // path: //.../.../.../... has false; false; true; true and
        // path: //.../.../.../...//... has false; false; true; true; false and
        // path: //.../.../.../...//.../... has false; false; true; true; false; false and
        // path: //.../.../.../...//.../.../... has false; false; true; true; false; false; true
        // and so on...
        if (i > 0 && ps_back_track[i - 1]
            && path->getPathStepAt(i)->getAxisType() == at_child) {
            ps_back_track.push_back(true);
        } else if (i > 1
                   && path->getPathStepAt(i - 2)->getAxisType() != at_child
                   && path->getPathStepAt(i - 1)->getAxisType() == at_child
                   && path->getPathStepAt(i)->getAxisType() == at_child) {
            ps_back_track.push_back(true);
        } else {
            ps_back_track.push_back(false);
        }
    }
}

BufferIterator::~BufferIterator() {
}

void BufferIterator::init(BufferNode * _base) {
    base = _base;
    cur = _base;
    cur_old = NULL;
    cur_locked = false;
    ps_index = 0;

    if (!path) {
        return;
    }

    if ((path->hasInnerTextNodeTest()
         && !path->getPathStepAfterTextNodeTest()->isDosNodeStep())
        || (_base->type == TYPE_PCDATA
            && !path->getPathStepAt(0)->isDosNodeStep())) {
        cur = NULL;
    }
}

void BufferIterator::debugPrint(OutputStream & dos, unsigned debug_mode) {
    vector < PathStepExpression * >*ps = NULL;
    if (path) {
        dos << "path=" << (*path) << NEWLINE;
        ps = path->getPathSteps();
        dos << "path_process=";
        for (unsigned i = 0; i < ps->size(); i++) {
            dos << (*path->getPathStepAt(i)) << "[" << ps_context_position[i];
            if (ps_attribute_position[i] > 0) {
                dos << " <= " << ps_attribute_position[i];
            }
            dos << "]";
            if (debug_mode == BIT_DEBUG_MODE_FULL
                || debug_mode == BIT_DEBUG_MODE_PARTIAL) {
                dos << " [";
                if (ps_attribute[i]) {
                    dos << "attribute=true";
                } else {
                    dos << "attribute=false";
                }
                dos << "; ";
                if (ps_skip_subtree[i]) {
                    dos << "skip_subtree=true";
                } else {
                    dos << "skip_subtree=false";
                }
                dos << "; ";
                if (ps_back_track[i]) {
                    dos << "back_track=true";
                } else {
                    dos << "back_track=false";
                }
                dos << "] ";
            } else {
                dos << " ";
            }
            if (i == ps_index) {
                dos << "{*processed*} ";
            }
        }
    }
    dos << NEWLINE << "base ";
    if (base) {
        if (base->type == TYPE_TAG) {
            dos << "[TAG] => ";
        } else {
            dos << "[#PCDATA] => ";
        }
        if (debug_mode == BIT_DEBUG_MODE_FULL && !base->isRoot()) {
            base->debugPrint(dos);
        } else {
            base->printNoSubnodes(dos);
        }
    } else {
        dos << "[NONE] => NULL";
    }
    if (path) {
        for (unsigned i = 0; i < ps->size(); i++) {
            dos << NEWLINE << "step=[" << i << "] ";
            if (ps_context[i]) {
                if (ps_context[i]->type == TYPE_TAG) {
                    dos << "[TAG] => ";
                } else {
                    dos << "[#PCDATA] => ";
                }
                if (debug_mode == BIT_DEBUG_MODE_FULL
                    && !ps_context[i]->isRoot()) {
                    ps_context[i]->debugPrint(dos);
                } else {
                    ps_context[i]->printNoSubnodes(dos);
                }
            } else {
                dos << "[NONE] => NULL";
            }
        }
    }
    dos << NEWLINE << NEWLINE << "cur (previous returned node) ";
    if (cur_old) {
        if (cur_old->type == TYPE_TAG) {
            dos << "[TAG] => ";
        } else {
            dos << "[#PCDATA] => ";
        }
        if (debug_mode == BIT_DEBUG_MODE_FULL && !cur_old->isRoot()) {
            cur_old->debugPrint(dos);
        } else {
            cur_old->printNoSubnodes(dos);
        }
    } else {
        dos << "[NONE] => NULL";
    }
    dos << NEWLINE << "cur (returned node) ";
    if (cur) {
        if (cur_locked) {
            dos << "[bit locked] ";
        } else {
            dos << "[bit not locked] ";
        }
        if (cur->type == TYPE_TAG) {
            dos << "[TAG] => ";
        } else {
            dos << "[#PCDATA] => ";
        }
        if (debug_mode == BIT_DEBUG_MODE_FULL && !cur->isRoot()) {
            cur->debugPrint(dos);
        } else {
            cur->printNoSubnodes(dos);
        }
    } else {
        dos << "[NONE] => NULL";
    }
}

BufferNode *BufferIterator::getNext(unsigned read_up_to_closing,
                                    unsigned lock_node) {
    if (cur == NULL) {
        return NULL;
    }
    // process the XML tree with a (full) left-to-right depth-first traversal skipping subtrees
    // whenever possible. Therefore method isSatisfyingPath() returns a node, which is:
    // -> NULL if the path is fully satisfied to stop traversal and return this/a node, which is
    //    the node stored in the last entry in member variable ps_context or the node stored in
    //    the first entry in member variable ps_context if the XML tree is fully traversed
    // -> the argument node of method isSatisfyingPath() to process the XML tree from this node on
    // -> a new node (right sibling of a node) to process the XML tree from this (new) node on (to
    //    skip subtrees)
    // The idea behind the left-to-right traversal is:
    // (1) from a tagnode (with childs) we go downward using his first child
    // (2) from a textnode or a tagnode (with no childs) we go upward using the child-parent
    //     connection as long as the upward processed node has no right sibling
    // (3) if the processed node has a right sibling we switch/jump to this right sibling node
    //     and start with (1) or (2) (depends on what node condition is fulfilled)
    if (path) {
        BufferNode *nnode = cur;

        while ((bool) (nnode = isSatisfyingPath(nnode))) {
            if (nnode->type == TYPE_TAG) {
                while (((TagNode *) nnode->node)->children.isEmpty()
                       && !nnode->isClosed()) {
                    spp->readNext();
                }
                if (!((TagNode *) nnode->node)->children.isEmpty()) {
                    nnode = ((TagNode *) nnode->node)->children[0];
                } else {
                    while (nnode->r_sibling == NULL && nnode->parent
                           && !nnode->parent->isClosed()) {
                        spp->readNext();
                    }
                    while (nnode->r_sibling == NULL && nnode != base) {
                        unsignPSNode(nnode);
                        nnode = nnode->parent;
                        while (nnode->r_sibling == NULL && nnode->parent
                               && !nnode->parent->isClosed()) {
                            spp->readNext();
                        }
                    }
                    unsignPSNode(nnode);
                    if (nnode == base) {
                        nnode = NULL;
                    } else {
                        nnode = nnode->r_sibling;
                    }
                }
            } else {            // type==TYPE_PCDATA
                while (nnode->r_sibling == NULL && nnode->parent
                       && !nnode->parent->isClosed()) {
                    spp->readNext();
                }
                while (nnode->r_sibling == NULL && nnode != base) {
                    unsignPSNode(nnode);
                    nnode = nnode->parent;
                    while (nnode->r_sibling == NULL && nnode->parent
                           && !nnode->parent->isClosed()) {
                        spp->readNext();
                    }
                }
                unsignPSNode(nnode);
                if (nnode == base) {
                    nnode = NULL;
                } else {
                    nnode = nnode->r_sibling;
                }
            }
        }

        // store the previous returned node to unlock and clear this node and also store the new
        // returned node for later unlock and clear
        BufferNode *cur_old_tmp = cur_old;

        cur_old = cur;

        // returned node, which is in case if it is not NULL, always the one stored for the last
        // path step in member variable ps_context or in case if it is NULL the one for the first
        // path step, which is NULL because of unsign/remove of all stored nodes for all path steps
        // after complete upward traversal of the XML tree by method unsignPSNode().
        cur = ps_context[ps_index];

        // if there are not more matching nodes for a path, (re-)set the counter for the number
        // of assignments for the first path step to zero to get correct debug output.
        if (cur == NULL) {
            ps_context_position[ps_index] = 0;
        }
        // if (cur==NULL) {
        //   if (ps_index>0) {
        //     throw RuntimeException("BufferIterator Error (ps_index)", eid_runtime_bit);
        //       }
        //   for (unsigned i=0; i<ps_context.size(); i++) {
        //     if (ps_context[i]) {
        //       throw RuntimeException("BufferIterator Error (ps_context)", eid_runtime_bit);
        //       break;
        //     } else if (ps_context_position[i]>0) {
        //       throw RuntimeException("BufferIterator Error (ps_context_position)", eid_runtime_bit);
        //       break;
        //     }
        //   }
        // }

        // read if it is requested up to a closing tag of a node.
        if (read_up_to_closing == READ_UP_TO_CLOSE_BASE && base
            && base->type == TYPE_TAG) {
            while (!base->isClosed()) {
                spp->readNext();
            }
        } else if (read_up_to_closing == READ_UP_TO_CLOSE_CONTEXT && cur
                   && cur->type == TYPE_TAG) {
            while (!cur->isClosed()) {
                spp->readNext();
            }
        }
        // lock and invoke (active) garbage collection via methode clear()
        // on a BufferNode if it is requested
        if (lock_node != LOCK_NONE_NO_CLEAR) {
            bool externally_locked = true;
            bool cur_locked_tmp = cur_locked;

            cur_locked = false;
            if (cur) {
                externally_locked = cur->isLocked();
                if (!externally_locked) {
                    cur->lock();
                    cur_locked = true;
                }
                if (cur_old_tmp && cur_locked_tmp) {
                    cur_old->unlock();
                    if (lock_node == LOCK_CONTEXT_ALWAYS_CLEAR) {
                        cur_old->clear();
                    }
                }
            } else {
                if (lock_node != LOCK_CONTEXT_NO_CLEAR) {
                    if (cur_old_tmp && cur_locked_tmp) {
                        cur_old->unlock();
                        cur_old->clear();
                    }
                }
            }
        }

        return cur;
    } else {                    // path==NULL
        // read if it is requested up to a closing tag of a node.
        if ((read_up_to_closing == READ_UP_TO_CLOSE_BASE
             || read_up_to_closing == READ_UP_TO_CLOSE_CONTEXT) && base
            && base->type == TYPE_TAG) {
            while (!base->isClosed()) {
                spp->readNext();
            }
        }

        cur = NULL;
        return base;
    }
}

void BufferIterator::reset() {
    // reset most of the member variables like set by the constructor
    cur = base;
    cur_old = NULL;
    cur_locked = false;
    ps_index = 0;

    if (!path) {
        return;
    }

    if ((path->hasInnerTextNodeTest()
         && !path->getPathStepAfterTextNodeTest()->isDosNodeStep())
        || (base->type == TYPE_PCDATA
            && !path->getPathStepAt(0)->isDosNodeStep())) {
        cur = NULL;
    }

    for (unsigned i = 0; i < path->getPathSize(); i++) {
        ps_context[i] = NULL;
        ps_context_position[i] = 0;
    }
}

void BufferIterator::clear() {

}

BufferNode *BufferIterator::isSatisfyingPath(BufferNode * nnode) {
    bool match = false;

    if (nnode == cur) {
        return nnode;
    }

    if (nnode) {
        // ####################
        // BACKTRACKING PART 2:
        // ####################
        // This piece of code depends on BACKTRACKING PART 1, therefore it is inevitable necessary
        // to understand or read BACKTRACKING PART 1 first!
        // Whereas BACKTRACKING PART 1 pushes path steps, which have a set backtracking value into
        // subtrees, BACKTRACKING PART 2 forces this path steps out of this subtrees. Therefore this
        // is BACKTRACKING PART 2, because of the fact, that we first need to push path step processing
        // into subtrees before we fetch these path steps out of there. 
        // Before we start one example to illustrate:
        // For example we had the following path //b/c/d and the following XML:
        // <a><b><c><d>...<b>...</b>...</d></c></b><b><c><d/></c></b></a>
        // with many elements or content in element <c><d>...</d></c>, i.e. with a deep subtree.
        // Then BACKTRACKING PART 1 pushes path step processing into this (deep) subtree and the done
        // upward traversal of the XML tree will remove all assigned nodes and also decrement member
        // variable ps_index. By the done upward traversal of the XML tree we then either processing
        // path step //.../..., strictly speaking the /... path step or the last /... path step (depends
        // on subtree depth and XML tree structure!). If we would not force path step processing out of
        // this subtree(s), we will not find the matching node <d/> in ...<b><c><d/></c></b>...,
        // which is a smaller subtree than the other one.
        // NOTE: because of the fact, that descendant or dos path steps will only be assigned
        // to a node once in subtrees we will not search for a matching node for the //... path step.
        // If we would search for a matching node for the //... path step this would be correct!
        // The idea behind the following piece of code is, that we calculate distances between all
        // path steps with a set backtracking value and ancestors of the current processed node by
        // this method. As we do not know if we had a smaller subtree, we enter this if-case in
        // three cases:
        // a.) the current processed path step is the one after the //... path step, i.e. the /...
        //     path step
        // b.) the current processed path step has a set backtracking value
        // NOTE: ensure that this if-case will not be invoked by BACKTRACKING PART 1 by checking
        //     if cur is NULL!
        if (cur
            &&
            ((ps_index > 0
              && path->getPathStepAt(ps_index - 1)->getAxisType() != at_child
              && path->getPathStepAt(ps_index)->getAxisType() == at_child
              && (ps_index + 1) < path->getPathSize()
              && path->getPathStepAt(ps_index + 1)->getAxisType() == at_child)
             || ps_back_track[ps_index])) {
            // first of all we calculate the first path step without any backtracking value, which is
            // the path step /... in //.../....
            unsigned ps_index_first_back_track = ps_index;

            while (ps_back_track[ps_index_first_back_track]) {
                ps_index_first_back_track--;
            }
            // then we calculate the last path step, which has a set backtracking value.
            unsigned ps_index_last_back_track = ps_index_first_back_track;

            ps_index_last_back_track++;
            while (ps_back_track[ps_index_last_back_track]
                   && ps_index_last_back_track < path->getPathSize()) {
                ps_index_last_back_track++;
            }
            ps_index_last_back_track = ps_index_last_back_track - 1;
            // we then check if the current processed node matches this last path step and if it is
            // so and also the current processed path step is not this one (if the current processed
            // path step would be the last one with a backtracking value checking for matching would
            // be handled automatically!) we start calculating the distance.  
            if (isMatchingNodeTestAndPredicates(nnode, ps_index_last_back_track)
                && ps_index != ps_index_last_back_track) {
                BufferNode *nnode_tmp = nnode;
                bool match_tmp = true;

                // process from the last path step with a set backtracking value on...
                unsigned ps_index_tmp = ps_index_last_back_track;

                // ...and put all ancestors on to a stack...
                stack < BufferNode * >st_ancestors = stack < BufferNode * >();
                // ...as long as we can go upward the XML tree (not reaching ROOT-node) and as long
                // as the parent nodes matches the path steps with a set backtracking value or the
                // path step //.../... we put these nodes on to the stack.
                while (nnode_tmp
                       && ps_index_tmp > (ps_index_first_back_track - 1)
                       && match_tmp) {
                    if (nnode_tmp->parent) {
                        nnode_tmp = (nnode_tmp->parent->isRoot()
                                     || nnode_tmp ==
                                     ps_context[ps_index_first_back_track -
                                                1]) ? NULL : nnode_tmp->parent;
                        if (nnode_tmp) {
                            st_ancestors.push(nnode_tmp);
                            ps_index_tmp--;
                            if (!isMatchingNodeTestAndPredicates
                                (nnode_tmp, ps_index_tmp)) {
                                match_tmp = false;
                                ps_index_tmp = ps_index_last_back_track;
                            }
                        } else {
                            match_tmp = false;
                            ps_index_tmp = ps_index_last_back_track;
                        }
                    } else {
                        nnode_tmp = nnode_tmp->parent;
                    }
                }
                // if we found matching ancestors for ALL path steps with a set backtracking value
                // and for the //.../... path step, we assign them by doing a recursive invocation
                // of this method.
                if (ps_index_tmp == (ps_index_first_back_track - 1)) {
                    // as in BACKTRACKING PART 1 we need to reset the set backtracking value and also
                    // set cur to NULL in order to avoid calling BACKTRACKING PART 1 and PART 2.
                    BufferNode *cur_old = cur;
                    bool ps_backtrack_reset;

                    ps_index = ps_index_first_back_track;
                    unsigned ps_index_old = ps_index;

                    st_ancestors.pop();
                    while (!st_ancestors.empty()) {
                        ps_backtrack_reset = false;

                        if (ps_back_track[ps_index]) {
                            ps_back_track[ps_index] = false;
                            ps_backtrack_reset = true;
                        }

                        cur_old = cur;
                        cur = NULL;

                        ps_index_old = ps_index;

                        nnode_tmp = isSatisfyingPath(st_ancestors.top());

                        if (ps_backtrack_reset) {
                            ps_back_track[ps_index_old] = true;
                        }

                        cur = cur_old;

                        st_ancestors.pop();
                    }

                    // clear stack.
                    st_ancestors = stack < BufferNode * >();
                    if (nnode_tmp == NULL) {
                        if (ps_index >= path->getPathSize()) {
                            ps_index--;
                        }
                        return NULL;
                    }
                }
                // clear stack.
                st_ancestors = stack < BufferNode * >();
            }
        }
        // switch over the current processed path step to check if it is matching.
        switch (path->getPathStepAt(ps_index)->getAxisType()) {
            case at_child:
                if (ps_index > 0
                    && path->getPathStepAt(ps_index - 1)->getAxisType() !=
                    at_child
                    && path->getPathStepAt(ps_index)->getAxisType() ==
                    at_child) {
                    if (isMatchingNodeTestAndPredicates(nnode, ps_index)
                        && isMatchingNodeTestAndPredicates(nnode->parent,
                                                           (ps_index - 1))) {

                        if (nnode->parent != getPrevPSNode()) {
                            // increment the node assignment counter of the previous processed
                            // path step, because we had an new assignment...
                            ps_context_position[ps_index - 1] =
                                (ps_context_position[ps_index - 1] + 1);
                            // ...and also reset the node assignment counter for the current
                            // processed path step, because we had a new context node for the
                            // previous processed path step.
                            ps_context_position[ps_index] = 0;
                        }
                        // unsign/remove the stored node from the current processed path step, because
                        // we had a new context node for the previous processed path step.
                        ps_context[ps_index] = NULL;
                        match = true;
                    }
                } else {
                    if (isMatchingNodeTestAndPredicates(nnode, ps_index)
                        && nnode->parent == getPrevPSNode()) {
                        match = true;
                    }
                }
                break;
            case at_descendant:
                if (isMatchingNodeTestAndPredicates(nnode, ps_index)) {
                    match = true;
                }
                break;
            case at_dos:
                if (isMatchingNodeTestAndPredicates(nnode, ps_index)) {
                    match = true;
                }
                break;
        }
    }

    BufferNode *pnode = nnode;

    if (match) {
        ps_context[ps_index] = nnode;
        ps_context_position[ps_index] = (ps_context_position[ps_index] + 1);
        ps_index++;
        // because of the fact that we are processing nodes by this method only once, we need to
        // check if one or more of the following path steps has axis dos and if it is so, if the
        // current processed node of this method matches one or more of these path steps.
        while (ps_index < path->getPathSize()
               && path->getPathStepAt(ps_index)->getAxisType() == at_dos
               && isMatchingNodeTestAndPredicates(nnode, ps_index)) {
            ps_context[ps_index] = nnode;
            ps_context_position[ps_index] = (ps_context_position[ps_index] + 1);
            ps_index++;
        }
    } else if ((nnode && ps_back_track[ps_index]
                && (nnode->parent == getPrevPSNode()
                    || (nnode->parent
                        && nnode->parent->parent == getPrevPSNode())))) {
        // ####################
        // BACKTRACKING PART 1:
        // ####################
        // The cause why we do this is not to loose/forget nodes for iteration or output.
        // This could happen if the current processed path step has a set backtracking value
        // (see constructor for it!).
        // Recall if you do not want to go back to the constructor:
        // path: //.../... has backtracking values false; false and
        // path: //.../.../... has backtracking values false; false; true and
        // path: //.../.../.../... has backtracking values false; false; true; true
        // First of all, what do we know if we enter this part:
        // --> the current processed path step has a set backtracking value AND
        // --> the current processed node does not match the current processed path step AND
        // --> the current processed node is a child node of the assigned node to the previous
        //     processed path step
        //     (the current processed node is the right one for the child (/...) axis but
        //      does not match the current processed path step) OR
        // --> the current processed node is a child-child node of the assigned node to the
        //     previous processed path step
        //     (this is the case if we had an assigned node to the last path step and the
        //      traversal of the XML tree goes on)

        // first of all we read the input stream to check what node we are currently processing,
        // this means, we need to know if the current processed node has a subtree or any childs.
        // If the current processed node has a subtree or any child we need to backtrack one or
        // more path step(s) to get all matching nodes for the entered path. This is done in order
        // to get all matching nodes into member variable ps_context, or in other words to force
        // path step processing to the correct path step index (ps_index) and to get all matching
        // nodes assigned for the correct path step in member variable ps_context.
        bool process_backtrack = false;

        if (nnode->parent && nnode->parent->parent == getPrevPSNode()) {
            process_backtrack = true;
        } else if (nnode->type == TYPE_TAG) {
            while (((TagNode *) nnode->node)->children.isEmpty()
                   && !nnode->isClosed()) {
                spp->readNext();
            }

            if (!((TagNode *) nnode->node)->children.isEmpty()) {
                process_backtrack = true;
            }
        }
        // the current processed node has a subtree or any child, therefore we need to backtrack!
        if (process_backtrack) {
            // first before processing backtracking we unsign/remove the stored node of the
            // current processed path step, because we will also unsign/remove the stored nodes
            // of all path steps up to the //... path step and reassign all path steps with new ones.
            ps_context[ps_index] = NULL;
            ps_context_position[ps_index] = 0;

            // use a stack to store all nodes for the later done upward traversal of the XML tree
            // to keep correct child-parent order.
            stack < BufferNode * >st_ancestors = stack < BufferNode * >();

            // push the current processed node on to the stack.
            st_ancestors.push(nnode);

            // if the current processed node is the child-child of the previous processed path step
            // we go up to his parent because we are to deep to get any match. We first need to
            // reassign all previous path step up to the //... path step.
            if (nnode->parent && nnode->parent->parent == getPrevPSNode()) {
                nnode = nnode->parent;

                // also put this one on to the stack.
                st_ancestors.push(nnode);
            }
            // then we reduce the current processed path step index (ps_index) by one. Now we are
            // processing either the //.../... path step, strictly speaking the /... path step which
            // also takes care for the //... path step or we are processing a path step which has a
            // also a set backtracking value which is the case for path //.../.../.../... if the
            // current processed path step is/was the last one.
            ps_index--;

            // now we need to go upward in the XML tree and also need to reduce the path step index
            // (ps_index) by one as long as any path step has a set backtracking value. On upward
            // traversal of the XML tree we push all nodes on to the stack and also unsign/remove
            // all stored nodes for all backtracked path steps. There is no need to check if the
            // parents are NULL, because all previous processed path steps had assigned nodes, which
            // means, because of axis child, the parent nodes always exists.
            while (ps_back_track[ps_index]) {
                ps_context[ps_index] = NULL;
                ps_context_position[ps_index] = 0;
                ps_index--;
                nnode = nnode->parent;
                st_ancestors.push(nnode);
            }

            // now we are processing path step //.../..., strictly speaking the /... path step,
            // knowing all ancestor nodes of the last processed one up to child node of the
            // last matched/assigned node of the //... path step.
            // NOTE: because of the first decrement of the path step index (ps_index), the node
            // on top of the stack is NOT the one which has been assigned to the //... path step,
            // it is a child of this!

            // Alright, up to now nothing difficult happend, now let us start with a little bit
            // more difficult code.

            // first we make a copy of the stack in order not to loose/forget any node and to
            // make it, because of this fact, possible to call pop() on the original one.
            stack < BufferNode * >st_ancestors_cpy =
                stack < BufferNode * >(st_ancestors);

            // store the path step index (ps_index) of the current processed path step and of
            // the path step index (ps_index) of the //.../... path step, strictly speaking of
            // the /... path step.
            unsigned ps_index_old = ps_index;
            unsigned ps_index_desc = ps_index;

            // as we use recursion in this we need to reset the backtracking value of all interim
            // processed path step. This is done, because, if we would do full recursion it would
            // be too difficult to handle.
            bool ps_backtrack_reset;

            // we also need to reset the cur variable to NULL, which needs to be done, because of
            // the performed recursion in here and because of the recursion it is possible that we
            // invoke a recursive call with an already returned node and if it is so the recursive
            // invocation of this methode would break (see first if-case of this method).
            BufferNode *cur_old = cur;

            // now for every node on the stack we check if it matches the path steps, which has been
            // backtracked, in correct order, of course. If a node on the stack matches a path step
            // we recognize it by inspecting the path step index (ps_index) before and after recursive
            // invocation. As long as we are processing the //.../... path step, strictly speaking
            // the /... path step, we can always pop() the node(s) on top of BOTH stacks. But what
            // if we find a matching node for the //.../... path step but not for any path step
            // followed this path step?
            // For example a node matches the //.../... path step but not the follwing /... if the
            // path is //.../.../.... For this case we copied the stack in order to call pop() on
            // the original stack to pop the node on top of this stack. Then, if we got no match for
            // any following path step, the original stack is overwritten by the copied stack without
            // the node on top of the copied stack, because this node has already been checked for
            // matching. In this case it is also important to jump back immediately to the //.../...
            // path step, checking if the (new) node on top of the (new) original stack is matching. 
            while (!st_ancestors.empty()) {

                // reset backtracking value(s) to prevent invocation of this
                // (backtracking) if-case. 
                ps_backtrack_reset = false;
                if (ps_back_track[ps_index]) {
                    ps_back_track[ps_index] = false;
                    ps_backtrack_reset = true;
                }
                // also reset cur value.
                cur_old = cur;
                cur = NULL;

                ps_index_old = ps_index;

                nnode = isSatisfyingPath(st_ancestors.top());

                // undo reset of backtracking value(s).
                if (ps_backtrack_reset) {
                    ps_back_track[ps_index_old] = true;
                }
                // undo reset of cur value.
                cur = cur_old;

                if (nnode == NULL) {
                    if (ps_index >= path->getPathSize()) {
                        ps_index--;
                    }
                    // clear both stacks.
                    st_ancestors = stack < BufferNode * >();
                    st_ancestors_cpy = stack < BufferNode * >();
                    return NULL;
                } else if (ps_index_old < ps_index) {   // match
                    st_ancestors.pop();
                    if (!ps_back_track[ps_index_old]) {
                        st_ancestors_cpy.pop();
                    }
                } else {        // no match
                    if (!ps_back_track[ps_index]) {
                        st_ancestors_cpy.pop();
                    }
                    st_ancestors = stack < BufferNode * >(st_ancestors_cpy);
                    for (unsigned i = ps_index_desc; i < ps_index; i++) {
                        ps_context[i] = NULL;
                        ps_context_position[i] = 0;
                    }
                    ps_index = ps_index_desc;
                }
            }

            // clear both stacks
            st_ancestors = stack < BufferNode * >();
            st_ancestors_cpy = stack < BufferNode * >();

            // after execution of this we got the correct path step to be processed and also the node,
            // which is returned of method isSatisfyingPath(), which is last processed in the input
            // stream. The rest will be handled automatically...or by BACKTRACKING PART 1!
        }
    } else if (nnode && ps_skip_subtree[ps_index]) {
        // #############
        // SKIP SUBTREE:
        // #############
        // If a path step has been marked for skip subtree(s), we try to satisfy all path steps which
        // has been marked for skip to avoid expensive left-to-right traversal of the XML tree.
        bool ps_first_assigned = false;
        bool ps_first_null = false;

        while (ps_index >= 0 && ps_index < path->getPathSize()
               && ps_skip_subtree[ps_index] && !ps_first_null) {
            BufferNode *tmp = NULL;

            if (ps_context[ps_index]) {
                tmp = ps_context[ps_index];
            } else if (nnode->parent == getPrevPSNode()) {
                tmp = nnode;
            } else if (nnode->parent
                       && nnode->parent->parent == getPrevPSNode()) {
                tmp = nnode->parent;
            } else {
                BufferNode *nnode_tmp = getPrevPSNode();

                if (nnode_tmp->type == TYPE_TAG) {
                    while (((TagNode *) nnode_tmp->node)->children.isEmpty()
                           && !nnode_tmp->isClosed()) {
                        spp->readNext();
                    }
                    if (!((TagNode *) nnode_tmp->node)->children.isEmpty()) {
                        tmp = ((TagNode *) nnode_tmp->node)->children[0];
                    }
                }

                if (ps_index == 0 && ps_first_assigned) {
                    tmp = NULL;
                }
            }

            if (ps_index == 0) {
                ps_first_assigned = true;
            }

            if (tmp) {
                // if we got a node to switch/jump from to the next right sibling we read the input
                // stream as long as we got no matching right sibling for the processed path step.
                while (tmp && (tmp == ps_context[ps_index]
                               || !isMatchingNodeTestAndPredicates(tmp,
                                                                   ps_index))) {
                    while (tmp->r_sibling == NULL && tmp->parent
                           && !tmp->parent->isClosed()) {
                        spp->readNext();
                    }
                    tmp = tmp->r_sibling;
                }

                // if we found a next matching right sibling we assign it to the current processed
                // path step and increment the assignment node counter and the path step index (ps_index).
                // Also ensure to process the traversal of the XML tree from this node on by setting the
                // returned node of this method to the found matching node.
                // If we found no next matching right sibling, which is the case if either the switch/jump
                // node has no right sibling or all right siblings does not match the current processed
                // path step, we unsign/remove the stored node from the current processed path step and
                // reduced the path step index (ps_index) by one.
                // DO NOT UNSIGN/REMOVE ALL STORED NODES FROM ALL PATH STEPS in this case because the previous
                // processed path step of the current processed path step can also be assigned to a new node,
                // which then is the new contetx node for the (still) processed path step.
                if (tmp && tmp != ps_context[ps_index]
                    && isMatchingNodeTestAndPredicates(tmp, ps_index)) {
                    ps_context[ps_index] = tmp;
                    ps_context_position[ps_index] =
                        (ps_context_position[ps_index] + 1);
                    ps_index++;
                    pnode = tmp;
                } else {
                    ps_context[ps_index] = NULL;
                    ps_context_position[ps_index] = 0;
                    if (ps_index == 0 && ps_first_assigned) {
                        ps_first_null = true;
                    }
                    if (ps_index > 0) {
                        ps_index--;
                    }
                }
            } else {
                ps_context[ps_index] = NULL;
                ps_context_position[ps_index] = 0;
                if (ps_index == 0 && ps_first_assigned) {
                    ps_first_null = true;
                }
                if (ps_index > 0) {
                    ps_index--;
                }
            }
        }

        if (ps_first_null) {
            return NULL;
        }
    }
    // return NULL if the path is fully satisfied, which is the case if the current
    // processed path step index (ps_index) is >= the path size.
    if (ps_index >= path->getPathSize()) {
        ps_index--;
        return NULL;
    }

    return pnode;
}

bool BufferIterator::isMatchingNodeTestAndPredicates(BufferNode * nnode,
                                                     unsigned ps_idx) {
    if (nnode && ps_idx >= 0 && ps_idx < path->getPathSize()) {
        switch (path->getPathStepAt(ps_idx)->getNodeTestType()) {
            case ntt_tag:
                if (nnode->type == TYPE_TAG) {
                    return (path->getPathStepAt(ps_idx)->
                            isMatchingTag(((TagNode *) nnode->node)->tag)
                            && (!ps_attribute[ps_idx]
                                || ps_context_position[ps_idx] <=
                                ps_attribute_position[ps_idx]));
                }
                return false;
            case ntt_star:
                return (nnode->type == TYPE_TAG && (!ps_attribute[ps_idx]
                                                    ||
                                                    ps_context_position[ps_idx]
                                                    <=
                                                    ps_attribute_position
                                                    [ps_idx]));
            case ntt_node:
                return ((nnode->type == TYPE_TAG || nnode->type == TYPE_PCDATA)
                        && (!ps_attribute[ps_idx]
                            || ps_context_position[ps_idx] <=
                            ps_attribute_position[ps_idx]));
            case ntt_text:
                return (nnode->type == TYPE_PCDATA && (!ps_attribute[ps_idx]
                                                       ||
                                                       ps_context_position
                                                       [ps_idx] <=
                                                       ps_attribute_position
                                                       [ps_idx]));
        }
    }

    return false;
}

BufferNode *BufferIterator::getPrevPSNode() {
    return (ps_index == 0) ? base : ps_context[ps_index - 1];
}

void BufferIterator::unsignPSNode(BufferNode * nnode) {
    if (nnode) {
        if (nnode == ps_context[ps_index]) {
            // do not reduce in this case the current processed path step index (ps_index),
            // because we are still searching for a node which matches the current processed
            // path step. And also do not reset the node assignment counter for the current
            // processed path step (ps_context_position), because there has not been made a
            // new assignment for the previous processed path step.
            ps_context[ps_index] = NULL;
        }
        if (ps_index > 0 && nnode == ps_context[ps_index - 1]) {
            // if the node, which has been assigned to the previous processed path step has been
            // read on upward traversal of the XML tree it is allowed to reset the node assignment
            // counter for the current processed path step (ps_context_position) and also to reduce
            // the current processed path step index (ps_index) by one.
            ps_context_position[ps_index] = 0;
            ps_context[ps_index - 1] = NULL;
            ps_index--;
        }
    }
}
