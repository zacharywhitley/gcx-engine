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
 * 	@brief Header file for projectiontreelabels.cpp.
 * 	@details Header file specifying constructors, destructor and functions for projectiontreelabels.cpp.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef PROJECTIONTREELABELS_H
#define PROJECTIONTREELABELS_H

#include <vector>
#include "projectiontreelabel.h"

/*! @class ProjectionTreeLabels
 * 	@brief Calculates and stores the set of all projection tree labels.
 * 	@details
 * 	@author Michael Schmidt
 * 	@author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
class ProjectionTreeLabels {

public:
	/*! @brief Constructor.
     *  @details Constructor - labels the projection tree and
     *           stores the labels internally.
     */
	ProjectionTreeLabels(ProjectionTreeNode* root);

	/*! @brief Destructor.
     *  @details Destructor.
     */
	virtual ~ProjectionTreeLabels();

	/*! @brief Brings the parent and predecessor links of the labels up to date.
     *  @details
     *  @retval void
     */
	void updateParentPredecessorInformation();

	/*! @brief Returns the label by id.
     *  @details 
     *  @param[in] id The id of the label that is requested.
     *  @retval ProjectionTreeLabel* 
     */
	inline ProjectionTreeLabel* getLabelById(unsigned id) { return labels[id]; } 	

	/*! @brief Returns the number of all projection tree labels.
     *  @details
     *  @retval unsigned The number of projection tree labels.
     */
	inline unsigned nrOfLabels() { return labels.size(); }

	/*! @brief Debug prints all projection tree labels to STDOUT.
     *  @details
     *  @retval void
     */
	void print();

	/*! @brief Returns the ids of all dos::node() successors recursively
     *  @details
     *  @param[in] The id of the token we search the dos::node() successors for.
     *  @param[in] vector<unsigned> Collects the ids of all dos::node() successors.
     */
	void getAllRecursiveDosNodeSuccessors(unsigned id,vector<unsigned>&);

private:
	/*! @brief Help function for projection tree label computation.
     *  @details Recursively calls itself.
     *  @param[in] n The parent node.
     *  @param[in] id The id of the parent node.
     *  @param[in] cur_position The current path step position=
     *                          in the current path.
     *  @retval ProjectionTreeLabel* The label that has been set up.
     */
	ProjectionTreeLabel* computeProjectionTreeLabels(ProjectionTreeNode* n,
													unsigned& id,
													unsigned cur_position);
	
	/*! @var vector<ProjectionTreeLabel*> labels
     *  @brief Stores the list of all projection tree labels.
     *  @details
     */
	vector<ProjectionTreeLabel*> labels;
};

#endif // PROJECTIONTREELABLES_H
