
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
 *  @brief Header file for role.cpp.
 *  @details Header file specifying constructors, destructor and functions for role.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef ROLE_H
#define ROLE_H

#include "typeenums.h"

// forward declarations
class PathExpression;
class SignOffExpression;
class OutputStream;

//! @namespace std
using namespace std;

/*! @class Role
 *  @brief Represents a single role.
 *  @details Represents a single role, which is removed from BufferNode by a SignOffExpression.
 *  @note Variables are internally represented by numbers!
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class Role {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for a role ID, a ROLE_TYPE, a basing variable,
     *                      a basing first straight ancestor (FSA) variable and two PathExpression.
     *  @param[in] _id Unique role ID.
     *  @param[in] _type ROLE_TYPE.
     *  @param[in] _basing_var Basing variable.
     *  @param[in] _basing_fsa Basing first straight ancestor (FSA) variable.
     *  @param[in] _rel_path Relative path.
     *  @param[in] _var_path Path of the variable.
     */
    Role(unsigned _id, ROLE_TYPE _type, unsigned _basing_var,
         unsigned _basing_fsa, PathExpression * _rel_path,
         PathExpression * _var_path);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ Role();

    /*! @fn void print(OutputStream& dos) const
     *  @brief Prints this single role.
     *  @details Prints this single role.
     *  @param[in] dos Pointer to a OutputStream object.
     *  @retval void
     */
    void print(OutputStream & dos) const;

    /*! @fn virtual void printExtended(OutputStream& dos)
     *  @brief Prints this single role with extended informations.
     *  @details Prints this single role with extended informations.
     *  @param[in] dos Pointer to a OutputStream object.
     *  @retval void
     */
    virtual void printExtended(OutputStream & dos);

    /*! @fn inline unsigned getId()
     *  @brief Returns the unique role ID.
     *  @details Returns the unique role ID, i.e. returns the member variable. 
     *  @retval unsigned Unique role ID of this single role.
     */
    inline unsigned getId() {
        return id;
    }
    /*! @fn inline void setId(unsigned _id)
     *  @brief Sets the unique role ID.
     *  @details Sets the unique role ID, i.e. sets the member variable.
     *  @param[in] _id New unique role ID. 
     *  @retval void
     */ inline void setId(unsigned _id) {
        id = _id;
    }

    /*! @fn inline ROLE_TYPE getType()
     *  @brief Returns the type of this single role.
     *  @details Returns the type of this single role.
     *  @retval ROLE_TYPE.
     */
    inline ROLE_TYPE getType() {
        return type;
    }

    /*! @fn inline unsigned getBasingVar()
     *  @brief Returns the basing variable of this single role.
     *  @details Returns the basing variable of this single role, i.e. returns the member variable.
     *  @retval unsigned Basing variable of this single role.
     */
    inline unsigned getBasingVar() {
        return basing_var;
    }

    /*! @fn inline unsigned getBasingFSA()
     *  @brief Returns the basing first straight ancestor (FSA) variable of this single role.
     *  @details Returns the basing first straight ancestor (FSA) variable of this single role, i.e. returns the member variable.
     *  @retval unsigned Basing first straight ancestor (FSA) variable of this single role.
     */
    inline unsigned getBasingFSA() {
        return basing_fsa;
    }

    /*! @fn inline PathExpression* getConcatenationPath()
     *  @brief Returns the concatenated path of this single role.
     *  @details Returns the concatenated path of this single role, which is the resulting PathExpression on
     *                  concatenating the relative path (rel_path) with the path of the variable (var_path).
     *  @retval PathExpression* Pointer to a PathExpression obejct.
     */
    inline PathExpression *getConcatenationPath() {
        return concat_path;
    }

    /*! @fn SignOffExpression* getSignOffExpression()
     *  @brief Returns the corresponding signOff-statement of this single role.
     *  @details Returns the corresponding signOff-statement of this single role.
     *  @retval SignOffExpression* Pointer to a SignOffExpression object.
     */
    SignOffExpression *getSignOffExpression();

    /*! @fn bool isPosRole()
     *  @brief Returns if this single role derives from a <tt>[position</tt><tt>()=1]</tt> PathStepAttributePosition.
     *  @details Returns if this single role derives from a <tt>[position</tt><tt>()=1]</tt> PathStepAttributePosition.
     *  @retval bool <tt>true</tt> if this single role derives from a <tt>[position</tt><tt>()=1]</tt> PathStepAttributePosition,
     *                  <tt>false</tt> otherwise.
     */
    bool isPosRole();

    /*! @fn bool isDosRole()
     *  @brief Returns if this single role derives from a <tt>/dos</tt><tt>:</tt><tt>:</tt><tt>node</tt><tt>()</tt> PathStepExpression.
     *  @details Returns if this single role derives from a <tt>/dos</tt><tt>:</tt><tt>:</tt><tt>node</tt><tt>()</tt> PathStepExpression.
     *  @retval bool <tt>true</tt> if this single role derives from a <tt>/dos</tt><tt>:</tt><tt>:</tt><tt>node</tt><tt>()</tt> PathStepExpression,
     *                  <tt>false</tt> otherwise.
     */
    bool isDosRole();

  private:
    /*! @var unsigned id
     *  @brief The unique role ID.
     *  @details The unique role ID, which is the first argument of the constructor.
     */
    unsigned id;

    /*! @var ROLE_TYPE type
     *  @brief The type of this single role.
     *  @details The type of this single role, which is the second argument of the constructor.
     */
    ROLE_TYPE type;

    /*! @var unsigned basing_var
     *  @brief The basing variable of this single role.
     *  @details The basing variable of this single role, which is the third argument of the constructor.
     */
    unsigned basing_var;

    /*! @var unsigned basing_fsa
     *  @brief The basing first straight ancestor (FSA) variable of this single role.
     *  @details The basing first straight ancestor (FSA) variable of this single role, which is the fourth argument of the constructor.
     */
    unsigned basing_fsa;

    /*! @var PathExpression* rel_path
     *  @brief The relative path of this single role.
     *  @details The relative path of this single role, which is the fifth argument of the constructor.
     */
    PathExpression *rel_path;

    /*! @var PathExpression* var_path
     *  @brief The path of the variable of this single role.
     *  @details The path of the variable of this single role, which is the sixth argument of the constructor.
     *              This member variable may be <tt>NULL</tt>.
     */
    PathExpression *var_path;

    /*! @var PathExpression* concat_path
     *  @brief The constructed concatenated path of this single role.
     *  @details The constructed concatenated path of this single role, which has been done in the constructor.
     */
    PathExpression *concat_path;
};

#endif // ROLE_H
