
/*
 | Author: Michael Schmidt;
 |         Gunnar Jehl 
 |
 | ************************* SOFTWARE LICENSE AGREEMENT ***********************
 | This source code is published under the BSD License.
 |
 | See file 'LICENSE.txt' that comes with this distribution or
 | http://dbis.informatik.uni-freiburg.de/index.php?project=GCX/license.php
 | for the full license agreement.
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
 | ****************************************************************************
*/

/*! @file
 *  @brief Header file for buffernode.cpp.
 *  @details Header file specifying constructors, destructor and functions for buffernode.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef BUFFERNODE_H
#define BUFFERNODE_H

/*! @def TYPE_TAG
 *  @brief Define for buffer nodes of type tag.
 *  @details Used to set the type member variable of the BufferNode class.
 */
#define TYPE_TAG		true

/*! @def TYPE_PCDATA
 *  @brief Define for buffer nodes of type character.
 *  @details Used to set the type member variable of the BufferNode class.
 */
#define TYPE_PCDATA		false

#include <vector>
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
 *  @brief Represents a buffer node.
 *  @details A buffer node represents a buffer node in the DOM tree, thus containing
 *    additional information like parent and right-sibling pointer. Additionally
 *    it contains fields for storing roles that are assigned to the buffer nodes,
 *    as well as methods for removing such roles. Each buffer node is either
 *    a TagNode or a CharNode. The specific information is stored in the
 *    void* node-member, which must be casted on demand. We chose this compact representation
 *    to keep the buffer representation minimal (recall that the goal of our project
 *    was to minimize main memory consumption, hence a small DOM buffer representation
 *    is crucial).
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
struct BufferNode {
    /*! @brief Constructor.
     *  @details Constructor - creating object for a fresh character buffer child node for the given parent.
     *  @param[in] _data The PCDATA content carried by the character node.
     *  @param[in] _parent The parent of the newly created node.
     */
    BufferNode(const char *_data, BufferNode * _parent);

    /*! @brief Constructor.
     *  @details Constructor - creating object for a fresh tag buffer child node for the given parent.
     *  @param[in] _tag The tag id carried by the tag node.
     *  @param[in] _parent The parent of the newly created node.
     */
     BufferNode(TAG _tag, BufferNode * _parent);

    /*! @brief Constructor.
     *  @details Constructor - creating object for a fresh character buffer child node for the given parent
     *    and appends the specified roles to the node.
     *  @param[in] _data The PCDATA content carried by the character node.
     *  @param[in] _parent The parent of the newly created node.
     *  @param[in] _cumulative_roles Pointer to a set of cumulative roles.
     *  @param[in] _non_cumulative_roles Pointer to a set of non-cumulative roles.
     */
     BufferNode(const char *_data, BufferNode * _parent,
                vector < unsigned >*_cumulative_roles,
                vector < unsigned >*_non_cumulative_roles);

    /*! @brief Constructor.
     *  @details Constructor - creating object for a fresh tag buffer child node for the given parent
     *    and appends the specified roles to the node.
     *  @param[in] _tag The tag id carried by the tag node.
     *  @param[in] _parent The parent of the newly created node.
     *  @param[in] _cumulative_roles Pointer to a set of cumulative roles.
     *  @param[in] _non_cumulative_roles Pointer to a set of non-cumulative roles.
     */
     BufferNode(TAG _tag, BufferNode * _parent,
                vector < unsigned >*_cumulative_roles,
                vector < unsigned >*_non_cumulative_roles);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ BufferNode();

    /*! @fn void addChild(BufferNode* child)
     *  @brief Adds a child node for this BufferNode object.
     *  @details The child is added at the rightmost position, thus following document order.
     *  @param[in] child The child node to be added.
     *  @return void
     */
    void addChild(BufferNode * child);

    /*! @fn void appendRoles(vector<unsigned>* _cumulative_roles, vector<unsigned>* _non_cumulative_roles)
     *  @brief Appends the specified roles to the buffer node.
     *  @details The method is intended to be called only once, i.e. previous role
     *    information is overwritten.
     *  @param[in] _cumulative_roles Pointer to a set of cumulative roles.
     *  @param[in] _non_cumulative_roles Pointer to a set of non-cumulative roles.
     *  @retval void
     */
    void appendRoles(vector < unsigned >*_cumulative_roles,
                     vector < unsigned >*_non_cumulative_roles);

    /*! @fn void print(OutputStream& eos)
     *  @brief Prints the subtree defined by the BufferNode object.
     *  @details The output contains no role information. This method can be used
     *    for output during regular query evaluation.
     *  @param[in] eos Reference to the (evaluation) OutputStream.
     *  @retval void
     */
    void print(OutputStream & eos);

