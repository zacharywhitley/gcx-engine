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
 * 	@brief Header file for pathexpressionadornment.cpp.
 * 	@details Header file specifying constructors, destructor and functions for pathexpressionadornment.cpp.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef PATHEXPRESSSIONADORNMENT_H
#define PATHEXPRESSSIONADORNMENT_H

#include "pathexpression.h"
#include "pathstepstarexpression.h"

/*! @def TAG_SHADOW_FRONT
 *  @details Label name for the shadow front tag used in the path containment check algorithm.
 *  @details For details, see the paper from Suciu on XPath Containment.
 */
#define TAG_SHADOW_FRONT ".shadow_front"

/*! @def TAG_SHADOW_TAIL
 *  @details Label name for the shadow tail tag used in the path containment check algorithm.
 *  @details For details, see the paper from Suciu on XPath Containment.
 */
#define TAG_SHADOW_TAIL ".shadow_tail"

/*! @class PathExpressionAdornment
 * 	@brief
 * 	@details
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
class PathExpressionAdornment {
	
public:
    PathExpressionAdornment(PathExpression* _path);
    virtual ~PathExpressionAdornment();

    inline PathExpression* getAdornedPath() { return adorned_path; }
    inline PathExpression* getRewrittenPath() { return rewritten_path; }
    inline vector<unsigned>* getPathAdornments() { return &path_adornments; }
    
    void print(ostream& o) const;
    
	inline bool isChildNodePath() { return is_child_node_path; }
	inline bool isDescendantNodePath() { return is_descendant_node_path; }
	inline bool isDosNodePath() { return is_dos_node_path; }
	inline bool isChildTextPath() { return is_child_text_path; }
	inline bool isDescendantTextPath() { return is_descendant_text_path; }

private:
	PathExpression* adorned_path;
	PathExpression* rewritten_path;
    vector<unsigned> path_adornments;

	// true if the original path ends with dos::node()
	// (this information is lost in the adorned path)
	bool is_child_node_path;
	bool is_descendant_node_path;
	bool is_dos_node_path;
	bool is_child_text_path;
	bool is_descendant_text_path;
};

/*!
 * operator << overloading
 */
inline ostream& operator<<(ostream& o, const PathExpressionAdornment& padorn) {
	padorn.print(o);
    return o;
}

#endif // PATHEXPRESSSIONADORNMENT_H
