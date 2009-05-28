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
 * 	@brief Header file for buffernodelist.cpp.
 *  @details Header file specifying constructors, destructor and functions for buffernodelist.cpp.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef BUFFERNODELIST_H
#define BUFFERNODELIST_H

#include <iostream>
#include <vector>

// forward declarations
class BufferNode;

/*! @class BufferNodeList
 *  @brief Represents a list of buffer nodes.
 * 	@details An (ordered) list of BufferNode elements. The BufferNode objects are linked via
 * 	         the BufferNode::r_sibling member variable.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
class BufferNodeList {

public:
	/*! @brief Constructor
	 * 	@details Constructor - creates an empty BufferNodeList object.
	 */
    BufferNodeList();
    
	/*! @brief Destructor
	 * 	@details Destructor
	 */
    virtual ~BufferNodeList();

    /*! @brief Appends another BufferNode to the list.
     *  @details
     *  @param[in] node The pointer to the node to be appended.
     *  @retval void
	 */
    void push_back(BufferNode* node);
    
    /*! @brief Removess a node from the BufferNodeList.
     *  @details 
     *  @param[in] to_remove The BufferNode object to be removed.
     *  @param[in] prev The predecessor of the object to be removed. May be in case
	 *                  the first removed node is the first one in the list. 
     *  @retval void
	 */
    void removeElem(BufferNode* to_remove, BufferNode* prev);
    
    /*! @brief Retrieves the BufferNode at position index.
     *  @details The first node in the list has index zero. Does not check for out-of-bound violation.
     *  @param[in] index The position of the BufferNode in the list.
     *  @retval BufferNode* Pointer to the retrieved BufferNode.
	 */
    BufferNode* operator[](unsigned index);
    
    /*! @brief Checks for emptyness.
     *  @details
     *  @retval bool Returns true if the BufferNodeList object is empty, false otherwise.
	 */
    inline bool isEmpty() { return front==NULL; }
    
    /*! @brief Retrieves the first element of the BufferNodeList.
     *  @details May return NULL if the list is empty.
     *  @retval BufferNode* Pointer to the first BufferNode in the list.
	 */
    inline BufferNode* getFront() { return front; }

    /*! @brief Prints the complete BufferNodeList.
     *  @details This is a debug print method: the output comprises role information etc.
     *  @retval void
	 */
    void print();

private:
	/*! @var BufferNode* front
	 *  @brief The front element of the list.
	 *  @details May be NULL (if the list is empty).
	 */
    BufferNode* front;

	/*! @var BufferNode* back 
	 *  @brief The last element of the list.
	 *  @details May be NULL (if the list is empty).
	 */
    BufferNode* back;
};

#endif // BUFFERNODELIST_H
