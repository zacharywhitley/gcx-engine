
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
 *  @brief Implementations of header file projectiontreenode.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file projectiontreenode.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "projectiontreenode.h"

ProjectionTreeNode::ProjectionTreeNode(ProjectionTreeNode * _parent, int _var,
                                       PathExpression * _path, bool _dep,
                                       Role * _role,
                                       bool _direct_output):parent(_parent),
var(_var),
path(_path),
dep(_dep),
role(_role),
direct_output(_direct_output) {
}

ProjectionTreeNode::~ProjectionTreeNode() {
    for (unsigned i = 0; i < children.size(); i++) {
        delete children[i];
    }
}

bool ProjectionTreeNode::insertNode(int parent_var, int introduced_var,
                                    PathExpression * path, bool dep,
                                    Role * role, bool direct_output) {

    if (parent_var == var) {
        ProjectionTreeNode *child =
            new ProjectionTreeNode(this, introduced_var, path, dep, role,
                                   direct_output);

        children.push_back(child);

        return true;
    } else {
        bool created = false;

        for (unsigned i = 0; i < children.size() && !created; i++) {
            created =
                children[i]->insertNode(parent_var, introduced_var, path, dep,
                                        role, direct_output);
        }

        return created;
    }
}

void ProjectionTreeNode::print(OutputStream & dos) {
    print(dos, 0);
    dos << resetIndents();
}

PathExpression *ProjectionTreeNode::getRootPath() {
    vector < PathExpression * >paths;
    getPathsFromRoot(paths);

    PathExpression *r = new PathExpression();

    for (unsigned i = 0; i < paths.size(); i++) {
        PathExpression *cur = paths[i];

        if (cur && !cur->isEmptyPath()) {
            for (unsigned i = 0; i < cur->getPathSize(); i++) {
                r->addPathStep(cur->getPathStepAt(i)->clone());
            }
        }
    }

    return r;
}

void ProjectionTreeNode::getPathsFromRoot(vector < PathExpression * >&paths) {
    if (parent) {
        parent->getPathsFromRoot(paths);
    }
    paths.push_back(path);
}

void ProjectionTreeNode::removeUnneededNodes(PassiveProjectionTree * ppt) {
    for (unsigned i = 0; i < children.size(); i++) {
        if (children[i]->getPath()) {
            if (children[i]->getPath()->hasInnerTextNodeTest() &&
                !children[i]->getPath()->getPathStepAfterTextNodeTest()->
                isDosNodeStep()) {
                children[i]->removeSubtreeInclSelf(ppt);
                i--;
            } else if (children[i]->getPath()->hasTerminatingTextNodeTest()) {
                children[i]->removeSubtreeExclSelf(ppt);
            } else {
                children[i]->removeUnneededNodes(ppt);
            }
        } else {
            children[i]->removeUnneededNodes(ppt);
        }
    }
}

void ProjectionTreeNode::removeRedundantRoles(PassiveProjectionTree * ppt) {
    if (children.size() > 0 && !direct_output) {
        setRedundantRoleSelf();
    } else if (dep && isDosDepNode() && parent) {       // process only /dos::node()-dep. nodes
        vector < ProjectionTreeNode * >*siblings = parent->getChildren();

        // check if this /dos::node()-dependency node has at least one sibling,
        // which derives from the variable tree...
        bool perform_check = false;

        for (unsigned i = 0; i < siblings->size(); i++) {
            if (!(*siblings)[i]->isDepNode()) {
                perform_check = true;
                break;
            }
        }

        // ...if such a node exists, we perform a (expensive) deeper inspection
        // of all siblings of this /dos::node()-dependency node, checking, if we
        // could delete some siblings of this /dos::node() dependency node including
        // their subtrees...
        if (perform_check && parent->getVar() >= TAGID_ROOT) {
            for (unsigned i = 0; i < siblings->size(); i++) {
                if ((*siblings)[i] != this && !(*siblings)[i]->isDepNode()
                    && ((*siblings)[i]->getPath()
                        && (*siblings)[i]->getPath()->
                        isSemanticallyContainedIn(path))
                    || !(*siblings)[i]->getPath()) {
                    if (!(*siblings)[i]->getPath()) {
                        if (path->getPathSize() == 1
                            && path->getTailPathStep()->isDosNodeStep()) {
                            (*siblings)[i]->removeSubtreeInclSelf(ppt);
                            siblings = parent->getChildren();
                            i--;
                        }
                    } else {
                        (*siblings)[i]->removeSubtreeInclSelf(ppt);
                        siblings = parent->getChildren();
                        i--;
                    }
                }
            }
        }
    }

    for (unsigned i = 0; i < children.size(); i++) {
        children[i]->removeRedundantRoles(ppt);
    }
}

void ProjectionTreeNode::
registerToPassiveProjectionTree(PassiveProjectionTree * ppt) {
    PathExpression *root_to_cur = getRootPath();

    ppt->registerPath(root_to_cur);
    delete root_to_cur;
}

void ProjectionTreeNode::print(OutputStream & dos, unsigned indents) {
    dos << resetIndents() << incrementIndents(indents);
    dos << writeIndents();
    if (var > -1) {
        dos << "($" << VarName::getInstance()->getVarname(var) << ") => ";
    }

    if (path) {
        dos << *(path);
    } else {
        dos << "%/%";
    }

    if (role) {
        dos << " (r" << role->getId() << ",";
        switch (role->getType()) {
            case rt_root:
                dos << "root";
                break;
            case rt_variable:
                dos << "var";
                break;
            case rt_condition:
                dos << "cond";
                break;
            case rt_output:
                dos << "out";
                break;
        }
        dos << ",$" << VarName::getInstance()->getVarname(role->getBasingVar())
            << ",$" << VarName::getInstance()->getVarname(role->
                                                          getBasingFSA()) <<
            ")";
    } else {
        dos << "    <= redundant role/node";
    }

    if (direct_output) {
        dos << " *";
    }
    dos << NEWLINE;

    for (unsigned i = 0; i < children.size(); i++) {
        children[i]->print(dos, indents + 1);
    }
}

void ProjectionTreeNode::removeSubtreeInclSelf(PassiveProjectionTree * ppt) {
    removeSubtreeExclSelf(ppt);

    if (parent) {
        vector < ProjectionTreeNode * >*siblings = parent->getChildren();
        for (unsigned i = 0; i < siblings->size(); i++) {
            if ((*siblings)[i] == this) {
                if (!(*siblings)[i]->getRole() ||
                    RoleList::getInstance()->removeRole((*siblings)[i]->
                                                        getRole())) {
                    (*siblings)[i]->registerToPassiveProjectionTree(ppt);
                    delete(*siblings)[i];
                    siblings->erase(siblings->begin() + i);
                }
            }
        }
    } else {                    // ROOT
        if (!role || RoleList::getInstance()->removeRole(role)) {
            this->registerToPassiveProjectionTree(ppt);
            delete this;
        }
    }
}

void ProjectionTreeNode::removeSubtreeExclSelf(PassiveProjectionTree * ppt) {
    for (unsigned i = 0; i < children.size(); i++) {
        if (!children[i]->getRole()
            || RoleList::getInstance()->removeRole(children[i]->getRole())) {
            children[i]->removeSubtreeExclSelf(ppt);
            if (children.size() > 0) {
                children[i]->registerToPassiveProjectionTree(ppt);
                delete children[i];

                children.erase(children.begin() + i);
                i--;
            }
        }
    }
}

void ProjectionTreeNode::setRedundantRoleSelf() {
    if (!role || RoleList::getInstance()->removeRole(role)) {
        role = NULL;
    }
}
