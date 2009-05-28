
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
 *  @brief Header file for variabletree.cpp.
 *  @details Header file specifying constructors, destructor and functions for variabletree.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef VARIABLETREE_H
#define VARIABLETREE_H

#include "variabletreenode.h"
#include "parvarmap.h"

/*! @class VariableTree
 *  @brief Represents the constructed variable tree.
 *  @details Represents the constructed variable tree, which is constructed through ParVarMap and is needed for
 *    constructing the ProjectionTree.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class VariableTree {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for variable <tt>\$root</tt>.
     */
    VariableTree();

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ VariableTree();

    /*! @fn void insertNode(unsigned parent_var, unsigned introduced_var, bool direct_output)
     *  @brief Inserts a new node into the variable tree.
     *  @details Inserts a new node into the variable tree, which is appended as child of the first argument of this method.
     *  @param[in] parent_var Parent node of the new node.
     *  @param[in] introduced_var New node or variable respectively to be inserted.
     *  @param[in] direct_output Indicator if new node or variable respectively has direct output in its corresponding ForExpression,
     *    i.e. <tt>true</tt> if direct output is present, <tt>false</tt> otherwise.
     *  @retval void
     */
    void insertNode(unsigned parent_var, unsigned introduced_var,
                    bool direct_output);

    /*! @fn void print(OutputStream& dos)
     *  @brief Prints the complete variable tree.
     *  @details Prints the complete variable tree.
     *  @param[in] dos Pointer to a OutputStream object.
     *  @retval void
     */
    void print(OutputStream & dos);

    /*! @fn inline VariableTreeNode* getRootNode()
     *  @brief Returns the root node of the variable tree.
     *  @details Returns the root node of the variable tree, i.e. returns the member variable,
     *    which is always variable <tt>\$root</tt>.
     *  @retval VariableTreeNode* Pointer to a VariableTreeNode object.
     */
    inline VariableTreeNode *getRootNode() {
        return root;
  } private:
    /*! @var VariableTreeNode* root
     *  @brief The root node of the variable tree.
     *  @details The root node of the variable tree, which is always variable <tt>\$root</tt>.
     */
     VariableTreeNode * root;
};

#endif // VARIABLETREE_H
