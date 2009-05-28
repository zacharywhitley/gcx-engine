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
 * 	@brief Implementations of header file pathexpression.h.
 * 	@details Implementations of constructors, destructor and functions of the corresponding header file pathexpression.h.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#include "pathexpression.h"

PathExpression::PathExpression() :
Expression(et_path), adornment(NULL) {}

PathExpression::~PathExpression() {
	for (unsigned i=0; i<pathsteps.size(); i++) {
		delete pathsteps[i];
	}
	delete adornment;
}

bool PathExpression::isDosNodePath() {
	if (pathsteps.size() > 1 || pathsteps.size()==0) {
		return false;
	}
	return ((pathsteps[0]->getAxisType()==at_dos) && (pathsteps[0]->isNodeNodeTest()));
}

bool PathExpression::hasInnerTextNodeTest() {
	for (unsigned i=0; i<pathsteps.size(); i++) {
		if ((pathsteps[i]->isTextNodeTest()) && (i<pathsteps.size()-1)) {
			return true;
		}
	}
	return false;
}

bool PathExpression::hasTerminatingTextNodeTest() {
	return pathsteps[pathsteps.size()-1]->isTextNodeTest();
}

bool PathExpression::hasFollowingDescendantOrDosFrom(unsigned ps_idx) {	
	for (unsigned i=(ps_idx+1); i<pathsteps.size(); i++) {
		if (pathsteps[i]->getAxisType()==at_descendant || pathsteps[i]->getAxisType()==at_dos) {
			return true;
		}
	}
	
	return false;
}

bool PathExpression::hasPreviousDescendantOrDosUpTo(unsigned ps_idx) {
	if (ps_idx==0 || ps_idx>=pathsteps.size()) {
		return false;
	}
	
	for (unsigned i=0; i<ps_idx; i++) {
		if (pathsteps[i]->getAxisType()==at_descendant || pathsteps[i]->getAxisType()==at_dos) {
			return true;
		}
	}
	
	return false;
}

bool PathExpression::isSyntacticallyEqualTo(PathExpression* path) {
	if (path) {
		if (pathsteps.size()==path->getPathSize()) {
			vector<PathStepExpression*>* steps = path->getPathSteps();
			for (unsigned i=0; i<pathsteps.size(); i++) {				
				if (!pathsteps[i]->isSyntacticallyEqualTo((*steps)[i])) {				
					return false;
				}				
			}
			return true;
		}
		return false;
	}
	return false;	
}

