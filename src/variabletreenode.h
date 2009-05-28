
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
 *  @brief Header file for variabletreenode.cpp.
 *  @details Header file specifying constructors, destructor and functions for variabletreenode.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef VARIABLETREENODE_H
#define VARIABLETREENODE_H

#include <vector>
#include "varname.h"
#include "outputstreammanip.h"

//! @namespace std
using namespace std;

/*! @class VariableTreeNode
 *  @brief Represents a single node of the constructed variable tree.
 *  @details Represents a single node of the constructed variable tree, i.e. of the constructed VariableTree,
 *    which takes a VariableTreeNode as parent node and a variable as child node of this parent node
 *    as arguments.
 *  @note Variables are internally represented by numbers!
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class VariableTreeNode {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for variable <tt>\$root</tt>.
     */
    VariableTreeNode();

    /*! @brief Constructor.
     *  @details Constructor - creating object for a VariableTreeNode and a variable.
     *  @param[in] _parent Pointer to a VariableTreeNode object (parent node).
     *  @param[in] _var Variable to be inserted as child node (of the entered parent node).
     *  @param[in] _direct_output <tt>true</tt> if the associated for-loop directly produces output,
     *                  <tt>false</tt> otherwise.
     */
    VariableTreeNode(VariableTreeNode * _parent, unsigned _var,
                     bool _direct_output);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ VariableTreeNode();

    /*! @fn bool insertNode(unsigned parent_var, unsigned introduced_var, bool direct_output)
     *  @brief Inserts a new node for a variable.
     *  @details Inserts a new node for a variable, which is appended as child of the first argument of this method.
     *  @param[in] parent_var Parent node of a new node.
     *  @param[in] introduced_var New node or variable respectively to be inserted.
     *  @param[in] direct_output <tt>true</tt> if the associated for-loop directly produces output,
     *                  <tt>false</tt> otherwise.
     *  @retval bool <tt>true</tt> if the new node was successfully inserted, <tt>false</tt> otherwise.
     */
    bool insertNode(unsigned parent_var, unsigned introduced_var,
                    bool direct_output);

    /*! @fn void print(OutputStream& dos)
     *  @brief Prints this single VariableTreeNode.
     *  @details Prints this single VariableTreeNode.
     *  @param[in] dos Pointer to a OutputStream object.
     *  @retval void
     */
    void print(OutputStream & dos);

    /*! @fn inline unsigned getVar()
     *  @brief Returns the (numeric value of a) variable of a VariableTreeNode.
     *  @details Returns the (numeric value of a) variable of a VariableTreeNode, i.e. returns the member variable.
     *  @retval unsigned Numeric value of a variable.
     */
    inline unsigned getVar() {
        return var;
    }
    /*! @fn inline vector<VariableTreeNode*>* getChildren()
     *  @brief Returns all children of a VariableTreeNode.
     *  @details Returns all children of a VariableTreeNode, i.e. returns the member variable.
     *  @retval vector<VariableTreeNode*>* Pointer to a vector object containing a pointer to VariableTreeNode objects.
     */ inline vector < VariableTreeNode * >*getChildren() {
        return &children;
    }

    /*! @fn inline bool isDirectOutput()
     *  @brief Returns if the for-loop introducing the variable generates direct output.
     *  @details Returns if the for-loop introducing the variable generates direct output.
     *  @retval bool <tt>true</tt> if the for-loop introducing the variable generates direct output,
     *                  <tt>false</tt> otherwise.
     */
    inline bool isDirectOutput() {
        return direct_output;
    }

  private:
    /*! @fn void print(OutputStream& dos, unsigned indents)
     *  @brief Prints this single VariableTreeNode with whitespace indenting.
     *  @details Prints this single VariableTreeNode with whitespace indenting.
     *  @param[in] dos Pointer to a OutputStream object.
     *  @param[in] indents Number of whitespaces to put before output or of a VariableTreeNode respectively.
     *  @retval void
     */
    void print(OutputStream & dos, unsigned indents);

    /*! @var VariableTreeNode* parent
     *  @brief The entered VariableTreeNode (parent node).
     *  @details The entered VariableTreeNode (parent node), which is the first argument of the constructor.
     */
    VariableTreeNode *parent;

    /*! @var unsigned var
     *  @brief The entered variable.
     *  @details The entered variable, which is the second argument of the constructor and internally represented by a number.
     */
    unsigned var;

    /*! @var vector<VariableTreeNode*> children
     *  @brief All child nodes of a VariableTreeNode.
     *  @details All child nodes of a VariableTreeNode.
     */
    vector < VariableTreeNode * >children;

    /*! @var bool direct_output
     *  @brief Indicator if the for-loop introducing the variable generates direct output.
     *  @details Indicator if the for-loop introducing the variable generates direct output.
     */
    bool direct_output;
};

#endif // VARIABLETREENODE_H
