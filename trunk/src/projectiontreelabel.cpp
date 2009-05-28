
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
 *  @brief Implementations of header file projectiontreelabel.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file projectiontreelabel.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "projectiontreelabel.h"
#include "varname.h"
#include "fsamap.h"
#include "pathsteptagexpression.h"

ProjectionTreeLabel::ProjectionTreeLabel(ProjectionTreeNode * _n,
                                         unsigned _position, unsigned _id,
                                         ProjectionTreeLabel *
                                         _same_level_successor,
                                         vector <
                                         ProjectionTreeLabel *
                                         >_child_successors,
                                         vector <
                                         ProjectionTreeLabel *
                                         >_self_successors):parent(NULL),
predecessor(NULL),
n(_n),
position(_position),
id(_id),
same_level_successor(_same_level_successor),
child_successors(_child_successors),
self_successors(_self_successors) {
}

ProjectionTreeLabel::~ProjectionTreeLabel() {
}

void ProjectionTreeLabel::
updateParentPredecessorInformation(ProjectionTreeLabel * _parent,
                                   ProjectionTreeLabel * _predecessor) {
    parent = _parent;
    predecessor = _predecessor;

    if (same_level_successor) {
        same_level_successor->updateParentPredecessorInformation(_parent, this);
    } else {
        for (unsigned i = 0; i < child_successors.size(); i++) {
            child_successors[i]->updateParentPredecessorInformation(this, this);
        }
    }
}

void ProjectionTreeLabel::print(OutputStream & dos) {

    dos << "[" << id << "] ";

    // print path and internal position info
    if (n->isRoot()) {
        dos << "path=<ROOT>";
    } else if (n->getPath()) {
        dos << "path=" << *n->getPath();
        dos << ", step=[" << position << "]";
    } else {
        dos << "path=%";
    }

    // print same_level successors
    dos << "; sl_succ=";
    if (same_level_successor) {
        dos << same_level_successor->getId();
    } else {
        dos << "%";
    }

    // print child successors
    dos << ", c_succ=[";
    for (unsigned i = 0; i < child_successors.size(); i++) {
        if (i)
            dos << ",";
        dos << child_successors[i]->getId();
    }
    dos << "]";

    // print self successors
    dos << ", self_succ=[";
    for (unsigned i = 0; i < self_successors.size(); i++) {
        if (i)
            dos << ",";
        dos << self_successors[i]->getId();
    }
    dos << "]";

    // print basing variable
    dos << " (";
    int var_id = getProjectionTreeNode()->getVar();

    if (var_id >= 0) {
        dos << "$" << VarName::getInstance()->getVarname(var_id);
    } else {
        dos << "%";
    }
    dos << ")";

    if (parent != NULL) {
        dos << " p=[" << parent->getId() << "]";
    }

    dos << NEWLINE;
}

ProjectionTreeLabel *ProjectionTreeLabel::getLeftmostSLPredecessor() {
    if (predecessor == parent) {
        return this;
    } else {
        return predecessor->getLeftmostSLPredecessor();
    }
}

unsigned ProjectionTreeLabel::getDependingVar() {
    int var = getProjectionTreeNode()->getVar();

    return var == -1 ? parent->getDependingVar() : (unsigned) var;
}

PathExpression *ProjectionTreeLabel::getPath() {
    return n->getPath();
}

PathStepExpression *ProjectionTreeLabel::getPathStep() {
    return n->getPath()? n->getPath()->getPathStepAt(position) : NULL;
}

TAG ProjectionTreeLabel::getTag() {
    if (n->isRoot())
        return TAGID_ROOT;

    // we abort if the path step is not a tag node test 
    // (the method should never have been called in this case)
    if (getPathStep()->isTagNodeTest()) {
        PathStepTagExpression *ptt = (PathStepTagExpression *) getPathStep();

        return ptt->getNodeTest();
    } else {
        throw RuntimeException("ProjectionTreeLabel: Internal Cast Error",
                               eid_runtime_ptlabel);
        return TAGID_ROOT;      // to avoid compiler warning
    }
}

