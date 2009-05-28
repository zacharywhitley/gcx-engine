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
 * 	@brief Header file for pathstepattributeposition.cpp.
 * 	@details Header file specifying constructors, destructor and functions for pathstepattributeposition.cpp.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef PATHSTEPATTRIBUTEPOSITION_H
#define PATHSTEPATTRIBUTEPOSITION_H

#include "pathstepattribute.h"

/*! @class PathStepAttributePosition
 * 	@brief Represents a XQuery attribute for a single XPath pathstep of the form <tt>[position</tt><tt>()=n]</tt>.
 * 	@details Represents a XQuery attribute for a single XPath pathstep of the form <tt>[position</tt><tt>()=n]</tt>
 * 			(whereas <tt>n</tt> denotes a (numeric) position of an item in a sequence),	which
 * 			takes a position <tt>n</tt> as argument.
 *  @note The information <tt>[position</tt><tt>()=n]</tt> or <tt>[position</tt><tt>()=1]</tt> respectively is only used
 * 			internally to determine that only the first	item in a sequence needs to be kept.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 *  @license Software License Agreement (BSD License)
 */
class PathStepAttributePosition : public PathStepAttribute {

public:
	/*! @brief Constructor.
     * 	@details Constructor - creating object for position <tt>n</tt>.
     * 	@param[in] _position Position <tt>n</tt> of an item in a sequence.
     */
	PathStepAttributePosition(unsigned _position);
	
    /*! @brief Destructor.
     * 	@details Destructor.
     */
	virtual ~PathStepAttributePosition();

	/*! @fn void print(ostream& o) const
	 *  @copydoc PathStepAttribute::print(ostream& o) const
	 */
	void print(ostream& o) const;
	
    /*! @fn inline unsigned getPosition()
     *  @brief Returns the position <tt>n</tt> of an attribute.
     *  @details Returns the position <tt>n</tt> of an attribute, i.e. returns the member variable.
     *  @retval unsigned Position <tt>n</tt> of an item in a sequence.
	 */
	inline unsigned getPosition() { return position; }
	
	/*! @fn PathStepAttributePosition* clone()
	 *  @copydoc PathStepAttribute::clone()
	 */
	PathStepAttributePosition* clone();

private:
	/*! @var unsigned position
	 *  @brief The entered position.
	 * 	@details The entered position, which is the argument of the constructor and determines
	 * 			the position <tt>n</tt> of an item in a sequence.
	 */
	unsigned position;
};

#endif // PATHSTEPATTRIBUTEPOSITION_H
