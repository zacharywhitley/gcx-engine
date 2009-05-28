
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
 *  @brief Implementations of header file passiveprojectiontreenode.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file passiveprojectiontreenode.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "passiveprojectiontreenode.h"

PassiveProjectionTreeNode::PassiveProjectionTreeNode():
parent(NULL), label(NULL) {
}

PassiveProjectionTreeNode::PassiveProjectionTreeNode(PassiveProjectionTreeNode * _parent, PathStepExpression * _label):
parent(_parent), label(_label) {
}

PassiveProjectionTreeNode::~PassiveProjectionTreeNode() {
    for (unsigned i = 0; i < children.size(); i++) {
        delete children[i];
    }
    delete label;
}

void PassiveProjectionTreeNode::registerPath(PathExpression * path,
                                             unsigned pos) {

    // seek for a child with the current path step expression as its label
    for (unsigned i = 0; i < children.size(); i++) {
        PathStepExpression *lc = children[i]->getLabel();
        PathStepExpression *lp = path->getPathStepAt(pos);

        if (lc->isSyntacticallyEqualTo(lp)) {
            if (pos < path->getPathSize() - 1) {
                return children[i]->registerPath(path, ++pos);
            } else {
                return;         // reached end of path step 
            }
        }
    }

    // if no child with path step label exists, insert the node
    PassiveProjectionTreeNode *new_child = new PassiveProjectionTreeNode(this,
                                                                         path->
                                                                         getPathStepAt
                                                                         (pos)->
                                                                         clone
                                                                         ());
    children.push_back(new_child);
    if (pos < path->getPathSize() - 1) {
        new_child->registerPath(path, ++pos);
    }
}

void PassiveProjectionTreeNode::print(OutputStream & dos) {
    print(dos, 0);
    dos << resetIndents();
}

void PassiveProjectionTreeNode::print(OutputStream & dos, unsigned indents) {
    dos << resetIndents() << incrementIndents(indents);
    dos << writeIndents();
    if (!label) {
        dos << "/";
    } else {
        dos << *label;
    }
    dos << NEWLINE;
    for (unsigned i = 0; i < children.size(); i++) {
        children[i]->print(dos, indents + 1);
    }
}