    /*! @fn void printNoSubnodes(OutputStream& dos)
     *  @brief Debug prints the BufferNode without its child nodes.
     *  @details The output also contains role information, so this method is
     *    suited for debug purpose only.
     *  @param[in] dos Reference to the (debug) OutputStream.
     *  @retval void
     */
    void printNoSubnodes(OutputStream & dos);

    /*! @fn void debugPrint(OutputStream& dos)
     *  @brief Debug prints the subtree defined by the BufferNode object.
     *  @details The output also contains role information, so this method is
     *    suited for debug purpose only.
     *  @param[in] dos Reference to the (debug) OutputStream.
     *  @retval void
     */
    void debugPrint(OutputStream & dos);

    /*! @fn void debugPrintCompleteBuffer(OutputStream& dos)
     *  @brief Debug prints the complete buffer (i.e. starting with the root).
     *  @details The output will also contain role information, so this method
     *    is suited for debug purpose only. This method is realized by climbing
     *    up to the buffer root node and then calling debugPrint().
     *  @param[in] dos Reference to the (debug) OutputStream.
     *  @retval void
     */
    void debugPrintCompleteBuffer(OutputStream & dos);

    /*! @fn const char* getPCDataRepresentation()
     *  @brief Returns the PCDATA representation of the buffer node.
     *  @details This is done by appending the PCDATA content of the child nodes.
     *  @note The caller is responsible to free() the return value.
     *  @retval const char* The PCDATA representation of the node.
     */
    const char *getPCDataRepresentation();

    /*! @fn void getPCDataRepresentation(std::ostringstream& s)
     *  @brief Collects the character child nodes in the passed input stream.
     *  @details This is a help method of getPCDataRepresentation()
     *  @param[in] s A std::ostringstream where the PCDATA representation is
     *    appended to.
     *  @retval void
     */
    void getPCDataRepresentation(std::ostringstream & s);

    /*! @fn inline void close()
     *  @brief Marks the associated buffer TagNode object as closed.
     *  @details This method applies only to tag buffer nodes, i.e.
     *    requires that the member variable type is set to TYPE_TAG. The caller
     *    must assert that this condition holds, otherwise the method is likely
     *    to crash. 
     *  @return void
     */
    inline void close() {
        ((TagNode *) node)->is_closed = true;
    }
    /*! @fn inline bool isClosed()
     *  @brief Returns true if the associated buffer TagNode is marked as closed,
     *    false otherwise. 
     *  @details This method applies only to tag buffer nodes, i.e.
     *    requires that the member variable type is set to TYPE_TAG. The caller
     *    must assert that this condition holds, otherwise the method is likely
     *    to crash (after all, checking for a CharNode to be closed makes no sense).
     *  @return void
     */ inline bool isClosed() {
        return ((TagNode *) node)->is_closed;
    }

    /*! @fn inline void lock()
     *  @brief Locks the BufferNode object.
     *  @details Locks the BufferNode object by setting the member is_locked to true.
     *  @retval void
     */
    inline void lock() {
        is_locked = true;
    }

    /*! @fn inline void unlock()
     *  @brief Unlocks the BufferNode object and triggers garbage collection (if possible).
     *  @details Unlocks the BufferNode object by releasing the member variable is_locked
     *    to true. In addition, active garbage collection is triggered.
     *  @retval void
     */
    inline void unlock() {
        is_locked = false;
    }

    /*! @fn inline bool isLocked ()
     *  @brief Returns true if the buffer node is locked, false otherwise. 
     *  @details This is a simple check for the value of the member variable is_locked.
     *  @retval bool Returns true if the buffer node is locked, false otherwise.
     */
    inline bool isLocked() {
        return is_locked;
    }

    /*! @fn inline bool isRoot()
     *  @brief Returns true if the buffer node is the document root, false otherwise.
     *  @details Returns true if the buffer node is the (virtual) XML document root node,
     *    i.e. the topmost node in the buffer, false otherwise.
     *  @return bool Returns true if this node is the virtual document root, false otherwise.
     */
    inline bool isRoot() {
        return type == TYPE_TAG && ((TagNode *) node)->tag == TAGID_ROOT;
    }

    /*! @fn bool isMarked()
     *  @brief Returns true if the BufferNode object carries cumulative or non-cumulative roles.
     *  @details The method does not take into consideration cumulative roles of ancestors.
     *  @retval bool Returns true if the BufferNode object directly carries cumulative
     *    or non-cumulative roles, false otherwise.
     */
    bool isMarked();

