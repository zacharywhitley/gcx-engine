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
 *  @brief Header file for buffernode.cpp.
 *  @details Header file specifying constructors, destructor and functions for buffernode.cpp.
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
#ifndef BUFFERNODE_H
#define BUFFERNODE_H

/*! @def TYPE_TAG
 *  @brief Define for buffer nodes of type tag.
 */
#define TYPE_TAG        true

/*! @def TYPE_PCDATA
 *  @brief Define for buffer nodes of type character.
 */
#define TYPE_PCDATA        false

#include <vector>
#include <iostream>
#include <sstream>
#include "tagmap.h"
#include "tagnode.h"
#include "charnode.h"
#include "intmultiset.h"
#include "role.h"

// forward declarations
class PathExpression;
struct BufferNode;

/*! @struct BufferNode
 *     @brief Represents a buffer node.
 *     @details A buffer node represents a buffer node in the DOM tree, thus containing
 *              additional information like parent and right-sibling pointer. Additionally
 *              it contains fields for storing roles that are assigned to the buffer nodes,
 *              as well as methods for removing such roles. Each buffer node is either
 *              a TagNode or a CharNode. The specific information is stored in the
 *              void* node-member, which must be casted on demand. We chose this compact representation
 *              to keep the buffer representation minimal (recall that the goal of our project
 *              was to minimize main memory consumption, hence a small DOM buffer representation
 *              is crucial).
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
struct BufferNode {
    /*! @brief Constructor for character nodes.
     *     @details Constructor - creates a fresh character buffer child node for the given parent.
     *     @param[in] _data The PCDATA content carried by the character node.
     *     @param[in] _parent The parent of the newly created node.
     */
    BufferNode(const char* _data, BufferNode* _parent);
    
    /*! @brief Constructor for tag nodes.
     *     @details Constructor - creates a fresh tag buffer child node for the given parent.
     *     @param[in] _tag The tag id carried by the tag node.
     *     @param[in] _parent The parent of the newly created node.
     */
    BufferNode(TAG _tag, BufferNode* _parent);
    
    /*! @brief Constructor for character nodes.
     *     @details Constructor - creates a fresh character buffer child node for the given parent
     *              and appends the specified roles to the node.
     *     @param[in] _data The PCDATA content carried by the character node.
     *     @param[in] _parent The parent of the newly created node.
     *     @param[in] _cumulative_roles Pointer to a set of cumulative roles.
     *     @param[in] _non_cumulative_roles Pointer to a set of non-cumulative roles.
     */
    BufferNode(const char* _data, BufferNode* _parent,
                vector<unsigned>* _cumulative_roles,
                vector<unsigned>* _non_cumulative_roles);
    
    /*! @brief Constructor for tag nodes.
     *     @details Constructor - creates a fresh tag buffer child node for the given parent
     *              and appends the specified roles to the node.
     *     @param[in] _tag The tag id carried by the tag node.
     *     @param[in] _parent The parent of the newly created node.
     *     @param[in] _cumulative_roles Pointer to a set of cumulative roles.
     *     @param[in] _non_cumulative_roles Pointer to a set of non-cumulative roles.
     */
    BufferNode(TAG _tag, BufferNode* _parent,
                vector<unsigned>* _cumulative_roles,
                vector<unsigned>* _non_cumulative_roles);
    
    /*! @brief Destructor.
     *     @details Destructor.
     */
    virtual ~BufferNode();
    
    /*! @brief Returns true if the buffer node is the document root, false otherwise.
     *  @details
     *  @return bool
     */
    inline bool isRoot() { return type==TYPE_TAG && ((TagNode*)node)->tag==TAGID_ROOT; }

    /*! @brief Appends the specified roles to the buffer node.
     *  @details
     *     @param[in] _cumulative_roles Pointer to a set of cumulative roles.
     *     @param[in] _non_cumulative_roles Pointer to a set of non-cumulative roles.
     */
    void appendRoles(vector<unsigned>* _cumulative_roles,
                        vector<unsigned>* _non_cumulative_roles);

    /*! @brief Adds a child node for this BufferNode object.
     *  @details The child is added at the rightmost position, thus following document order.
     *  @param[in] child The child node to be added.
     *  @return void
     */
    void addChild(BufferNode* child);

    /*! @brief Marks the associated buffer TagNode object as closed.
     *  @details ATTENTION: This method applies only to tag buffer nodes, i.e.
     *           requires that the member variable type is set to TYPE_TAG. The caller
     *           must assert that this condition holds, otherwise the method is likely
     *           to crash (after all, closing a character node makes no sense).
     *  @return void
     */
    inline void close() { ((TagNode*)node)->is_closed=true; }

    /*! @brief Returns true if the associated buffer TagNode is marked as closed, false otherwise. 
     *  @details ATTENTION: This method applies only to tag buffer nodes, i.e.
     *           requires that the member variable type is set to TYPE_TAG. The caller
     *           must assert that this condition holds, otherwise the method is likely
     *           to crash (after all, checking for a CharNode to be closed makes no sense).
     *  @return void
     */
    inline bool isClosed() { return ((TagNode*)node)->is_closed; }

    /*! @brief Returns the PCDATA representation of the buffer node.
     *  @details This is done by appending the PCDATA content of the child nodes.
     *           ATTENTION: The caller is responsible to free() the return value.
     *  @retval const char* The PCDATA representation of the node.
     */
    const char* getPCDataRepresentation();

    /*! @brief Collects the character child nodes in the passed input stream.
     *  @details This is a help method of getPCDataRepresentation()
     *  @retval void
     */
    void getPCDataRepresentation(std::ostringstream& s);

    /*! @brief Locks the BufferNode object.
     *  @details The member variable lock is set to true.
     *  @retval void
     */
    inline void lock() { is_locked=true; }
    
    /*! @brief Returns true if the buffer node is locked, false otherwise. 
     *  @details
     *  @retval bool 
     */
    inline bool isLocked() { return is_locked; }

    /*! @brief Unlocks the BufferNode object and triggers garbage collection (if possible).
     *  @details The member variable lock is set to false. 
     *  @retval void
     */
    inline void unlock() { is_locked=false; }

    /*! @brief Removes the specified role from the BufferNode object.
     *  @details Afterwards, garbage collection is automativally triggered.
     *  @retval void
     */
    void removeRoleFromNode(Role* role);

    /*! @brief Triggers garbage collection, starting at this BufferNode object.
     *  @details Garbage collection is propagated downwards and upwards, if necessary.
     *  @retval void
     */
    void clear();

    /*! @brief This garbage collection method tries to garbage collect the whole subtree of the node.
     *  @details 
     *  @retval void
     */
    void clearSubtreeIfPossible();

    /*! @brief Returns true if the BufferNode object carries cumulative or non-cumulative roles.
     *  @details ATTENTION: The method does not take into consideration cumulative roles of ancestors.
     *  @retval bool
     */
    bool isMarked();

    /*! @brief Returns true if the BufferNode object carries a cumulativ role.
     *  @details ATTENTION: The method does not take into consideration cumulative roles of ancestors.
     *  @retval bool
     */
    bool isCumulativeMarked();
    
    /*! @brief Returns true if the BufferNode object has an ancestor with a cumulative role.
     *  @details In particular, ancestors with non-cumulative roles are ignored.
     *  @retval bool
     */
    bool hasCumulativeMarkedAncestor();
    
    /*! @brief Returns true if the BufferNode object has any marked descendant-or-self nodes.
     *  @details The type of the marking (cumulative vs. non-cumulative) roles does not play any role.
     *  @retval bool
     */
    bool hasNoMarkedAndNoLockedDos(); 

    /*! @brief Prints the subtree defined by the BufferNode object to STDOUT.
     *  @details The output contains no role information. This method can be used
     *           for output during regular query evaluation.
     *  @retval void
     */
    void print();
    
    /*! @brief Debug prints the BufferNode without its child nodes to STDOUT.
     *  @details The output also contains role information.
     *  @retval void
     */
    void printNoSubnodes();

    /*! @brief Debug prints the subtree defined by the BufferNode object to STDOUT.
     *  @details The output also contains role information.
     *  @retval void
     */
    void debugPrint();
    
    /*! @brief Debug prints the complete buffer (i.e. starting at the root).
     *  @details This is done by climbing up to the root and then calling a debugPrint().
     *  @retval void
     */
    void debugPrintCompleteBuffer();

    /*! @var bool type
     *  @brief The type of the buffer node.
     *  @details The type is either TYPE_TAG or TYPE_PCDATA and serves as a switch for
     *           casting the void* node member, either to a TagNode* or CharNode* object.
     */
    bool type;

    /*! @var BufferNode* parent
     *  @brief The parent of the buffer node object.
     *  @details Set to NULL for the document root.
     */
    BufferNode* parent;

    /*! @var BufferNode* r_sibling
     *  @brief The right sibling of the buffer node object.
     *  @details Set to NULL if no right sibling exists.
     */
    BufferNode* r_sibling;

    /*! @var vector<unsigned> cumulative_roles
     *  @brief A vector containing the cumulative roles associated with the buffer node.
     *  @details
     */
    #ifdef ROLE_REFCOUNT
    unsigned cumulative_roles;
    #else
    IntMultiSet* cumulative_roles;
    #endif // #ifdef ROLE_REFCOUNT

    /*! @var vector<unsigned> non_cumulative_roles
     *  @brief A vector containing the non-cumulative roles associated with the buffer node.
     *  @details
     */
    #ifdef ROLE_REFCOUNT
    unsigned non_cumulative_roles;
    #else
    IntMultiSet* non_cumulative_roles;
    #endif // #ifdef ROLE_REFCOUNT

    /*! @var void* node
     *  @brief The content carried by the BufferNode object.
     *  @details This variable is either of type TagNode* or of type CharNode*, i.e.
     *           represents either the character or tag node data. The member variable
     *           type serves as a switch and can be queries to cast the node accordingly.
     */
    void* node;
    
    /*! @var bool is_locked
     *  @brief A flag that can be used to lock the BufferNode.
     *  @details Locking prevents the BufferNode from being deleted. It is necessary to ensure
     *           that nodes referenced by buffer iterators will not be deleted too early, which
     *           might cause undefined pointers. 
     */
    bool is_locked;
};

#endif // BUFFERNODE_H
