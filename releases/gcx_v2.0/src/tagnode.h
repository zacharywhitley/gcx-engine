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
 * 	@brief Header file for tagnode.cpp.
 * 	@details Header file specifying constructors, destructor and functions for tagnode.cpp.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef TAGNODE_H
#define TAGNODE_H

#include "tagmap.h"
#include "buffernodelist.h"

/*! @struct TagNode
 * 	@brief Represents a tag buffer node.
 * 	@details The TagNode class provides the tag id, a list of children, and a flag that
 * 	         specifies whether the node is closed (i.e. if the closing tag has been read)
 * 	         or not.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 *  @license Software License Agreement (BSD License)
 */
struct TagNode {
	/*! @brief Constructor.
	 *  @details Constructor - creating a fresh TagNode object.
	 *  @param[in] _tag - The id of the tag.
	 */
    TagNode(TAG _tag);

	/*! @brief Destructor.
	 *  @details Destructor.
	 */
    virtual ~TagNode();

	/*! @var TAG tag
	 *  @brief The id of the tag caried by this object.
	 *  @details
	 */
    TAG tag;

	/*! @var bool is_closed
	 *  @brief True if the closing tag has already been read, false otherwise.
	 *  @details
	 */
    bool is_closed;

	/*! @var BufferNodeList children
	 *  @brief A list of the tag nodes's children.
	 *  @details
	 */
    BufferNodeList children;
};

#endif // TAGNODE_H
