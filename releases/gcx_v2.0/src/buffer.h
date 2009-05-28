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
 * 	@brief Header file for buffer.cpp.
 * 	@details Header file specifying constructors, destructor and functions for buffer.cpp.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef BUFFER_H
#define BUFFER_H

#include <iostream>
#include "buffernode.h"
#include "projectiondfa.h"

/*! @class Buffer
 * 	@brief Represents the DOM buffer.
 * 	@details The buffer is implemented as a DOM tree. The buffer may contain
 * 	         both tag and character nodes, and the class provides methods
 * 	         for appending suchlike node types.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
class Buffer {

public:
	/*! @brief Constructor.
     *  @details Constructs a buffer that contains solely the document root.
     *  @param[in] _pdfa The ProjectionDFA is used to assign the roles to the document root.
     */
    Buffer(ProjectionDFA* _pdfa);

	/*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~Buffer();

	/*! @fn inline BufferNode* getRoot()
     *  @brief Returns the document root.
     *  @details The document root is the virtual root node of the XML document.
     *  @retval BufferNode* Pointer to the document root.
     */
    inline BufferNode* getRoot() { return root; }

	/*! @fn inline BufferNode* getCurrent()
     *  @brief Returns the current buffer node (i.e. the one that has last
     *         recently been read into the buffer).
     *  @retval BufferNode* Pointer to the current buffer node.
     */
    inline BufferNode* getCurrent() { return cur; }

	/*! @brief Appends a tag node to the buffer.
     *  @details The tag node is appended at the current position of the buffer.
     *  @param[in] tag The tag id of the node that will be created.
     *  @retval void
     */
    void appendTag(TAG tag);

	/*! @brief Appends a tag node including roles to the buffer.
     *  @details The tag node is appended at the current position of the buffer.
     *  @param[in] tag The tag id of the node that will be created.
     *  @param[in] _cumulative_roles Pointer to a vector containing cumulative roles.
     *  @param[in] _non_cumulative_roles Pointer to a vector containing non-cumulative roles.
     *  @retval void
     */
    void appendTag(TAG tag, vector<unsigned>* _cumulative_roles,
					vector<unsigned>* _non_cumulative_roles);

	/*! @brief Appends a text node to the buffer.
     *  @details The text node is appended at the current position of the buffer.
     *  @param[in] data The PCDATA content of the node that will be created.
     *  @retval void
     */
    void appendPCData(const char* data);

	/*! @brief Appends a text node including some roles to the buffer.
     *  @details The text node is appended at the current position of the buffer.
     *  @param[in] data The PCDATA content of the node that will be created.
     *  @param[in] _cumulative_roles Pointer to a vector containing cumulative roles.
     *  @param[in] _non_cumulative_roles Pointer to a vector containing non-cumulative roles.
     *  @retval void
     */
    void appendPCData(const char* data, vector<unsigned>* _cumulative_roles,
						vector<unsigned>* _non_cumulative_roles);

	/*! @brief Closes the current tag node in the buffer.
     *  @details This is simply realized by marking the tag node as closed.
     *  @retval void
     */
    void closeTag();

	/*! @brief Appends a set of roles to the current buffer node.
     *  @param[in] _cumulative_roles Pointer to a vector containing cumulative roles.
     *  @param[in] _non_cumulative_roles Pointer to a vector containing non-cumulative roles.
     *  @retval void
     */
    void appendRoles(vector<unsigned>* _cumulative_roles,
                       vector<unsigned>* _non_cumulative_roles);

	/*! @brief Prints the buffer.
     *  @retval void
     */
    void print();

	/*! @brief Prints the current node of the buffer (including its subtree).
     *  @retval void
     */
    void printCurrent();

	/*! @brief Prints the buffer including debug and role information.
     *  @retval void
     */
    void debugPrint();

private:
	/*! @var BufferNode* root
     *  @brief The root node of the buffer, i.e. the virtual document root.
     */
    BufferNode* root;
	
	/*! @var BufferNode* cur
     *  @brief The current node of the buffer, i.e. the node that has been appended
     *         last recently.
     */
    BufferNode* cur;
};

#endif // BUFFER_H
