
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
 *  @brief Header file for pathstepexpression.cpp.
 *  @details Header file specifying constructors, destructor and functions for pathstepexpression.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef PATHSTEPEXPRESSION_H
#define PATHSTEPEXPRESSION_H

#include "expression.h"
#include "pathstepattribute.h"

/*! @def WEIGHT_AXIS_CHILD
 *  @brief Weight definition for axis <tt>child</tt> used to determine the evaluation order
 *    of SignOffExpression if more than one appear in a series.
 *  @details Weight definition for axis <tt>child</tt> used to determine the evaluation order
 *    of SignOffExpression if more than one appear in a series.
 */
#define WEIGHT_AXIS_CHILD        1

/*! @def WEIGHT_AXIS_DESCENDANT
 *  @brief Weight definition for axis <tt>descendant</tt> used to determine the evaluation order
 *    of SignOffExpression if more than one appear in a series.
 *  @details Weight definition for axis <tt>descendant</tt> used to determine the evaluation order
 *    of SignOffExpression if more than one appear in a series.
 */
#define WEIGHT_AXIS_DESCENDANT    3

/*! @def WEIGHT_AXIS_DOS
 *  @brief Weight definition for axis <tt>dos</tt> used to determine the evaluation order
 *    of SignOffExpression if more than one appear in a series.
 *  @details Weight definition for axis <tt>dos</tt> used to determine the evaluation order
 *    of SignOffExpression if more than one appear in a series.
 */
#define WEIGHT_AXIS_DOS            3

/*! @def WEIGHT_INNER_NODETEST
 *  @brief Weight definition for all inner node tests of a path used to determine the evaluation order
 *    of SignOffExpression if more than one appear in a series.
 *  @details Weight definition for all inner node tests used to determine the evaluation order
 *    of SignOffExpression if more than one appear in a series.
 */
#define WEIGHT_INNER_NODETEST    1

