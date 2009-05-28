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
 * 	@brief Implementations of header file pathexpressionadornment.h.
 * 	@details Implementations of constructors, destructor and functions of the corresponding header file pathexpressionadornment.h.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#include "pathexpressionadornment.h"

PathExpressionAdornment::PathExpressionAdornment(PathExpression* _path) :
adorned_path(NULL), rewritten_path(NULL), is_child_node_path(false),
is_descendant_node_path(false), is_dos_node_path(false),
is_child_text_path(false), is_descendant_text_path(false) {

    // The fact that we, in addition to the Suciu XPath containment paper,
	// also consider node() and text() expressions, as well as dos-axes at the
	// end makes the implementation of the algorithm a little tricky. The key
	// idea is to replace interior 	// node() steps by * and interior text() steps
	// lead to unsatisfyable paths. Consequently, we clean the path by rewriting
	// these constructs. The information if node() and text() appear at the
	// end is stored externally in some class member variables and will be
	// handled using a series of special cases. The implementation of Suciu's
	// path containment algorithm can be found in class PathExpression.

	// if the original path is empty or selects no node, set adorned_path=NULL
	if (!_path || _path->selectsNoNode() || _path->isEmptyPath()) {
		return;
	}

	// otherwise we take the original path as a starting point	
	rewritten_path=new PathExpression();
	rewritten_path->addPathStep(new PathStepTagExpression(at_child,TAG_SHADOW_FRONT));
	for (unsigned i=0;i<_path->getPathSize();i++) {
		rewritten_path->addPathStep(_path->getPathStepAt(i)->clone());
	}

	// if the path is of the form [...]/axis::text()/dos::node() we drop
	// the final (redundant) dos::node() path step 
	unsigned path_size=rewritten_path->getPathSize();
	if (path_size>1 &&
		rewritten_path->getPathStepAt(path_size-1)->isDosNodeStep() && 
		rewritten_path->getPathStepAt(path_size-2)->isTextNodeTest()) {
		delete rewritten_path->getPathStepAt(path_size-1);
		rewritten_path->getPathSteps()->pop_back();
	}
	
	// if the path is of the form [...]/axis::node()/dos::node() we 
	// we replace it by [...]//node()
	path_size=rewritten_path->getPathSize();
	if (path_size>1 &&
		rewritten_path->getPathStepAt(path_size-1)->isDosNodeStep() && 
		rewritten_path->getPathStepAt(path_size-2)->isNodeNodeTest()) {

		rewritten_path->getPathStepAt(path_size-2)->setAxisType(at_descendant);
		delete rewritten_path->getPathStepAt(path_size-1);
		rewritten_path->getPathSteps()->pop_back();
	}
		
	// if the tail path step is of the form axis::node(), remove this
	// tail step and store the information
	PathStepExpression* tail=rewritten_path->getTailPathStep();
	if (tail->isNodeNodeTest()) {

		switch (tail->getAxisType()) {
			case at_child:
				is_child_node_path=true;
				break;
			case at_descendant:
				is_descendant_node_path=true;
				break;
			case at_dos:
				is_dos_node_path=true;
				break;
		}
		rewritten_path->getPathSteps()->pop_back();
		delete tail;

	// if the tail path step is axis::text(), remove it and store information
	// (note that patterns of the form [...]/axis::text()/axis::node() have
	//  been removed previously, either by containsInnerTextNode() or the
	//  axis::text()/dos::node() condition)
	} else if (tail->isTextNodeTest()) {

		switch (tail->getAxisType()) {
			case at_child:
				is_child_text_path=true;
				break;
			case at_descendant:
				is_descendant_text_path=true;
				break;
			default:
				// should never happen
				break;
		}
		rewritten_path->getPathSteps()->pop_back();
		delete tail;
	}

    // now we have a regular token (i.e. a tag or a wildcard at the end)

	// if the rewritten path has become empty now, we set it to NULL and return
	if (rewritten_path->isEmptyPath()) {
		delete rewritten_path;
		rewritten_path=NULL;
		return;
	}

	// a little trick: we append //* at the end and allow this guy to match
	// text nodes too; this way, the path can be used for standard containment
	// checking and matches text() at the same time
	if (is_descendant_node_path) {
		is_descendant_text_path=true;
		rewritten_path->addPathStep(new PathStepStarExpression(at_descendant));
	}

	// now we are in the position to add the descendant shadow leaf ...
	rewritten_path->addPathStep(new PathStepTagExpression(at_descendant,
															TAG_SHADOW_TAIL));
		
	// ... and to replace interior node() path steps by star labels 
	vector<PathStepExpression*>* steps = rewritten_path->getPathSteps();
	for (unsigned i=0; i<steps->size()-1; i++) {
		if ((*steps)[i]->isNodeNodeTest()) {
			rewritten_path->replacePathStepAt(i,new PathStepStarExpression(
												(*steps)[i]->getAxisType()));
		}
	}
		
	// compute adornment in a one-pass traversal over the path)
	adorned_path=new PathExpression();
	unsigned adornment_ctr=0; // #eliminated predecessor-pathsteps

	for (unsigned i=0;i<steps->size();i++) {

		PathStepExpression* step=(*steps)[i]; // cur path step

		// an axis::* path step can be eliminated whenever there is a
		// follow-up path step AND it is not of the form /*/ (note that
		// this second condition implicitly holds if adornment_ctr>0,
		// since in this case the current axis is rewritten to //);
		// moreover, it can be eliminated if the path is something like
		// /*/axis_1::*/axis_2::*/.../axis_n::tag, where at least one
		// axis_n is a descendant axis (note that there are no more
		// dos-axes in the rewritten_path)
		if (step->isStarNodeTest() && i<steps->size()-1 &&
				(adornment_ctr>0 || step->getAxisType()==at_descendant || 
				(*steps)[i+1]->getAxisType()==at_descendant ||
				rewritten_path->containsStarDescendantSequence(i+1))) {
			adornment_ctr++;
		} else {
			PathStepExpression* p=(*steps)[i]->clone();
			if (adornment_ctr>0) {
				p->setAxisType(at_descendant); // force descendant
			}
			adorned_path->addPathStep(p);
			path_adornments.push_back(adornment_ctr);

			// reset variables
			adornment_ctr=0;
		}
	}
}

PathExpressionAdornment::~PathExpressionAdornment() {
	delete rewritten_path;
	delete adorned_path;
}

void PathExpressionAdornment::print(ostream& o) const {
	if (!adorned_path) {
		cout << "empty";
	} else {
		for (unsigned i=0; i<adorned_path->getPathSize(); i++) {
			o << *(adorned_path->getPathStepAt(i)) 
				<< "(>=" << path_adornments[i] << ")";
		}
	}
	cout << " [cn/dn/dosn/ct/dt=" << is_child_node_path << "/"
			<< is_descendant_node_path << "/" << is_dos_node_path << "/"
			<< is_child_text_path << "/" << is_descendant_text_path << "]";
}