bool PathExpression::isSemanticallyContainedIn(PathExpression* path) {
	
	PathExpressionAdornment* path_adornment=path->getAdornment();
	PathExpressionAdornment* this_adornment=getAdornment();

    //cout << *this << " contained in " << *path << " ?" << endl;
	//cout << *(this_adornment->getRewrittenPath()) <<  " vs. " 
	//		<< *(path_adornment->getRewrittenPath()) << endl;
	//cout << *(this_adornment->getAdornedPath()) <<  " vs. " 
	//		<< *(path_adornment->getAdornedPath()) << endl;

	// please note that the following checks are not necessarily
	// complete, but never ever return true errorneously
	
	// first we catch the scenario where the adorned path is NULL,
	// hence is at most of the form /axis::node() or /axis::text()
	if (this_adornment->getAdornedPath()==NULL) {

		// case 1: /child::node() is always contained in
		//         /child::node(), /descendant::node(), and /dos::node()
		if (this_adornment->isChildNodePath()) {
			return path_adornment->getAdornedPath()==NULL &&
					(path_adornment->isChildNodePath() ||
					 path_adornment->isDescendantNodePath() ||
					 path_adornment->isDosNodePath());

		// case 2: /descendant::node() is contained in
		//         /descendant::node() and /dos::node()
		} else if (this_adornment->isDescendantNodePath()) {
			return path_adornment->getAdornedPath()==NULL &&
					(path_adornment->isDescendantNodePath() ||
					 path_adornment->isDosNodePath());

		// case 3: /dos::node() is contained in /dos::node()
		} else if (this_adornment->isDosNodePath()) {
			return path_adornment->getAdornedPath()==NULL &&
					path_adornment->isDosNodePath();

		// case 4: /child::text() is always contained in
		//         /child::node(), /descendant::node(), /dos::node(),
		//         /child::text(), and /descendant::text()
		} else if (this_adornment->isChildTextPath()) {
			return path_adornment->getAdornedPath()==NULL &&
					(path_adornment->isChildNodePath() ||
					 path_adornment->isDescendantNodePath() ||
					 path_adornment->isDosNodePath() ||
					 path_adornment->isChildTextPath() ||
					 path_adornment->isDescendantTextPath());

		// case 5: /descendant::text() is always contained in
		//         /descendant::node(), /dos::node(),
		//         and /descendant::text()
		} else if (this_adornment->isDescendantTextPath()) {
			return path_adornment->getAdornedPath()==NULL &&
					(path_adornment->isDescendantNodePath() ||
					 path_adornment->isDosNodePath() ||
					 path_adornment->isDescendantTextPath());
		} else {
			// otherwise the current path selects no node at all,
			// thus being trivially contained in the other path
			return true; 
		}

	// second we catch the scenario where the adorned path tested to
	// contain the current path is NULL, hence is at most of the form
	// /axis::node() or /axis::text()
	} else if (path_adornment->getAdornedPath()==NULL) {

		// case 1: /descendant::node() and /dos::node() always contains
		//         everything that is non-empty
		if (path_adornment->isDosNodePath() || 
			path_adornment->isDescendantNodePath()) {
			return true; 

		// case 2: /descendant::text() contains everything that
		//         selects some text
		} else if (path_adornment->isDescendantTextPath()) {
			return this_adornment->isChildTextPath() ||
					this_adornment->isDescendantTextPath();

		// case 3: although there might be containment in some rare cases,
		//         we return false here (this is always sound)
		} else {
			return false;
		}

	} else {
		// we now assert that the final path step in the original path matches
		// the final path step in the path we check containment for;
		// again, note that this is not necessarily complete, but always sound
		
		// case 1: dos::node() is matched only by dos::node()
		if (this_adornment->isDosNodePath()) {
			if (!path_adornment->isDosNodePath() &&
				!(path_adornment->isDescendantNodePath() &&
					path_adornment->isDescendantTextPath()))
				return false;


		// case 2: descendant::node() is matched by dos::node()
		//         and descendant::node()
		} else if (this_adornment->isDescendantNodePath()) {
			if (!(path_adornment->isDosNodePath() ||  
					path_adornment->isDescendantNodePath()))
				return false;

		// case 3: child::node() is matched by dos::node(), descendant::node(),
		//         and child::node()
		} else if (this_adornment->isChildNodePath()) { 
			if (!(path_adornment->isDosNodePath() ||
					path_adornment->isDescendantNodePath() ||
					path_adornment->isChildNodePath())) 
				return false;

		// case 4: descendant::text() is matched by dos::node(),
		//         descendant::node(), and descendant::text()
		} else if (this_adornment->isDescendantTextPath()) {
			if (!(path_adornment->isDosNodePath() ||
						path_adornment->isDescendantNodePath() ||
						path_adornment->isDescendantTextPath()))
				return false;

		// case 5: child::text() is matched by dos::node(),
		//         descendant::node(), child::node(), descendant::text(),
		//         and child::text()
		} else if (this_adornment->isChildTextPath()) { 
			if (!(path_adornment->isDosNodePath() ||
					path_adornment->isDescendantNodePath() ||
					path_adornment->isChildNodePath() ||
					path_adornment->isDescendantTextPath() ||
					path_adornment->isChildTextPath())) 
				return false;

		// case 6: if the path is a path without any of the endings above,
		//         we must assert that the other path is so, too, or a path
		//         of the for dos::node() (which causes no harm)
		} else {
			if (path_adornment->isDescendantNodePath() ||
					path_adornment->isChildNodePath() ||
					path_adornment->isDescendantTextPath() ||
					path_adornment->isChildTextPath())
				return false;
		}
	}


	// now we are in the position to run the homomorphism check algorithm; we
	// do not need to consider the special cases above anymore; if we reach
	// this position, it suffices to check for path containment
	PathExpression* p=this_adornment->getRewrittenPath();
	unsigned p_size=p->getPathSize();

	PathExpression* p_prime=path_adornment->getAdornedPath();
	vector<unsigned>* p_prime_ad=path_adornment->getPathAdornments();
	unsigned p_prime_size=(int)p_prime->getPathSize();

	bool C[p_size][p_prime_size];
	int D[p_size][p_prime_size];
	for (unsigned i=0;i<p_size;i++) {
		for (unsigned j=0;j<p_prime_size;j++) {
			C[i][j]=false;
			D[i][j]=0;
		}
	}

	// core algorithm (Algorithm 3 in paper)
	for (int x=p_size-1;x>=0;x--) {
		for (int y=p_prime_size-1;y>=0;y--) {

			bool x_at_end=(unsigned)x==p_size-1;
			bool y_at_end=(unsigned)y==p_prime_size-1;

			// part 1: compute C[x][y]
			bool cond=false;
			if (p_prime->getPathStepAt(y)->isStarNodeTest()) {
				cond=true;
			} else if (p_prime->getPathStepAt(y)->isTagNodeTest() &&
						p->getPathStepAt(x)->isTagNodeTest()) {
				PathStepTagExpression* ps_tag=
					(PathStepTagExpression*)p->getPathStepAt(x);
				PathStepTagExpression* ps_prime_tag=
					(PathStepTagExpression*)p_prime->getPathStepAt(y);
				cond=ps_tag->getNodeTest()==ps_prime_tag->getNodeTest();
			}
			
			if (cond && !y_at_end) {
				switch (p_prime->getPathStepAt(y+1)->getAxisType()) {
					case at_child:
						cond=!x_at_end && C[x+1][y+1] && 
							p->getPathStepAt(x+1)->getAxisType()==at_child;
						break;
					case at_descendant:
						cond=D[x][y+1]>=(int)(1+(*p_prime_ad)[y+1]);
						break;
					default:
						// should never happen
						break;
				}
			}

			C[x][y]=cond; 
			

			// part 2: compute d
			int d=C[x][y]?0:-1; // -1 := -infty

			// part 3: comput D[x][y]
			int max2=-1; // -1 := -infty
			if (!x_at_end && D[x+1][y]>=0) {
				max2=1+D[x+1][y];
			}
			
			int max=max2>d?max2:d;
			D[x][y]=max;
		}
	}

	return C[0][0];
}

