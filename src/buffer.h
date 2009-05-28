
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
 *  @brief Header file for buffer.cpp.
 *  @details Header file specifying constructors, destructor and functions for buffer.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef BUFFER_H
#define BUFFER_H

#include "buffernode.h"
#include "projectiondfa.h"

/*! @class Buffer
 *  @brief Represents the DOM buffer.
 *  @details The buffer is implemented as a DOM tree. The buffer may contain
 *    both tag and character nodes, and the class provides methods
 *    for appending suchlike node types.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class Buffer {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for a ProjectionDFA.
     *  @param[in] _pdfa The ProjectionDFA is used to assign the roles to the document root.
     */
    Buffer(ProjectionDFA * _pdfa);

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ Buffer();

    /*! @fn void appendTag(TAG tag)
     *  @brief Appends a tag node to the buffer.
     *  @details The tag node is appended at the current position of the buffer.
     *  @param[in] tag The tag id of the node that will be created.
     *  @retval void
     */
    void appendTag(TAG tag);

    /*! @fn void appendTag(TAG tag, vector<unsigned>* _cumulative_roles, vector<unsigned>* _non_cumulative_roles)
     *  @brief Appends a tag node including roles to the buffer.
     *  @details The tag node is appended at the current position of the buffer.
     *  @param[in] tag The tag id of the node that will be created.
     *  @param[in] _cumulative_roles Pointer to a vector containing cumulative roles.
     *  @param[in] _non_cumulative_roles Pointer to a vector containing non-cumulative roles.
     *  @retval void
     */
    void appendTag(TAG tag, vector < unsigned >*_cumulative_roles,
                   vector < unsigned >*_non_cumulative_roles);

    /*! @fn void appendPCData(const char* data)
     *  @brief Appends a text node to the buffer.
     *  @details The text node is appended at the current position of the buffer.
     *  @param[in] data The PCDATA content of the node that will be created.
     *  @retval void
     */
    void appendPCData(const char *data);

    /*! @fn void appendPCData(const char* data, vector<unsigned>* _cumulative_roles, vector<unsigned>* _non_cumulative_roles)
     *  @brief Appends a text node including some roles to the buffer.
     *  @details The text node is appended at the current position of the buffer.
     *  @param[in] data The PCDATA content of the node that will be created.
     *  @param[in] _cumulative_roles Pointer to a vector containing cumulative roles.
     *  @param[in] _non_cumulative_roles Pointer to a vector containing non-cumulative roles.
     *  @retval void
     */
    void appendPCData(const char *data, vector < unsigned >*_cumulative_roles,
                      vector < unsigned >*_non_cumulative_roles);

    /*! @fn void appendRoles(vector<unsigned>* _cumulative_roles, vector<unsigned>* _non_cumulative_roles)
     *  @brief Appends a set of roles to the current buffer node.
     *  @param[in] _cumulative_roles Pointer to a vector containing cumulative roles.
     *  @param[in] _non_cumulative_roles Pointer to a vector containing non-cumulative roles.
     *  @retval void
     */
    void appendRoles(vector < unsigned >*_cumulative_roles,
                     vector < unsigned >*_non_cumulative_roles);

    /*! @fn void print(OutputStream& dos)
     *  @brief Prints the buffer.
     *  @details Prints the complete buffer content to the debug output stream.
     *  @param[in] dos Reference to the (debug) OutputStream.
     *  @retval void
     */
    void print(OutputStream & dos);

    /*! @fn void printCurrent(OutputStream& dos)
     *  @brief Prints the current node of the buffer (including its subtree).
     *  @details Prints the node that is currently active to the debug output stream,
     *    including its subtree.
     *  @param[in] dos Reference to the (debug) OutputStream.
     *  @retval void
     */
    void printCurrent(OutputStream & dos);

    /*! @fn void debugPrint(OutputStream& dos)
     *  @brief Prints the buffer including debug and role information.
     *  @details Prints the complete buffer to the debug output stream, including
     *    idebug and role information.
     *  @param[in] dos Reference to the (debug) OutputStream.
     *  @retval void
     */
    void debugPrint(OutputStream & dos);

    /*! @fn inline BufferNode* getRoot()
     *  @brief Returns the document root.
     *  @details The document root is the virtual root node of the XML document.
     *  @retval BufferNode* Pointer to the document root.
     */
    inline BufferNode *getRoot() {
        return root;
    }
    /*! @fn inline BufferNode* getCurrent()
     *  @brief Returns the current buffer node.
     *  @details Returns the current buffer node, i.e. the one that has last recently
     *    been read into the buffer.
     *  @retval BufferNode* Pointer to the current BufferNode.
     */ inline BufferNode *getCurrent() {
        return cur;
    }

    /*! @fn void closeTag()
     *  @brief Closes the current tag node in the buffer.
     *  @details This is simply realized by marking the tag node as closed.
     *  @retval void
     */
    void closeTag();

  private:
    /*! @var BufferNode* root
     *  @brief The root node of the buffer.
     *  @details The root node of the buffer, i.e. the virtual document root.
     */
    BufferNode * root;

    /*! @var BufferNode* cur
     *  @brief The current node of the buffer.
     *  @details The current node of the buffer, i.e. the node that has been
     *    appended or closed last recently.
     */
    BufferNode *cur;
};

#endif // BUFFER_H