    /*! @fn bool isCumulativeMarked()
     *  @brief Returns true if the BufferNode object carries a cumulativ role.
     *  @details The method does not take into consideration cumulative roles of ancestors.
     *  @retval bool Returns true if the BufferNode object carries one or more
     *    cumulative roles, false otherwise.
     */
    bool isCumulativeMarked();

    /*! @fn bool hasCumulativeMarkedAncestor()
     *  @brief Returns true if the BufferNode object has an ancestor with a cumulative role.
     *  @details In particular, ancestors with non-cumulative roles are ignored.
     *  @retval bool Returns true if at least one ancestor BufferNode carries one or
     *    more cumulative roles.
     */
    bool hasCumulativeMarkedAncestor();

    /*! @fn bool hasNoMarkedAndNoLockedDos()
     *  @brief Returns true if the BufferNode object has any marked
     *    or locked descendants or is marked/locked itself.
     *  @details The type of the marking (cumulative vs. non-cumulative) roles does
     *    not play a role.
     *  @retval Returns true if the BufferNode object has any marked
     *    or locked descendants or is marked/locked itself.
     */
    bool hasNoMarkedAndNoLockedDos();

    /*! @fn void removeRoleFromNode(Role* role)
     *  @brief Removes the specified role from the BufferNode object.
     *  @details Removes the specified role from the BufferNode object and triggers
     *    garbage collection, if possible.
     *  @param[in] role Pointer to the Role that is to be removed.
     *  @retval void
     */
    void removeRoleFromNode(Role * role);

    /*! @fn void clear()
     *  @brief Triggers garbage collection, starting at this BufferNode object.
     *  @details Garbage collection is propagated downwards and upwards, if possible.
     *  @retval void
     */
    void clear();

    /*! @fn void clearSubtreeIfPossible()
     *  @brief This garbage collection method tries to garbage collect the whole subtre
     *     of the node.
     *  @details Implements downward propagation of garbage collection.
     *  @retval void
     */
    void clearSubtreeIfPossible();

    /*! @var bool type
     *  @brief The type of the buffer node.
     *  @details The type is either TYPE_TAG or TYPE_PCDATA and serves as a switch for
     *    casting the void* node member, either to a TagNode* or CharNode* object.
     */
    bool type;

    /*! @var BufferNode* parent
     *  @brief The parent of the buffer node object.
     *  @details Set to NULL for the document root.
     */
    BufferNode *parent;

    /*! @var BufferNode* r_sibling
     *  @brief The right sibling of the buffer node object.
     *  @details Set to NULL if no right sibling exists.
     */
    BufferNode *r_sibling;

#ifdef ROLE_REFCOUNT
    /*! @var unsigned cumulative_roles
     *  @brief The number of cumulative roles.
     *  @details The number of cumulative roles carried by this buffer node.
     *  @note This member variable is used if GCX is compiled with
     *    -DROLE_REFCOUNT turned on.
     */
    unsigned cumulative_roles;
#else
    /*! @var IntMultiSet* cumulative_roles
     *  @brief The set of cumulative roles.
     *  @details The IntMultiSet specifying all cumulative roles associated
     *    with this buffer node.
     *  @note This member variable is used if GCX is compiled with
     *    -DROLE_REFCOUNT turned off, which is useful for debugging purpose.
     */
    IntMultiSet *cumulative_roles;
#endif // #ifdef ROLE_REFCOUNT

#ifdef ROLE_REFCOUNT
    /*! @var unsigned non_cumulative_roles
     *  @brief The number of non-cumulative roles.
     *  @details The number of non-cumulative roles carried by this buffer node.
     *  @note This member variable is used if GCX is compiled with
     *    -DROLE_REFCOUNT turned on.
     */
    unsigned non_cumulative_roles;
#else
    /*! @var IntMultiSet* non_cumulative_roles
     *  @brief The set of non-cumulative roles.
     *  @details The IntMultiSet specifying all non-cumulative roles associated
     *    with this buffer node.
     *  @note This member variable is used if GCX is compiled with
     *    -DROLE_REFCOUNT turned off, which is useful for debugging purpose.
     */
    IntMultiSet *non_cumulative_roles;
#endif // #ifdef ROLE_REFCOUNT

    /*! @var void* node
     *  @brief The content carried by the BufferNode object.
     *  @details This variable is either of type TagNode* or of type CharNode*, i.e.
     *    represents either the character or tag node data. The member variable
     *    type serves as a switch and can be queries to cast the node accordingly.
     */
    void *node;

    /*! @var bool is_locked
     *  @brief A flag that can be used to lock the BufferNode.
     *  @details Locking prevents the BufferNode from being deleted. It is necessary to ensure
     *    that nodes referenced by buffer iterators will not be deleted too early, which
     *    might cause undefined pointers. 
     */
    bool is_locked;
};

#endif // BUFFERNODE_H