AXIS_TYPE ProjectionTreeLabel::getAxisFollowingFSA() {
    return getAxisFollowingFSA(FSAMap::getInstance()->
                               getFSA(getDependingVar()));
}

AXIS_TYPE ProjectionTreeLabel::getAxisFollowingFSA(unsigned fsa) {
    return predecessor->getProjectionTreeNode()->getVar() == (int) fsa ?
        getPathStep()->getAxisType() : predecessor->getAxisFollowingFSA(fsa);
}

ProjectionTreeLabel *ProjectionTreeLabel::getFSALabel() {
    int var_fsa = FSAMap::getInstance()->getFSA(getDependingVar());

    return locateVarAncestorLabel(var_fsa);
}

bool ProjectionTreeLabel::isChildOrDescendantLabel() {
    if (!n->getPath())
        return false;
    return getPathStep()->getAxisType() == at_child ||
        getPathStep()->getAxisType() == at_descendant;
}

bool ProjectionTreeLabel::isDescendantLabel() {
    return n->getPath() && !n->getPath()->isEmptyPath()?
        getPathStep()->getAxisType() == at_descendant : false;
}

bool ProjectionTreeLabel::isChildLabel() {
    return n->getPath() && !n->getPath()->isEmptyPath()?
        getPathStep()->getAxisType() == at_child : false;
}

bool ProjectionTreeLabel::isDosLabel() {
    return n->getPath() && !n->getPath()->isEmptyPath()?
        n->getPath() && getPathStep()->getAxisType() == at_dos : false;
}

bool ProjectionTreeLabel::isDosNodeLabel() {
    return n->getPath() && !n->getPath()->isEmptyPath()?
        getPathStep()->isNodeNodeTest() &&
        getPathStep()->getAxisType() == at_dos : false;
}

bool ProjectionTreeLabel::isDosOrDescendantLabel() {
    return n->getPath() && !n->getPath()->isEmptyPath()?
        getPathStep()->getAxisType() == at_dos
        || getPathStep()->getAxisType() == at_descendant : false;
}

bool ProjectionTreeLabel::isNodeLabel() {
    return n->getPath() && !n->getPath()->isEmptyPath()?
        getPathStep()->isNodeNodeTest() : false;
}

bool ProjectionTreeLabel::isStarLabel() {
    return n->getPath() && !n->getPath()->isEmptyPath()?
        getPathStep()->isStarNodeTest() : false;
}

bool ProjectionTreeLabel::hasSameLevelOrChildSuccessors() {
    return same_level_successor || child_successors.size();
}

bool ProjectionTreeLabel::atEndOfPath() {
    return n->getPath()? n->getPath()->getPathSize() == position + 1 : true;
}

ProjectionTreeLabel *ProjectionTreeLabel::locateVarAncestorLabel(unsigned var) {
    if (getProjectionTreeNode()->getVar() == (int) var) {
        return this;
    } else {
        for (unsigned i = 0; i < self_successors.size(); i++) {
            if (self_successors[i]->getProjectionTreeNode()->getVar() ==
                (int) var) {
                return this;
            }
        }

        // else (i.e. no success)
        return parent->locateVarAncestorLabel(var);
    }
}

bool ProjectionTreeLabel::matchesTag(TAG t) {
    if (!n->getPath())
        return false;

    return getPathStep()->isMatchingTag(t);
}

bool ProjectionTreeLabel::matchesText() {
    return n->getPath()? getPathStep()->isTextNodeTest()
        || getPathStep()->isNodeNodeTest() : false;
}

bool ProjectionTreeLabel::descendantAxisBetw(unsigned basing_id,
                                             bool consider_final) {

    // if this is the basing_id label we are done
    if (id == basing_id) {
        if (consider_final) {
            return getPathStep()->getAxisType() == at_descendant ||
                getPathStep()->getAxisType() == at_dos;
        } else {
            return false;
        }
    } else if (getPathStep()->getAxisType() == at_descendant ||
               getPathStep()->getAxisType() == at_dos) {
        return true;            // we found such an axis
    } else {
        return getPredecessor()->descendantAxisBetw(basing_id, consider_final);
    }
}
