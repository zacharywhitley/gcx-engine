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
 * 	@brief Header file for pathstepattribute.cpp.
 * 	@details Header file specifying constructors, destructor and functions for pathstepattribute.cpp.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef PATHSTEPATTRIBUTE_H
#define PATHSTEPATTRIBUTE_H

#include <iostream>
#include "typeenums.h"

//! @namespace std
using namespace std;

/*! @class PathStepAttribute
 * 	@brief Represents base class of all attribute expressions. 
 * 	@details Represents base class of all attribute expressions.
 *  @note Currently there is only support for position()==nr
 *        attributes. They are used internally to determine that
 *        only the first item in a sequence needs to be kept.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
class PathStepAttribute {
	
public:
	/*! @brief Constructor.
	 * 	@details Constructor - creating object for a ATTRIBUTE_TYPE.
	 * 	@param[in] _type ATTRIBUTE_TYPE.
	 */
	PathStepAttribute(ATTRIBUTE_TYPE _type);
	
	/*! @brief Destructor.
	 * 	@details Destructor.
	 */
	virtual ~PathStepAttribute();
	
    /*! @fn virtual void print(ostream& o) const
     *  @brief
     *  @details
     * 	@param[in] o Reference to ostream object.
     *  @retval void
	 */
	virtual void print(ostream& o) const=0;
	
    /*! @fn inline const ATTRIBUTE_TYPE getType()
     *  @brief
     *  @details
     *  @retval ATTRIBUTE_TYPE ATTRIBUTE_TYPE.
	 */
	inline const ATTRIBUTE_TYPE getType() { return type; }
	
    /*! @fn virtual PathStepAttribute* clone()
     *  @brief
     *  @details
     *  @retval PathStepAttribute* Pointer to a PathStepAttribute object.
	 */
	virtual PathStepAttribute* clone()=0;
	
private:
	/*! @var ATTRIBUTE_TYPE type
	 *  @brief The entered type.
	 * 	@details The entered type, which is the argument of the constructor and determines
	 * 			the ATTRIBUTE_TYPE type of the corresponding attribute object.
	 */
	ATTRIBUTE_TYPE type;
};

/*! @fn inline ostream& operator<<(ostream& o, const PathStepAttribute& exp)
 *  @brief
 *  @details
 *  @overload
 *  @retval ostream&
 */
inline ostream& operator<<(ostream& o, const PathStepAttribute& pstepattr) {
	pstepattr.print(o);
    return o;
}

#endif // PATHSTEPATTRIBUTE_H
