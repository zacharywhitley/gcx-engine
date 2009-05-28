/*
 | Author: Michael Schmidt;
 |         Gunnar Jehl (multi-step paths/aggregate functions/optimizations) 
 ===========================================================================
 ===========================================================================
 | Software License Agreement (BSD License)
 | 
 | Copyright (c) 2006-2007, Saarland University Database Group 
 | All rights reserved.
 |
 | Redistribution and use of this software in source and binary forms,
 | with or without modification, are permitted provided that the following
 | conditions are met:
 | 
 | * Redistributions of source code must retain the above
 |   copyright notice, this list of conditions and the
 |   following disclaimer.
 |
 | * Redistributions in binary form must reproduce the above
 |   copyright notice, this list of conditions and the
 |   following disclaimer in the documentation and/or other
 |   materials provided with the distribution.
 |
 | * Neither the name the of Saarland University Database Group nor the names 
 |   of its contributors may be used to endorse or promote products derived 
 |   from this software without specific prior written permission of the
 |   Saarland University Database Group.
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
*/
/*! @file
 *     @brief Header file for pathstepexpression.cpp.
 *     @details Header file specifying constructors, destructor and functions for pathstepexpression.cpp.
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
#ifndef PATHSTEPEXPRESSION_H
#define PATHSTEPEXPRESSION_H

#include "expression.h"
#include "pathstepattribute.h"

/*! @def WEIGHT_AXIS_CHILD
 *  @brief Weight definition for axis <tt>child</tt> used to determine the evaluation order
 *             of SignOffExpression if more than one appear in a series.
 *  @details Weight definition for axis <tt>child</tt> used to determine the evaluation order
 *             of SignOffExpression if more than one appear in a series.
 */
#define WEIGHT_AXIS_CHILD        1

/*! @def WEIGHT_AXIS_DESCENDANT
 *  @brief Weight definition for axis <tt>descendant</tt> used to determine the evaluation order
 *             of SignOffExpression if more than one appear in a series.
 *  @details Weight definition for axis <tt>descendant</tt> used to determine the evaluation order
 *             of SignOffExpression if more than one appear in a series.
 */
#define WEIGHT_AXIS_DESCENDANT    3

/*! @def WEIGHT_AXIS_DOS
 *  @brief Weight definition for axis <tt>dos</tt> used to determine the evaluation order
 *             of SignOffExpression if more than one appear in a series.
 *  @details Weight definition for axis <tt>dos</tt> used to determine the evaluation order
 *             of SignOffExpression if more than one appear in a series.
 */
#define WEIGHT_AXIS_DOS            3

/*! @def WEIGHT_INNER_NODETEST
 *  @brief Weight definition for all inner nodetests of a path used to determine the evaluation order
 *             of SignOffExpression if more than one appear in a series.
 *  @details Weight definition for all inner nodetests used to determine the evaluation order
 *             of SignOffExpression if more than one appear in a series.
 */
#define WEIGHT_INNER_NODETEST    1

/*! @class PathStepExpression
 *  @brief Abstract base class for path step expressions.
 *     @details The base clase defines elements that are common to
 *              all path step expressions, in particular attributes
 *              and an axis.
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
class PathStepExpression : public Expression {

public:
    /*! @brief Constructor.
     *  @details Constructor - initializes member variables of the base class.
     *  @param[in] _type The type of the PathStepExpression (either a tag, star
     *                   text, or node expression).
     *  @param[in] _axis The axis specification.
     *  @param[in] _attribute An attribute specification, might be NULL.
     */
    PathStepExpression(EXP_TYPE _type, AXIS_TYPE _axis,
                        PathStepAttribute* _attribute);

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~PathStepExpression();

    /*! @brief Returns the axis type.
     *  @details
     *  @retval AXIS_TYPE
     */
    inline AXIS_TYPE getAxisType() { return axis; }

    /*! @brief Sets the axis type.
     *  @details
     *  @param[in] _axis The axis type to be specified.
     *  @retval void
     */
    inline void setAxisType(AXIS_TYPE _axis) { axis=_axis; }
    
    /*! @brief Returns the type of the path step expression.
     *  @details
     *  @retval NODETEST_TYPE
     */
    NODETEST_TYPE getNodeTestType();
    
    /*! @brief Returns the attribute of the path step expression. 
     *  @details The attribute might be NULL (which means true()).
     *  @retval PathStepAttribute*
     */
    inline PathStepAttribute* getAttribute() { return attribute; }

    /*! @brief Assigns an attribute to the path step expression.
     *  @details
     *  @param[in] _attribute The attribute to be set.
     *  @retval void
     */
    inline void setAttribute(PathStepAttribute* _attribute) { attribute=_attribute; }
    
    /*! @brief Returns true if the path step has an attribute associated.
     *  @details An example of such an attribute might be "position()=1".
     *  @pretval bool True if an attribute is associated, false otherwise.
     */
    inline bool hasAttribute() { return attribute!=NULL ; }
    
    /*! @brief Returns true if the path step is a tag path step
     *  @details
     *  @retval bool
     */
    inline bool isTagNodeTest() { return type==et_pathsteptag; }

    /*! @brief Returns true if the path step is a star-carrying path step
     *  @details
     *  @retval bool
     */
    inline bool isStarNodeTest() { return type==et_pathstepstar; }

    /*! @brief Returns true if the path step is a node()-carrying path step
     *  @details
     *  @retval bool
     */
    inline bool isNodeNodeTest() { return type==et_pathstepnode; }

    /*! @brief Returns true if the path step is a text()-carrying path step
     *  @details
     *  @retval bool
     */
    inline bool isTextNodeTest() { return type==et_pathsteptext; } 
    
    /*! @brief Returns true if the path step is a descendant-or-self::node()
     *         path step.
     *  @details
     *  @retval bool
     */
    inline bool isDosNodeStep() { return axis==at_dos && type==et_pathstepnode; }
    
    /*! @brief Checks syntactical equivalence towards another path step
     *  @details
     *  @param[in] ps The path step that is compared to this object.
     *  @retval bool
     */
    bool isSyntacticallyEqualTo(PathStepExpression* ps);
    
    // TODO: document this!!!
    unsigned getStepWeight(bool is_last_step);
    
    /*! @brief Checks if this path step matches the passed tag
     *  @details Abstract method reimplemented in the inherited classes.
     *  @param[in] tag The tag for which matching is checked.
     *  @retval bool
     */
    virtual bool isMatchingTag(TAG tag)=0;
    
    /*! @brief Prints the path step expression.
     *  @details Abstract method reimplemented in the inherited classes.
     *  @param[in] o The output stream to print the path step to.
     *  @retval void
     */
    virtual void print(ostream& o) const=0;
    
    /*! @brief Clones the path step expression.
     *  @details Abstract method reimplemented in the inherited classes.
     *  @retval PathStepExpression* The cloned path step expression.
     */
    virtual PathStepExpression* clone()=0;
    
    /*! @brief Clones the path step expression while ignoring its attributes.
     *  @details Abstract method reimplemented in the inherited classes.
     *  @retval PathStepExpression* The cloned path step expression.
     */
    virtual PathStepExpression* cloneWithoutAttributes()=0;
    
protected:
    /*! @var AXIS_TYPE axis
     *  @brief The axis type of the path step expression.
     */
    AXIS_TYPE axis;    

    /*! @var PathStepAttribute* attribute
     *  @brief The attribute associated with the path step expression.
     *  @details NULL is used as an equivalent for attribute true()
     */
    PathStepAttribute* attribute;
};

#endif // PATHSTEPEXPRESSION_H
