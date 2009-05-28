
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
 *  @brief Implementations of header file variabletreenode.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file variabletreenode.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "variabletreenode.h"

VariableTreeNode::VariableTreeNode():
parent(NULL), var(TAGID_ROOT), direct_output(false) {
}

VariableTreeNode::VariableTreeNode(VariableTreeNode * _parent, unsigned _var,
                                   bool _direct_output):parent(_parent),
var(_var),
direct_output(_direct_output) {
}

VariableTreeNode::~VariableTreeNode() {
    for (unsigned i = 0; i < children.size(); i++) {
        delete children[i];
    }
}

bool VariableTreeNode::insertNode(unsigned parent_var,
                                  unsigned introduced_var, bool direct_output) {
    if (parent_var == var) {
        VariableTreeNode *child =
            new VariableTreeNode(this, introduced_var, direct_output);
        children.push_back(child);

        return true;
    } else {
        bool created = false;

        for (unsigned i = 0; i < children.size() && !created; i++) {
            created =
                children[i]->insertNode(parent_var, introduced_var,
                                        direct_output);
        }

        return created;
    }
}

void VariableTreeNode::print(OutputStream & dos) {
    print(dos, 0);
    dos << resetIndents();
}

void VariableTreeNode::print(OutputStream & dos, unsigned indents) {
    dos << resetIndents() << incrementIndents(indents);
    dos << writeIndents();
    dos << "($" << VarName::getInstance()->getVarname(var) << ")";
    if (direct_output) {
        dos << " *";
    }
    dos << NEWLINE;
    for (unsigned i = 0; i < children.size(); i++) {
        children[i]->print(dos, indents + 1);
    }
}
