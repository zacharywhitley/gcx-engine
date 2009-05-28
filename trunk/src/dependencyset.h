
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
 *  @brief Header file for dependencyset.cpp.
 *  @details Header file specifying constructors, destructor and functions for dependencyset.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef DEPENDENCYSET_H
#define DEPENDENCYSET_H

#include <vector>
#include "dependencytuple.h"
#include "pathstepnodeexpression.h"
#include "pathstepattributeposition.h"

/*! @class DependencySet
 *  @brief Represents a set of dependency-tuples for a variable.
 *  @details Represents a set of dependency-tuples for a variable, i.e. a set of DependencyTuple for a variable,
 *    e.g. <tt>dep(\$x) = {\</bib//book/dos</tt><tt>:</tt><tt>:</tt><tt>node</tt><tt>(), r<sub>i</sub>\>, \</bib/book/title[positions</tt><tt>()=1], r<sub>j</sub>\>}</tt>
 *    (whereas <tt>i != j</tt>), which are attached to the VariableTree as child nodes of this variable. This attachment together with the extension of all VariableTreeNode
 *    by their corresponding PathExpression results in the ProjectionTree.  
 *  @note Variables are internally represented by numbers!
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class DependencySet {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for a variable (with no DependencyTuple).
     *  @param[in] _var Variable for which this set of dependency-tuples holds.
     */
    DependencySet(unsigned _var);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ DependencySet();

    /*! @fn void insertTuple(PathExpression* path, bool pos, bool dos, bool cond)
     *  @brief Inserts a new tuple to this set of dependency-tuples.
     *  @details Inserts a new tuple to this set of dependency-tuples, i.e. creates a new DependencyTuple for this set of dependency-tuples.
     *  @param[in] path Pointer to a PathExpression object.
     *  @param[in] pos <tt>true</tt> if this new tuple should be a <tt>[positions</tt><tt>()=1]</tt> dependency-tuple, <tt>false</tt> otherwise.
     *  @param[in] dos <tt>true</tt> if this new tuple should be a <tt>/dos</tt><tt>:</tt><tt>:</tt><tt>node</tt><tt>()</tt> dependency-tuple, <tt>false</tt> otherwise.
     *  @param[in] cond <tt>true</tt> if this new tuple results from a CondExpression, <tt>false</tt> otherwise.
     *  @retval void
     */
    void insertTuple(PathExpression * path, bool pos, bool dos, bool cond);

    /*! @fn void print(OutputStream& dos)
     *  @brief Prints this set of dependency-tuples.
     *  @details Prints this set of dependency-tuples.
     *  @param[in] dos Pointer to a OutputStream object.
     *  @retval void
     */
    void print(OutputStream & dos);

    /*! @fn inline unsigned getVar()
     *  @brief Returns the variable of this set of dependency-tuples.
     *  @details Returns the variable of this set of dependency-tuples, i.e. returns the member variable.
     *  @retval unsigned Variable of this set of dependency-tuples.
     */
    inline unsigned getVar() {
        return var;
    }
    /*! @fn vector<DependencyTuple*>* getTuples()
     *  @brief Returns all dependency-tuples for this variable.
     *  @details Returns all dependency-tuples for this variable, i.e. returns the member variable.
     *  @retval vector<DependencyTuple*>* Pointer to a vector object containing pointer of DependencyTuple objects.
     */ vector < DependencyTuple * >*getTuples() {
        return &tuples;
    }

    /*! @fn inline bool hasTuples()
     *  @brief Returns if this variable has a set of dependency-tuples.
     *  @details Returns if this variable has a set of dependency-tuples, i.e. returns <tt>true</tt>
     *    if this variable has at least one DependencyTuple, <tt>false</tt> otherwise.
     *  @retval bool <tt>true</tt> if this variable has at least one DependencyTuple, <tt>false</tt> otherwise.
     */
    inline bool hasTuples() {
        return tuples.size() > 0;
    }

    /*! @fn void removeSyntacticallyEqualTuple(PassiveProjectionTree* ppt)
     *  @brief Removes all tuples of this set dependency-tuples that are syntactically equal.
     *  @details Removes all tuples of this set dependency-tuples that are syntactically equal
     *    with respect to their paths.
     *  @param[in] ppt Pointer to a PassiveProjectionTree object.
     *    All deleted paths are registered to the passive projection tree.
     *  @retval void
     */
    void removeSyntacticallyEqualTuple(PassiveProjectionTree * ppt);

    /*! @fn void removeSemanticallyContainedTuple(PassiveProjectionTree* ppt)
     *  @brief Removes all tuples of this set dependency-tuples that are semantically
     *    contained among each other.
     *  @details Removes all tuples of this set dependency-tuples that are semantically
     *    contained among each other, i.e. those tuples are kept that locate a greater
     *    or a greater equal set of nodes of an arbitrarily XML tree than any other
     *    tuple of this set of dependency-tuples.
     *  @param[in] ppt Pointer to a PassiveProjectionTree object.
     *    All deleted paths are registered to the passive projection tree.
     *  @retval void
     */
    void removeSemanticallyContainedTuple(PassiveProjectionTree * ppt);

  private:
    /*! @var unsigned var
     *  @brief The entered variable.
     *  @details The entered variable, which is the argument of the constructor and internally
     *    represented by a number.
     */
    unsigned var;

    /*! @var vector<DependencyTuple*> tuples
     *  @brief The set of dependency-tuples for this variable.
     *  @details The set of dependency-tuples for this variable, which is a set of DependencyTuple.
     */
    vector < DependencyTuple * >tuples;
};

#endif // DEPENDENCYSET_H