unsigned PathExpression::getWeight() {
	unsigned weight = 0;
	
	if (pathsteps.size()>0) {
		for (unsigned i=0; i<(pathsteps.size()-1); i++) {
			weight += pathsteps[i]->getStepWeight(false);
		}
		
		weight += pathsteps[pathsteps.size()-1]->getStepWeight(true);
	}
	
	return weight;
}

PathExpression* PathExpression::clone() {
    PathExpression* p=new PathExpression();
    for (unsigned i=0; i<pathsteps.size(); i++) {
        p->addPathStep(pathsteps[i]->clone());
    }
    
    return p;
}

PathExpression* PathExpression::cloneWithoutFinalDosNodeAndAttributes() {
    PathExpression* p=new PathExpression();
    for (unsigned i=0; i<pathsteps.size(); i++) {
		if (!(i==pathsteps.size()-1 && pathsteps[i]->isNodeNodeTest() &&
				pathsteps[i]->getAxisType()==at_dos)) {
	        p->addPathStep(pathsteps[i]->cloneWithoutAttributes());
		}
    }
    
    return p;
}

void PathExpression::print(ostream& o) const {
    for (unsigned i=0; i<pathsteps.size(); i++) {
    	o << *(pathsteps[i]);
    }
}

PathStepExpression* PathExpression::getTailPathStep() {
	if (pathsteps.size()==0) {
		return NULL;
	}
	
	return pathsteps[pathsteps.size()-1];
}

