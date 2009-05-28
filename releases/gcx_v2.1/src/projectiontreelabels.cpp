
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
 *  @brief Implementations of header file projectiontreelabels.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file projectiontreelabels.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "projectiontreelabels.h"

ProjectionTreeLabels::ProjectionTreeLabels(ProjectionTreeNode * root) {
    unsigned id = 0;            // root label has id 0 by default

    computeProjectionTreeLabels(root, id, 0);
}

ProjectionTreeLabels::~ProjectionTreeLabels() {
    for (unsigned i = 0; i < labels.size(); i++) {
        delete labels[i];
    }
}

void ProjectionTreeLabels::updateParentPredecessorInformation() {
    labels[0]->updateParentPredecessorInformation(NULL, NULL);
}

void ProjectionTreeLabels::print(OutputStream & dos) {
    for (unsigned i = 0; i < labels.size(); i++) {
        labels[i]->print(dos);
    }
}

void ProjectionTreeLabels::getAllRecursiveDosNodeSuccessors(unsigned id,
                                                            vector <
                                                            unsigned >&succ) {

    ProjectionTreeLabel *sls = getLabelById(id)->getSameLevelSuccessor();

    if (sls && sls->isDosNodeLabel()) {
        succ.push_back(sls->getId());
        getAllRecursiveDosNodeSuccessors(sls->getId(), succ);
    }

    vector < ProjectionTreeLabel * >*css =
        getLabelById(id)->getChildSuccessors();
    for (unsigned i = 0; i < css->size(); i++) {
        ProjectionTreeLabel *cur = (*css)[i];

        if (cur->isDosNodeLabel()) {
            succ.push_back(cur->getId());
            getAllRecursiveDosNodeSuccessors(cur->getId(), succ);
        }
    }
}

ProjectionTreeLabel *ProjectionTreeLabels::
computeProjectionTreeLabels(ProjectionTreeNode * n, unsigned &id,
                            unsigned cur_position) {

    // reserve a position in the labels vector (just to maintain ordering)
    labels.push_back(NULL);

    unsigned this_id = id;
    unsigned this_position = cur_position;

    ProjectionTreeLabel *same_level_successor = NULL;

    vector < ProjectionTreeLabel * >self_successors;
    vector < ProjectionTreeLabel * >child_successors;

    // horizontal advancement
    if (n->getPath() && ++cur_position < n->getPath()->getPathSize()) {
        same_level_successor =
            computeProjectionTreeLabels(n, ++id, cur_position);

        // vertical advancement 
    } else {
        vector < ProjectionTreeNode * >*children = n->getChildren();
        for (unsigned i = 0; i < children->size(); i++) {
            ProjectionTreeLabel *child_label =
                computeProjectionTreeLabels((*children)[i], ++id, 0);

            // if the path step of the child's projection tree node is
            // different from NULL, we have a standard child with path step
            if (child_label->getProjectionTreeNode()->getPath()) {
                child_successors.push_back(child_label);

                // else the corresponding path step describes exactly
                // the same path (i.e. is self-axis)
            } else {
                // In this case, we first ...
                // ... add the child to the self-successors
                self_successors.push_back(child_label);
                // ... as well as the child's self-successors
                vector < ProjectionTreeLabel * >*child_self_successors =
                    child_label->getSelfSuccessors();
                for (unsigned j = 0; j < child_self_successors->size(); j++) {
                    self_successors.push_back((*child_self_successors)[j]);
                }

                // and second add the self-successor's child-successors
                // to the set of child successors
                vector < ProjectionTreeLabel * >*child_child_successors =
                    child_label->getChildSuccessors();
                for (unsigned j = 0; j < child_child_successors->size(); j++) {
                    child_successors.push_back((*child_child_successors)[j]);
                }

            }
        }
    }

    // finally set up the label for the current node/position-configuration
    ProjectionTreeLabel *l = new ProjectionTreeLabel(n, this_position, this_id,
                                                     same_level_successor,
                                                     child_successors,
                                                     self_successors);

    labels[this_id] = l;

    // return the label we just added
    return l;
}
