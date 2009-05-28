
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
 *  @brief Header file for pathexpression.cpp.
 *  @details Header file specifying constructors, destructor and functions for pathexpression.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef PATHEXPRESSION_H
#define PATHEXPRESSION_H

#include "expression.h"
#include "pathexpressionadornment.h"
#include "pathsteptagexpression.h"

// forward declarations
class PathExpressionAdornment;

/*! @class PathExpression
 *  @brief Represents an XQuery path expression.
 *  @details Represents an XQuery path expression, which takes a number of path step
 *  		expressions as argument.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class PathExpression:public Expression {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for stepwise addition of path step expressions.
     */
    PathExpression();

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ PathExpression();

    /*! @fn inline void addPathStep(PathStepExpression* ps)
     *  @brief Appends a new path step expression to the end of this path expression.
     *  @details Appends a new path step expression to the end of this path expression.
     *  @param[in] ps Pointer to a PathStepExpression object.
     *  @retval void
     */
    inline void addPathStep(PathStepExpression * ps) {
        pathsteps.push_back(ps);
    }
    /*! @fn virtual void print(OutputStream& dos) const
     *  @copydoc Expression::print(OutputStream& dos) const
     */ virtual void print(OutputStream & dos) const;

    /*! @fn inline vector<PathStepExpression*>* getPathSteps()
     *  @brief Returns all path step expressions of this path expression.
     *  @details Returns all path step expressions of this path expression, i.e. returns the member variable.
     *  @retval vector<PathStepExpression*>* Pointer to a vector containing PathStepExpression objects.
     */
    inline vector < PathStepExpression * >*getPathSteps() {
        return &pathsteps;
    }

    /*! @fn inline unsigned getPathSize()
     *  @brief Returns the (current) path expression size.
     *  @details Returns the (current) path expression size, i.e. returns the number of path step expressions.
     *  @retval unsigned Number of path step expressions of a path expression.
     */
    inline unsigned getPathSize() {
        return pathsteps.size();
    }

    /*! @fn inline PathStepExpression* getPathStepAt(unsigned i)
     *  @brief Returns the path step expression of a path expression at a specific position.
     *  @details Returns the path step expression of a path expression at a specific position.
     *  @param[in] i Position at which the path step expression should be obtained. 
     *  @retval PathStepExpression* Pointer to a PathStepExpression object.
     */
    inline PathStepExpression *getPathStepAt(unsigned i) {
        return pathsteps[i];
    }

    /*! @fn PathStepExpression* getTailPathStep()
     *  @brief Returns the last path step expression of a path expression.
     *  @details Returns the last path step expression of a path expression. 
     *  @retval PathStepExpression* Pointer to a PathStepExpression object.
     */
    PathStepExpression *getTailPathStep();

    /*! @fn PathStepExpression* getPathStepAfterTextNodeTest()
     *  @brief Returns the path step expression after the first <tt>text()</tt> node test of a path expression.
     *  @details Returns the path step expression after the first <tt>text()</tt> node test of a path expression. 
     *  @retval PathStepExpression* Pointer to a PathStepExpression object.
     */
    PathStepExpression *getPathStepAfterTextNodeTest();

    /*! @fn unsigned getWeight()
     *  @brief Returns the weight as a whole of a path expression.
     *  @details Returns the weight as a whole of a path expression.
     *  @retval unsigned Weight as a whole of a path expression.
     */
    unsigned getWeight();

    /*! @fn PathExpressionAdornment* getAdornment()
     *  @brief Returns the path expression adornments.
     *  @details Returns the path expression adornments.
     *  @retval PathExpressionAdornment* Pointer to a PathExpressionAdornment object.
     */
    PathExpressionAdornment *getAdornment();

    /*! @fn inline bool isEmptyPath()
     *  @brief Returns if this path expression is empty.
     *  @details Returns if this path expression is empty, i.e. returns if this path
     *                  expression contains no path step expression.
     *  @retval bool <tt>true</tt> if this path expression contains no path step expression,
     *          <tt>false</tt> otherwise.
     */
    inline bool isEmptyPath() {
        return pathsteps.size() == 0;
    }

    /*! @fn bool isDosNodePath()
     *  @brief Returns if this path expression has a <tt>dos</tt><tt>:</tt><tt>:</tt><tt>node</tt><tt>()</tt>
     *                  PathStepExpression at his end.
     *  @details Returns if this path expression has a <tt>dos</tt><tt>:</tt><tt>:</tt><tt>node</tt><tt>()</tt>
     *                  PathStepExpression at his end.
     *  @retval bool <tt>true</tt> f this path expression has a <tt>dos</tt><tt>:</tt><tt>:</tt><tt>node</tt><tt>()</tt>
     *                  PathStepExpression at his end, <tt>false</tt> otherwise.
     */
    bool isDosNodePath();

    /*! @fn bool isSyntacticallyEqualTo(PathExpression* path)
     *  @brief Returns if this path expression is syntactically equal to the entered path expression.
     *  @details Returns if this path expression is syntactically equal to the entered path expression.
     *  @param[in] path Pointer to a PathExpression object (path expression to be compared).
     *  @retval bool <tt>true</tt> if this path expression is syntactically equal to the entered path expression,
     *                      <tt>false</tt> otherwise.
     */
    bool isSyntacticallyEqualTo(PathExpression * path);

    /*! @fn bool isSemanticallyContainedIn(PathExpression* path)
     *  @brief Returns if this path expression is semantically contained in the entered path expression.
     *  @details Returns if this path expression is semantically contained in the entered path expression.
     *  @note The current implementations does not consider PathStepAttribute and therefore
     *              invoking this method on any path expression would maybe lead to wrong results!
     *  @param[in] path Pointer to a PathExpression object (path expression to be compared).
     *  @retval bool <tt>true</tt> if this path expression is semantically contained in the entered path expression,
     *                      <tt>false</tt> otherwise.
     */
    bool isSemanticallyContainedIn(PathExpression * path);

    /*! @fn bool hasInnerTextNodeTest()
     *  @brief Returns if the path expression has an inner <tt>text()</tt> node test PathStepExpression.
     *  @details Returns if the path expression has an inner <tt>text()</tt> node test PathStepExpression. 
     *  @retval bool <tt>true</tt> if the path expression has an inner <tt>text()</tt> node test PathStepExpression,
     *                  <tt>false</tt> otherwise.
     */
    bool hasInnerTextNodeTest();

    /*! @fn bool hasTerminatingTextNodeTest()
     *  @brief Returns if the path expression has an <tt>text()</tt> node test PathStepExpression at his end.
     *  @details Returns if the path expression has an <tt>text()</tt> node test PathStepExpression at his end. 
     *  @retval bool <tt>true</tt> if the path expression has an <tt>text()</tt> node test PathStepExpression at his end,
     *                  <tt>false</tt> otherwise.
     */
    bool hasTerminatingTextNodeTest();

    /*! @fn bool hasFollowingDescendantOrDosFrom(unsigned ps_idx)
     *  @brief Returns if the path expression has a following <tt>/dos</tt> or <tt>/descendant</tt> axis from a specific position on.
     *  @details Returns if the path expression has a following <tt>/dos</tt> or <tt>/descendant</tt> axis from a specific position on.
     *  @param[in] ps_idx Position at which the path step expression should be checked.
     *  @retval bool <tt>true</tt> if the path expression has a following <tt>/dos</tt> or <tt>/descendant</tt> axis from a specific position on,
     *                  <tt>false</tt> otherwise.
     */
    bool hasFollowingDescendantOrDosFrom(unsigned ps_idx);

    /*! @fn bool hasPreviousDescendantOrDosUpTo(unsigned ps_idx)
     *  @brief Returns if the path expression has a previous <tt>/dos</tt> or <tt>/descendant</tt> axis from a specific position on.
     *  @details Returns if the path expression has a previous <tt>/dos</tt> or <tt>/descendant</tt> axis from a specific position on.
     *  @param[in] ps_idx Position at which the path step expression should be checked.
     *  @retval bool <tt>true</tt> if the path expression has a previous <tt>/dos</tt> or <tt>/descendant</tt> axis from a specific position on,
     *                  <tt>false</tt> otherwise.
     */
    bool hasPreviousDescendantOrDosUpTo(unsigned ps_idx);

    /*! @fn bool selectsNoNode()
     *  @brief Returns if the path expression selects/locates no nodes of an arbitrary XML tree.
     *  @details Returns if the path expression selects/locates no nodes of an arbitrary XML tree.
     *  @retval bool <tt>true</tt> if the path expression selects/locates no nodes of an arbitrary XML tree,
     *                  <tt>false</tt> otherwise.
     */
    bool selectsNoNode();

    /*! @fn bool containsStarDescendantSequence(unsigned pos)
     *  @brief Returns if the path expression contains a sequence of wildcard <tt>/</tt><tt>*</tt> node tests from a specific position on.
     *  @details Returns if the path expression contains a sequence of wildcard <tt>/</tt><tt>*</tt> node tests from a specific position on.
     *  @param[in] pos Position at which the path step expression should be checked.
     *  @retval bool <tt>true</tt> if the path expression contains a sequence of wildcard <tt>/</tt><tt>*</tt> node tests from a specific position on,
     *                  <tt>false</tt> otherwise.
     */
    bool containsStarDescendantSequence(unsigned pos);

    /*! @fn bool mightHasChildDescendantConflict(PathExpression* path)
     *  @brief Checks if there is a child-descendant conflict that forces the path step
     * to be kept anyway, i.e. such that no projection can be applied.
     *  @details A simple example for such a child-descendant conflict are two path
     * expressions /a/d and //d. Here, such a conflict arises: when we
     * read the document starting sequence &lt;a&gt;&lt;c&gt; for instance, we are
     * not allowed to project the &lt;c&gt;-tag away, although it is not matched by
     * any of the projection paths. The reason is simple: consider the document
     * &lt;a&gt;&lt;c&gt;&lt;d/&gt;&lt;/c&gt;&lt;/a&gt; and the query
     * &lt;q&gt;{ &lt;one&gt;{ /a/d }&lt;/one&gt;, &lt;two&gt;{ //d  }&lt;/two&gt; }&lt;/q&gt;.
     * It is easy to see that this query gives different results when evaluated
     * on documents &lt;a&gt;&lt;c&gt;&lt;d/&gt;&lt;/c&gt;&lt;/a&gt; and
     * &lt;a&gt;&lt;d/&gt;&lt;/a&gt;. We denote this situation as child-descendant
     * conflict, and in such cases the &lt;c&gt;-tag cannot be projected
     * away, as witnessed by the above example. We refer the interested reader
     * to the paper for more details.
     *  @param[in] path Pointer to a PathExpression object.
     *  @retval bool <tt>true</tt> if the current path has a child-descendant conflict with
     *    the passed path, <tt>false</tt> otherwise.
     */
    bool mightHasChildDescendantConflict(PathExpression * path);

    /*! @fn void replacePathStepAt(unsigned idx, PathStepExpression* ps)
     *  @brief Replaces a path step expression at a specific position of a path expression by a new path step expression.
     *  @details Replaces a path step expression at a specific position of a path expression by a new path step expression.
     *  @param[in] idx Position at which replacement should take place.
     *  @param[in] ps Pointer to a PathStepExpression object.
     *  @retval void
     */
    void replacePathStepAt(unsigned idx, PathStepExpression * ps);

    /*! @fn PathExpression* clone()
     *  @brief Returns a new, independent copy of a path expression or of this object respectively.
     *  @details Returns a new, independent copy of a path expression or of this object respectively.
     *  @retval PathExpression* Copy of a path expression or of this object respectively.
     */
    PathExpression *clone();

    /*! @fn PathExpression* cloneWithoutFinalDosNodeAndAttributes()
     *  @brief Returns a new, independent copy of a path expression or of this object respectively
     *                  <tt>without</tt> an ending <tt>/dos</tt> PathStepExpression or any PathStepAttribute.
     *  @details Returns a new, independent copy of a path expression or of this object respectively
     *                  <tt>without</tt> an ending <tt>/dos</tt> PathStepExpression or any PathStepAttribute.
     *  @retval PathExpression* Copy of a path expression or of this object respectively
     *                  <tt>without</tt> an ending <tt>/dos</tt> PathStepExpression or any PathStepAttribute.
     */
    PathExpression *cloneWithoutFinalDosNodeAndAttributes();

  private:
    /*! @var vector<PathStepExpression*> pathsteps
     *  @brief The path step expressions of this path expressions.
     *  @details The path step expressions of this path expressions.
     */
    vector < PathStepExpression * >pathsteps;

    /*! @var PathExpressionAdornment* adornment
     *  @brief The adornments associated with this path expression.
     *  @details The adornments associated with this path expression.
     */
    PathExpressionAdornment *adornment;
};

#endif // PATHEXPRESSION_H