/*! @class PathStepExpression
 *  @brief Represents base class of all path step expressions.
 *  @details Represents base class of all path step expressions.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class PathStepExpression:public Expression {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for an EXP_TYPE, an AXIS_TYPE and
     *                      a PathStepAttribute if required.
     *  @param[in] _type EXP_TYPE.
     *  @param[in] _axis AXIS_TYPE.
     *  @param[in] _attribute Pointer to a PathStepAttribute object.
     */
    PathStepExpression(EXP_TYPE _type, AXIS_TYPE _axis,
                       PathStepAttribute * _attribute);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ PathStepExpression();

    /*! @fn virtual void print(OutputStream& dos) const
     *  @brief Prints the corresponding path step expression.
     *  @details Prints the corresponding path step expression.
     *  @param[in] dos Pointer to a OutputStream object.
     *  @retval void
     */
    virtual void print(OutputStream & dos) const = 0;

    /*! @fn inline AXIS_TYPE getAxisType()
     *  @brief Returns the AXIS_TYPE.
     *  @details Returns the AXIS_TYPE, i.e. return the member variable.
     *  @retval AXIS_TYPE.
     */
    inline AXIS_TYPE getAxisType() {
        return axis;
    }
    /*! @fn inline void setAxisType(AXIS_TYPE _axis)
     *  @brief Sets the AXIS_TYPE.
     *  @details Sets the AXIS_TYPE, i.e. sets the member variable.
     *  @param[in] _axis AXIS_TYPE.
     *  @retval void
     */ inline void setAxisType(AXIS_TYPE _axis) {
        axis = _axis;
    }

    /*! @fn NODETEST_TYPE getNodeTestType()
     *  @brief Returns the NODETEST_TYPE.
     *  @details Returns the NODETEST_TYPE.
     *  @retval NODETEST_TYPE.
     */
    NODETEST_TYPE getNodeTestType();

    /*! @fn inline PathStepAttribute* getAttribute()
     *  @brief Returns the attribute of this path step expression.
     *  @details Returns the attribute of this path step expression.
     *                      The returned PathStepAttribute might be <tt>NULL</tt>.
     *  @retval PathStepAttribute* Pointer to a PathStepAttribute object.
     */
    inline PathStepAttribute *getAttribute() {
        return attribute;
    }

    /*! @fn inline void setAttribute(PathStepAttribute* _attribute)
     *  @brief Sets the attribute of this path step expression.
     *  @details Sets the attribute of this path step expression.
     *  @param[in] _attribute Pointer to a PathStepAttribute object.
     *  @retval void
     */
    inline void setAttribute(PathStepAttribute * _attribute) {
        attribute = _attribute;
    }

    /*! @fn unsigned getStepWeight(bool is_last_step)
     *  @brief Returns the weight of this path step expression.
     *  @details Returns the weight of this path step expression.
     *  @param[in] is_last_step Indicator if this path step expression is the last one, i.e.
     *                      <tt>true</tt> if it is the last one, <tt>false</tt> otherwise.
     *  @retval unsigned Weight of this path step expression.
     */
    unsigned getStepWeight(bool is_last_step);

    /*! @fn inline bool isTagNodeTest()
     *  @brief Returns if this path step expression is of the form <tt>/tag</tt>.
     *  @details Returns if this path step expression is of the form <tt>/tag</tt>.
     *  @retval bool <tt>true</tt> if this path step expression is of the form <tt>/tag</tt>,
     *                      <tt>false</tt> otherwise.
     */
    inline bool isTagNodeTest() {
        return type == et_pathsteptag;
    }

    /*! @fn inline bool isStarNodeTest()
     *  @brief Returns if this path step expression is of the form <tt>/</tt><tt>*</tt>.
     *  @details Returns if this path step expression is of the form <tt>/</tt><tt>*</tt>.
     *  @retval bool <tt>true</tt> if this path step expression is of the form <tt>/</tt><tt>*</tt>,
     *                      <tt>false</tt> otherwise.
     */
    inline bool isStarNodeTest() {
        return type == et_pathstepstar;
    }

    /*! @fn inline bool isNodeNodeTest()
     *  @brief Returns if this path step expression is of the form <tt>/node</tt><tt>()</tt>.
     *  @details Returns if this path step expression is of the form <tt>/node</tt><tt>()</tt>.
     *  @retval bool <tt>true</tt> if this path step expression is of the form <tt>/node</tt><tt>()</tt>,
     *                      <tt>false</tt> otherwise.
     */
    inline bool isNodeNodeTest() {
        return type == et_pathstepnode;
    }

    /*! @fn inline bool isTextNodeTest()
     *  @brief Returns if this path step expression is of the form <tt>/text</tt><tt>()</tt>.
     *  @details Returns if this path step expression is of the form <tt>/text</tt><tt>()</tt>.
     *  @retval bool <tt>true</tt> if this path step expression is of the form <tt>/text</tt><tt>()</tt>,
     *                      <tt>false</tt> otherwise.
     */
    inline bool isTextNodeTest() {
        return type == et_pathsteptext;
    }


    /*! @fn inline bool isDosNodeStep()
     *  @brief Returns if this path step expression is of the form <tt>/dos</tt><tt>:</tt><tt>:</tt><tt>node</tt><tt>()</tt>.
     *  @details Returns if this path step expression is of the form <tt>/dos</tt><tt>:</tt><tt>:</tt><tt>node</tt><tt>()</tt>.
     *  @retval bool <tt>true</tt> if this path step expression is of the form <tt>/dos</tt><tt>:</tt><tt>:</tt><tt>node</tt><tt>()</tt>,
     *                      <tt>false</tt> otherwise.
     */
    inline bool isDosNodeStep() {
        return axis == at_dos && type == et_pathstepnode;
    }

    /*! @fn bool isSyntacticallyEqualTo(PathStepExpression* ps)
     *  @brief Returns if this path step expression is syntactically equal to the entered path step expression.
     *  @details Returns if this path step expression is syntactically equal to the entered path step expression.
     *  @param[in] ps Pointer to a PathStepExpression object (path step expression to be compared).
     *  @retval bool <tt>true</tt> if this path step expression is syntactically equal to the entered path step expression,
     *                      <tt>false</tt> otherwise.
     */
    bool isSyntacticallyEqualTo(PathStepExpression * ps);

    /*! @fn virtual bool isMatchingTag(TAG tag)
     *  @brief Returns if the correspnding path step expression is matching the entered TAG.
     *  @details Returns if the correspnding path step expression is matching the entered TAG.
     *  @param[in] tag TAG.
     *  @retval bool <tt>true</tt> if the correspnding path step expression is matching the entered TAG,
     *                      <tt>false</tt> otherwise.
     */
    virtual bool isMatchingTag(TAG tag) = 0;

    /*! @fn inline bool hasAttribute()
     *  @brief Returns if this path step expression contains an PathStepAttribute.
     *  @details Returns if this path step expression contains an PathStepAttribute.
     *  @retval bool <tt>true</tt> if this path step expression contains an PathStepAttribute,
     *                      <tt>false</tt> otherwise.
     */
    inline bool hasAttribute() {
        return attribute != NULL;
    }

    /*! @fn virtual PathStepExpression* clone()
     *  @brief Returns a new, independent copy of a path step expression or of this object respectively.
     *  @details Returns a new, independent copy of a path step expression or of this object respectively.
     *  @retval PathStepExpression* Copy of a path step expression or of this object respectively.
     */
    virtual PathStepExpression *clone() = 0;

    /*! @fn virtual PathStepExpression* cloneWithoutAttributes()
     *  @brief Returns a new, independent copy of a path step expression or of this object respectively
     *                  <tt>without</tt> any PathStepAttribute.
     *  @details Returns a new, independent copy of a path step expression or of this object respectively
     *                  <tt>without</tt> any PathStepAttribute.
     *  @retval PathStepExpression* Copy of a path step expression or of this object respectively
     *                  <tt>without</tt> any PathStepAttribute.
     */
    virtual PathStepExpression *cloneWithoutAttributes() = 0;

  protected:
    /*! @var AXIS_TYPE axis
     *  @brief The type of axis for this path step expression.
     *  @details The type of axis for this path step expression, which is the second argument of the constructor.
     */
    AXIS_TYPE axis;

    /*! @var PathStepAttribute* attribute
     *  @brief The attribute associated with this path step expression.
     *  @details The attribute associated with this path step expression, which is the third argument of the constructor.
     *                      This member variable may be <tt>NULL</tt>.
     */
    PathStepAttribute *attribute;
};

#endif // PATHSTEPEXPRESSION_H
