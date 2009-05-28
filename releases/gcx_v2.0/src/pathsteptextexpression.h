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
 * 	@brief Header file for pathsteptextexpression.cpp.
 * 	@details Header file specifying constructors, destructor and functions for pathsteptextexpression.cpp.
 * 	@author Michael Schmidt
 * 	@author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef PATHSTEPTEXTEXPRESSION_H
#define PATHSTEPTEXTEXPRESSION_H

#include "pathstepexpression.h"

/*! @def WEIGHT_NODETEST_TEXT
 *  @brief Weight definition for nodetest <tt>text()</tt> used to determine the evaluation order
 * 			of SignOffExpression if more than one appear in a series.
 *  @details Weight definition for nodetest <tt>text()</tt> used to determine the evaluation order
 * 			of SignOffExpression if more than one appear in a series.
 */
#define WEIGHT_NODETEST_TEXT	4

/*! @class PathStepTextExpression
 * 	@brief Represents a single XPath pathstep of the form <tt>/text()</tt> or <tt>//text()</tt>.
 * 	@details Represents a single XPath pathstep of the form <tt>/text()</tt> or <tt>//text()</tt>,
 * 			which takes	an AXIS_TYPE (default: child-axis), e.g. <tt>/text()</tt> or <tt>//text()</tt>,
 * 			and a PathStepAttribute (if required), e.g. <tt>/text()[position</tt><tt>()=1]</tt> or <tt>//text()[position</tt><tt>()=1]</tt>,
 * 			as argument(s).
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
class PathStepTextExpression : public PathStepExpression {

public:
	/*! @brief Constructor.
     * 	@details Constructor - creating object for an AXIS_TYPE.
     * 	@param[in] _axis AXIS_TYPE.
     */
	PathStepTextExpression(AXIS_TYPE _axis);
	
	/*! @brief Constructor.
     * 	@details Constructor - creating object for an AXIS_TYPE and a PathStepAttribute.
     * 	@param[in] _axis AXIS_TYPE.
     *  @param[in] _attribute Pointer to a PathStepAttribute object.
     */
	PathStepTextExpression(AXIS_TYPE _axis, PathStepAttribute* _attribute);
	
	/*! @brief Constructor.
     * 	@details Constructor - creating object for (default) AXIS_TYPE <tt>child</tt>.
     */
    PathStepTextExpression();
    
    /*! @brief Destructor.
     * 	@details Destructor.
     */
    virtual ~PathStepTextExpression();

	/*! @fn virtual void print(ostream& o) const
	 *  @copydoc PathStepExpression::print(ostream& o) const
	 */
    virtual void print(ostream& o) const;
    
    /*! @fn inline bool isMatchingTag(TAG tag)
     *  @copydoc PathStepExpression::isMatchingTag(TAG tag)
     */
    inline bool isMatchingTag(TAG tag) { return false; }
	
	/*! @fn PathStepTextExpression* clone()
	 *  @copydoc PathStepExpression::clone()
	 */
	PathStepTextExpression* clone();

	/*! @fn PathStepNodeExpression* cloneWithoutAttributes()
	 *  @copydoc PathStepExpression::cloneWithoutAttributes()
	 */
    PathStepTextExpression* cloneWithoutAttributes();
};

#endif // PATHSTEPTEXTEXPRESSION_H
