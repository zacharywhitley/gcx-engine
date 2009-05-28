
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
 *  @brief Header file for buffernodelist.cpp.
 *  @details Header file specifying constructors, destructor and functions for buffernodelist.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef BUFFERNODELIST_H
#define BUFFERNODELIST_H

#include <vector>

// forward declarations
class BufferNode;
class OutputStream;

/*! @class BufferNodeList
 *  @brief Represents a list of buffer nodes.
 *  @details An (ordered) list of BufferNode elements. The BufferNode objects are linked via
 *    the BufferNode::r_sibling member variable.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class BufferNodeList {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object.
     */
    BufferNodeList();

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ BufferNodeList();

    /*! @fn void push_back(BufferNode* node)
     *  @brief Appends another BufferNode to the list.
     *  @details The passed BufferNode is appended to the end of the list.
     *  @param[in] node Pointer to the BufferNode to be appended to the list.
     *  @retval void
     */
    void push_back(BufferNode * node);

    /*! @fn BufferNode* operator[](unsigned index)
     *  @brief Retrieves the BufferNode at position index.
     *  @details The first node in the list has index zero. For efficiency reasons,
     *    this method does not check for out-of-bound violation, so the caller is
     *    responsible to make sure to access buffer nodes in the valid range.
     *  @param[in] index The position of the BufferNode in the list.
     *  @retval BufferNode* Pointer to the retrieved BufferNode.
     */
    BufferNode *operator[] (unsigned index);

    /*! @fn void print(OutputStream& dos)
     *  @brief Prints the complete BufferNodeList.
     *  @details This is a debug print method: the output comprises role information.
     *  @param[in] dos Reference to the (debug) OutputStream.
     *  @retval void
     */
    void print(OutputStream & dos);

    /*! @fn inline BufferNode* getFront()
     *  @brief Retrieves the first element of the BufferNodeList.
     *  @details May return NULL if the list is empty.
     *  @retval BufferNode* Pointer to the first BufferNode in the list.
     */
    inline BufferNode *getFront() {
        return front;
    }
    /*! @fn inline bool isEmpty()
     *  @brief Checks for emptyness.
     *  @details Checks for emptyness, i.e. returns true if this is the empty list.
     *  @retval bool Returns true if the BufferNodeList object is empty, false otherwise.
     */ inline bool isEmpty() {
        return front == NULL;
    }

    /*! @fn void removeElem(BufferNode* to_remove, BufferNode* prev)
     *  @brief Removes a node from the BufferNodeList.
     *  @details The node to_remove is removed from the list; node prev is required
     *    to adjust the linked list after node removal.
     *  @param[in] to_remove The BufferNode object to be removed.
     *  @param[in] prev The predecessor of the object to be removed. May be in case
     *    the first removed node is the first one in the list. 
     *  @retval void
     */
    void removeElem(BufferNode * to_remove, BufferNode * prev);

  private:
    /*! @var BufferNode* front
     *  @brief The front element of the list.
     *  @details May be NULL (if the list is empty).
     */
    BufferNode * front;

    /*! @var BufferNode* back 
     *  @brief The last element of the list.
     *  @details May be NULL (if the list is empty).
     */
    BufferNode *back;
};

#endif // BUFFERNODELIST_H
