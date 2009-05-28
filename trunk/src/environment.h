
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
 *  @brief Header file for environment.cpp.
 *  @details Header file specifying constructors, destructor and functions for environment.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <map>
#include "typeenums.h"
#include "buffernode.h"
#include "varname.h"

/*! @typedef map<unsigned, BufferNode*> Env
 *  @brief The underlying map of this object.
 *  @details The underlying map of this object, which maps variables to a BufferNode.
 */
typedef map < unsigned,
 BufferNode * >Env;

/*! @class Environment
 *  @brief Represents (current) binding of variables to a buffer node.
 *  @details Represents (current) binding of variables to a buffer node, which takes therefore
 *    a variable and a BufferNode as arguments.
 *  @note Variables are internally represented by numbers!
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class Environment {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object.
     */
    Environment();

    /*! @brief Constructor.
     *  @details Constructor - creating object for a BufferNode.
     *  @param[in] _root Pointer to a BufferNode object.
     */
    Environment(BufferNode * _root);

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ Environment();

    /*! @fn void insertNodeBinding(unsigned var, BufferNode* node)
     *  @brief Inserts (or sets) a new variable with its (current) binding to a BufferNode.
     *  @details Inserts (or sets) a new variable with its (current) binding to a BufferNode.
     *  @param[in] var Variable for which (current) binding holds.
     *  @param[in] node Pointer to a BufferNode object.
     *  @retval void
     */
    void insertNodeBinding(unsigned var, BufferNode * node);

    /*! @fn void print(OutputStream & dos)
     *  @brief Prints all variables with their (current) binding to a BufferNode.
     *  @details Prints all variables with their (current) binding to a BufferNode.
     *  @param[in] dos Pointer to a OutputStream object.
     *  @retval void
     */
    void print(OutputStream & dos);

    /*! @fn BufferNode* getNodeBinding(unsigned var)
     *  @brief Returns the (current) binding to a BufferNode for the entered variable.
     *  @details Returns the (current) binding to a BufferNode for the entered variable.
     *  @param[in] var Variable for which (current) binding should be obtained. 
     *  @retval BufferNode* Pointer to a BufferNode object.
     */
    BufferNode *getNodeBinding(unsigned var);

    /*! @fn Environment* clone()
     *  @brief Returns a new, independent copy of all variables with their (current) binding to a BufferNode
     *    or of this object respectively.
     *  @details Returns a new, independent copy of all variables with their (current) binding to a BufferNode
     *    or of this object respectively.
     *  @retval Environment* Pointer to a Environment object.
     */
    Environment *clone();

  private:
    /*! @var Env env
     *  @brief The underlying map of this object.
     *  @details The underlying map of this object, which maps variables to a BufferNode.
     */
    Env env;
};

#endif // ENVIRONMENT_H
