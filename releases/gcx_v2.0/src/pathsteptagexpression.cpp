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
 * 	@brief Implementations of header file pathsteptagexpression.h.
 * 	@details Implementations of constructors, destructor and functions of the corresponding header file pathsteptagexpression.h.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#include "pathsteptagexpression.h"

PathStepTagExpression::PathStepTagExpression(AXIS_TYPE _axis, const char* _tag) :
PathStepExpression(et_pathsteptag, _axis, NULL), nt(tagmap->insertTag(_tag)) {}

PathStepTagExpression::PathStepTagExpression(const char* _tag) :
PathStepExpression(et_pathsteptag, at_child, NULL), nt(tagmap->insertTag(_tag)) {}

PathStepTagExpression::PathStepTagExpression(AXIS_TYPE _axis, TAG _tag, PathStepAttribute* _attribute) :
PathStepExpression(et_pathsteptag, _axis, _attribute), nt(_tag) {}

PathStepTagExpression::~PathStepTagExpression() {}

void PathStepTagExpression::print(ostream& o) const {
	switch (axis) {
		case at_child:
	    	o << "/";
			break;
		case at_descendant:
			o << "//";
			break;
		case at_dos:
			o << "/dos::";
			break;
	}
    o << tagmap->getTag(nt);
    if (attribute) {
    	o << *(attribute);
	}
}

PathStepTagExpression* PathStepTagExpression::clone() {
	if (attribute) {
		return new PathStepTagExpression(axis, nt, attribute->clone());
	} else {
		return new PathStepTagExpression(axis, nt, NULL);
	}
}

PathStepTagExpression* PathStepTagExpression::cloneWithoutAttributes() {
	return new PathStepTagExpression(axis, nt, NULL);
}