PathStepExpression* PathExpression::getPathStepAfterTextNodeTest() {
	if (pathsteps.size()==0) {
		return NULL;
	} else {
		for (unsigned i=0; i<pathsteps.size(); i++) {
			if (pathsteps[i]->isTextNodeTest()) {
				return pathsteps[i+1];
			}
		}
	}
	
	return NULL;
}

void PathExpression::replacePathStepAt(unsigned idx, PathStepExpression* ps) {
	if (ps && idx <= pathsteps.size()) {
		delete pathsteps[idx];
		pathsteps[idx]=ps;
	}
}

bool PathExpression::selectsNoNode() {
	for (unsigned i=0;i<pathsteps.size()-1;i++) {
		if (i!=pathsteps.size()-2 || !pathsteps[i+1]->isDosNodeStep()) {
			if (pathsteps[i]->isTextNodeTest()) return true;
		}
	}

	return false;
}

bool PathExpression::containsStarDescendantSequence(unsigned pos) {
	// break if end of path reached
	if (pos>=pathsteps.size()) {
		return false;
	} else if (pathsteps[pos]->getAxisType()==at_descendant) {
		return true;
	} else {
		return pathsteps[pos]->isStarNodeTest()?
			containsStarDescendantSequence(pos+1):false;
	}
}

PathExpressionAdornment* PathExpression::getAdornment() {
	if (adornment==NULL) {
		adornment=new PathExpressionAdornment(this);
	}
	
	return adornment;
}

bool PathExpression::mightHasChildDescendantConflict(PathExpression* p) {
	PathStepExpression* this_last_nondos_ps=NULL;
	PathStepExpression* path_last_nondos_ps=NULL;

	unsigned this_size=getPathSize();
	unsigned path_size=p->getPathSize();

	if (this_size==0 || (this_size==1 && getPathStepAt(0)->isDosNodeStep())) {
		return false;	// no conflict
	} else {
		this_last_nondos_ps=getPathStepAt(this_size-1)->isDosNodeStep()?
							getPathStepAt(this_size-2):
							getPathStepAt(this_size-1);
	}


	if (path_size==0 || (path_size==1 && p->getPathStepAt(0)->isDosNodeStep())) {
		return false;	// no conflict
	} else {
		path_last_nondos_ps=p->getPathStepAt(path_size-1)->isDosNodeStep()?
							p->getPathStepAt(path_size-2):
							p->getPathStepAt(path_size-1);
	}

	// note: both this_last_nondos_ps and path_last_nondis_ps differ from NULL
	AXIS_TYPE this_type=this_last_nondos_ps->getAxisType();
	AXIS_TYPE path_type=path_last_nondos_ps->getAxisType();

	if ((this_type==at_child && path_type==at_descendant) ||
		(this_type==at_descendant && path_type==at_child)) {
		if (this_last_nondos_ps->isTagNodeTest() &&
			path_last_nondos_ps->isTagNodeTest()) {
			// compare tags
			PathStepTagExpression* t1=
				(PathStepTagExpression*)this_last_nondos_ps;
			PathStepTagExpression* t2=
				(PathStepTagExpression*)path_last_nondos_ps;
			return t1->getNodeTest()==t2->getNodeTest(); // confl. if identical
		} else if (this_last_nondos_ps->isTextNodeTest() ||
					path_last_nondos_ps->isTextNodeTest()) {
			return false; // no conflict
		} else {
			// yes, we have a conflict for node()=a, a=node(), *=a, a=*, *=*,
			// *=node(), node()=node(), ...
			return true;
		}
	} else {
		return false;
	}
}
