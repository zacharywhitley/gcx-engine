
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
 *  @brief Implementations of header file variabletree.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file variabletree.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "variabletree.h"

VariableTree::VariableTree():root(new VariableTreeNode) {
    ParVarMap *parvarmap = ParVarMap::getInstance();
    vector < unsigned >*varsq = parvarmap->getVarsq();

    vector < bool > *direct_output_info = parvarmap->getDirectOutputInfo();
    for (unsigned i = 1; i < varsq->size(); i++) {      // omit root variable!
        insertNode(parvarmap->getParVar((*varsq)[i]), (*varsq)[i],
                   (*direct_output_info)[i]);
    }
}

VariableTree::~VariableTree() {
    delete root;
}

void VariableTree::insertNode(unsigned parent_var, unsigned introduced_var,
                              bool direct_output) {
    root->insertNode(parent_var, introduced_var, direct_output);
}

void VariableTree::print(OutputStream & dos) {
    root->print(dos);
}
