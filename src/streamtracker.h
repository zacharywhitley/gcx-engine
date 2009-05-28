
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
 *  @brief Header file for streamtracker.cpp.
 *  @details Header file specifying constructors, destructor and functions for streamtracker.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef STREAMTRACKER_H
#define STREAMTRACKER_H

#include "buffer.h"

#ifdef VALIDATION
#include "parseexception.h"
#endif // #ifdef VALIDATION

/*! @class StreamTracker
 *  @brief Keeps track of the input stream and appends data to the buffer.
 *  @details This class communicates directly with the buffer. It keeps
 *    track of the current state in the ProjectionDFA and, based on
 *    this state, decides whether new tags or PCDATA content is
 *    appended to the buffer or discarded. Furthermore, it is
 *    responsible for adding roles to buffer where necessary.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class StreamTracker {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for a ProjectionDFA and a Buffer.
     *  @param[in] _pdfa The projection DFA that is used for stream preprojection.
     *  @param[in] _buffer The buffer where data is stored in.
     */
    StreamTracker(ProjectionDFA * _pdfa, Buffer * _buffer);

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ StreamTracker();

    /*! @fn void processOpeningTag(const char* tag)
     *  @brief Processes an opening tag.
     *  @details Depending on the current state of the projection DFA,
     *    the token is either projected away or stored in the
     *    buffer (together with associated roles).
     *  @param[in] tag The opening tag to be processed.
     *  @retval void
     */
    void processOpeningTag(const char *tag);

    /*! @fn void processBachelorTag(const char* tag)
     *  @brief Processes a bachelor tag.
     *  @details Depending on the current state of the projection DFA,
     *    the token is either projected away or stored in the
     *    buffer (together with associated roles).
     *  @param[in] tag The bachelor tag to be processed.
     *  @retval void
     */
    void processBachelorTag(const char *tag);

#ifdef VALIDATION
    /*! @fn void processClosingTag(TAG tag)
     *  @brief Processes a closing tag.
     *  @details Depending on the current state of the projection DFA,
     *    the closing tag is either ignored or the corresponing
     *    opening tag in the buffer is marked as closed.
     *  @param[in] tag The closing tag to be processed.
     *  @retval void
     *  @exception May throw a ParseException if -DVALIDATION is turned on.
     */
    void processClosingTag(TAG tag);
#endif                          // #ifdef VALIDATION


#ifdef VALIDATION
    /*! @fn processClosingTag(const char* _tag)
     *  @brief Processes a closing tag.
     *  @details Depending on the current state of the projection DFA,
     *    the closing tag is either ignored or the corresponing
     *    opening tag in the buffer is marked as closed.
     *  @param[in] tag The character representation of th closing tag.
     *  @retval void
     *  @exception May throw a ParseException if -DVALIDATION is turned on.
     */
    void processClosingTag(const char *_tag);
#else
    /*! @fn void processClosingTag()
     *  @brief Processes a closing tag.
     *  @details Depending on the current state of the projection DFA,
     *    the closing tag is either ignored or the corresponing
     *    opening tag in the buffer is marked as closed.
     *  @retval void
     *  @exception May throw a ParseException if -DVALIDATION is turned on.
     */
    void processClosingTag();
#endif                          // #ifdef VALIDATION

    /*! @fn void processPCData(const char* data)
     *  @brief Processes PCDATA content. 
     *  @details Depending on the current state of the projection DFA,
     *    the PCDATA content is either projected away or stored in the buffer
     *  @param[in] data The PCDATA content.
     *  @retval void
     */
    void processPCData(const char *data);

  private:
    /*! @var ProjectionDFAState* cur
     *  @brief The current state of the projection DFA.
     *  @details This variable is used to permanently keep track of
     *    the current projection DFA state. It determines
     *    the actions to be taken when opening/closing tags
     *    or PCDATA content is received.
     */
     ProjectionDFAState * cur;

    /*! @var Buffer* buffer
     *  @brief Pointer to the Buffer where data is stored in.
     *  @details This member variable allows for direct manipulation of
     *    the one and only Buffer used during evaluation. 
     */
    Buffer *buffer;

    /*! @var TagMap* tagmap
     *  @brief Map for translating tags to their integer ids and vice versa.
     *  @details Allows for the efficient retrieval of tag ids.
     */
    TagMap *tagmap;

    /*! @var unsigned skip_subtree_depth
     *  @brief Depth tracker used to efficiently skip whole subtrees.
     *  @details Whenever subtrees are irrelevant, this member variable
     *    is used to keep track of the current depth, which allows for
     *    fast skipping of subtrees.
     */
    unsigned skip_subtree_depth;

    /*! @var unsigned keep_subtree_depth
     *  @brief Depth tracker used to efficiently keep whole subtrees.
     *  @details Whenever subtrees are stored completely in the buffer,
     *    this. member variable is used to keep track of the current depth
     *    in the relevant subtree, allowing for faster processing.
     */
    unsigned keep_subtree_depth;
};

#endif // STREAMTRACKER_H
