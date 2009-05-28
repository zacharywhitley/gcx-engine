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
 * 	@brief Implementations of header file pathstepexpression.h.
 * 	@details Implementations of constructors, destructor and functions of the corresponding header file pathstepexpression.h.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#include "pathstepexpression.h"
#include "pathsteptagexpression.h"
#include "pathstepstarexpression.h"
#include "pathstepnodeexpression.h"
#include "pathsteptextexpression.h"

PathStepExpression::PathStepExpression(EXP_TYPE _type, AXIS_TYPE _axis,	PathStepAttribute* _attribute) :
Expression(_type), axis(_axis), attribute(_attribute) {}

PathStepExpression::~PathStepExpression() {
	delete attribute;
}

NODETEST_TYPE PathStepExpression::getNodeTestType() {
	switch (type) {
		case et_pathsteptag:
			return ntt_tag;
		case et_pathstepstar:
			return ntt_star;
		case et_pathstepnode:
			return ntt_node;
		case et_pathsteptext:
			return ntt_text;
		default: // should never happen
			return ntt_tag;
	}
}

bool PathStepExpression::isSyntacticallyEqualTo(PathStepExpression* ps) {
	if (ps) {
		if (axis!=ps->getAxisType()) {
			return false;
		} else if (getNodeTestType()!=ps->getNodeTestType()) {
			return false;
		} else if (ps->isTagNodeTest()) {
			return isMatchingTag(((PathStepTagExpression*)ps)->getNodeTest());
		}
		return true;
	}
	return false;
}

unsigned PathStepExpression::getStepWeight(bool is_last_step) {
	unsigned weight = 0;
	
	switch (axis) {
		case at_child:
			weight = WEIGHT_AXIS_CHILD;
			break;
		case at_descendant:
			weight = WEIGHT_AXIS_DESCENDANT;
			break;
		case at_dos:
			weight = WEIGHT_AXIS_DOS;
			break;
	}
	if (is_last_step) {
		switch (type) {
			case et_pathsteptag:
				weight = weight * WEIGHT_NODETEST_TAG;
				break;
			case et_pathstepstar:
				weight = weight * WEIGHT_NODETEST_STAR;
				break;
			case et_pathstepnode:
				weight = weight * WEIGHT_NODETEST_NODE;
				break;
			case et_pathsteptext:
				weight = weight * WEIGHT_NODETEST_TEXT;
				break;
			default: // should never happen
				break;
		}
	} else {
		weight = weight * WEIGHT_INNER_NODETEST;
	}
	
	return weight;
}
