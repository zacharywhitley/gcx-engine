
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
 *  @brief Header file for dependencytuple.cpp.
 *  @details Header file specifying constructors, destructor and functions for dependencytuple.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef DEPENDENCYTUPLE_H
#define DEPENDENCYTUPLE_H

#include "pathexpression.h"
#include "passiveprojectiontree.h"

/*! @class DependencyTuple
 *  @brief Represents a single tuple of a set of dependency-tuples.
 *  @details Represents a single tuple of a set of dependency-tuples, i.e. of a DependencySet, which takes a
 *    PathExpression, e.g. <tt>/bib//book</tt> or <tt>/bib/book/title</tt>, a Role, and a boolean
 *    value, which indicates that this single tuple results from a CondExpression, as arguments.             
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class DependencyTuple {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for a PathExpression and a boolean value.
     *  @param[in] _path Pointer to a PathExpression object.
     *  @param[in] _cond <tt>true</tt> if this single tuple results from a CondExpression, <tt>false</tt> otherwise.
     */
    DependencyTuple(PathExpression * _path, bool _cond);

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ DependencyTuple();

    /*! @fn void print(OutputStream& dos)
     *  @brief Prints this single tuple.
     *  @details Prints this single tuple.
     *  @param[in] dos Pointer to a OutputStream object.
     *  @retval void
     */
    void print(OutputStream & dos);

    /*! @fn inline PathExpression* getPath()
     *  @brief Returns the path of this single tuple.
     *  @details Returns the path of this single tuple, i.e. returns the member variable.
     *  @retval PathExpression* Pointer to a PathExpression object.
     */
    inline PathExpression *getPath() {
        return path;
    }
    /*! @fn inline void setRole(Role* _role)
     *  @brief Sets the role for this single tuple.
     *  @details Sets the role for this single tuple, i.e. sets the member variable.
     *  @param[in] _role Pointer to a Role object.
     *  @retval void
     */ inline void setRole(Role * _role) {
        role = _role;
    }

    /*! @fn inline bool isCondTuple()
     *  @brief Returns if this single tuple results from a CondExpression.
     *  @details Returns if this single tuple results from a CondExpression, i.e. returns the member variable.
     *  @retval bool <tt>true</tt> if this single tuple results from a CondExpression, <tt>false</tt> otherwise.
     */
    inline bool isCondTuple() {
        return cond;
    }

    /*! @fn bool isPosTuple()
     *  @brief Returns if this single tuple is a <tt>[positions</tt><tt>()=1]</tt> tuple.
     *  @details Returns if this single tuple is a <tt>[positions</tt><tt>()=1]</tt> tuple.
     *  @retval bool <tt>true</tt> if this single tuple is a <tt>[positions</tt><tt>()=1]</tt> tuple, <tt>false</tt> otherwise.
     */
    bool isPosTuple();

    /*! @fn bool isDosTuple()
     *  @brief Returns if this single tuple is a <tt>/dos</tt><tt>:</tt><tt>:</tt><tt>node</tt><tt>()</tt> tuple.
     *  @details Returns if this single tuple is a <tt>/dos</tt><tt>:</tt><tt>:</tt><tt>node</tt><tt>()</tt> tuple.
     *  @retval bool <tt>true</tt> if this single tuple is a <tt>/dos</tt><tt>:</tt><tt>:</tt><tt>node</tt><tt>()</tt> tuple, <tt>false</tt> otherwise.
     */
    bool isDosTuple();

    /*! @fn bool isSyntacticallyEqualTo(DependencyTuple* tuple)
     *  @brief Returns if this single tuple is syntactically equal to the entered single tuple.
     *  @details Returns if this single tuple is syntactically equal to the entered single tuple with respect to their paths.
     *  @param[in] tuple Pointer to a DependencyTuple object.
     *  @retval bool <tt>true</tt> if this single tuple if syntactically equal to the entered single tuple with respect to their paths, <tt>false</tt> otherwise.
     */
    bool isSyntacticallyEqualTo(DependencyTuple * tuple);

    /*! @fn bool isSemanticallyContainedIn(DependencyTuple* tuple)
     *  @brief Returns if this single tuple is semantically contained in the entered single tuple.
     *  @details Returns if this single tuple is semantically contained in the entered single tuple, i.e. this single tuple is sematically
     *    contained if it locate a greater or greater equal set of nodes of an arbitrarily XML tree than the entered single tuple.
     *  @param[in] tuple Pointer to a DependencyTuple object.
     *  @retval bool <tt>true</tt> if this single tuple is semantically contained in the entered single tuple, <tt>false</tt> otherwise.
     */
    bool isSemanticallyContainedIn(DependencyTuple * tuple);

    /*! @fn void registerToPassiveProjectionTree(unsigned var, PassiveProjectionTree* ppt)
     *  @brief Register the path described by this single tuple to the entered passive projection tree.
     *  @details Register the path described by this single tuple to the entered passive projection tree.
     *  @param[in] var The basing variable.
     *  @param[in] ppt Pointer to a PassiveProjectionTree object.
     *  @retval void
     */
    void registerToPassiveProjectionTree(unsigned var,
                                         PassiveProjectionTree * ppt);

  private:
    /*! @var PathExpression* path
     *  @brief The entered PathExpression.
     *  @details The entered PathExpression, which is the first argument of the constructor.
     */
    PathExpression * path;

    /*! @var bool cond
     *  @brief The entered boolean value.
     *  @details The entered boolean value, which is the second argument of the constructor.
     */
    bool cond;

    /*! @var Role* role
     *  @brief The set Role of this single tuple.
     *  @details The set Role of this single tuple, which is <tt>NULL</tt> on creation of object
     *    and will be set while constructing the ProjectionTree.
     */
    Role *role;
};

#endif // DEPENDENCYTUPLE_H
