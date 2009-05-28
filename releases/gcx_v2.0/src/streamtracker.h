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
 * 	@brief Header file for streamtracker.cpp.
 * 	@details Header file specifying constructors, destructor and functions for streamtracker.cpp.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 *  @license Software License Agreement (BSD License)
 */
#ifndef STREAMTRACKER_H
#define STREAMTRACKER_H

#include "buffer.h"

/*! @class StreamTracker
 * 	@brief Keeps track of the input stream and appends data to the buffer.
 * 	@details This class communicates directly with the buffer. It keeps
 * 	         track of the current state in the ProjectionDFA and, based on
 * 	         this state, decides whether new tags or PCDATA content is
 * 	         appended to the buffer or discarded. Furthermore, it is
 * 	         responsible for adding roles to buffer where necessary.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
class StreamTracker {

public:
	/*! @brief Constructor.
     *  @details Constructor - creates a new StreamTracker object.
     *  @param[in] _pdfa The projection DFA that is used for stream preprojection.
     *  @param[in] _buffer The buffer where data is stored in.
     */
    StreamTracker(ProjectionDFA* _pdfa, Buffer* _buffer);

	/*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~StreamTracker();

	/*! @brief Processes an opening tag.
     *  @details Depending on the current state of the projection DFA,
     *           the token is either projected away or stored in the
     *           buffer (together with associated roles).
     *  @param[in] tag The opening tag to be processed.
     *  @retval void
     */
    void processOpeningTag(const char* tag);

	/*! @brief Processes a bachelor tag.
     *  @details Depending on the current state of the projection DFA,
     *           the token is either projected away or stored in the
     *           buffer (together with associated roles).
     *  @param[in] tag The bachelor tag to be processed.
     *  @retval void
     */
    void processBachelorTag(const char* tag);

	#ifdef VALIDATION
	/*! @brief Processes a closing tag.
     *  @details Depending on the current state of the projection DFA,
     *           the closing tag is either ignored or the corresponing
     *           opening tag in the buffer is marked as closed.
     *  @param[in] tag The closing tag to be processed.
     *  @retval void
     */
    void processClosingTag(TAG tag);
	#endif // #ifdef VALIDATION

	/*! @brief Processes a closing tag.
     *  @details Depending on the current state of the projection DFA,
     *           the closing tag is either ignored or the corresponing
     *           opening tag in the buffer is marked as closed.
     *  @param[in] tag The character representation of th closing tag.
     *  @retval void
     */
	#ifdef VALIDATION
    void processClosingTag(const char* _tag);
	#else
    void processClosingTag();
	#endif // #ifdef VALIDATION

	/*! @brief Processes PCDATA content. 
     *  @details Depending on the current state of the projection DFA,
     *           the PCDATA content is either projected away or stored in
     *           the buffer
     *  @param[in] data The PCDATA content.
     *  @retval void
     */
    void processPCData(const char* data);

private:
	/*! @var ProjectionDFAState* cur
     *  @brief The current state of the projection DFA.
     *  @details This variable is used to permanently keep track of
     *           the current projection DFA state. It determines
     *           the actions to be taken when opening/closing tags
     *           or PCDATA content is received.
     */
    ProjectionDFAState* cur;

	/*! @var Buffer* buffer
     *  @brief The buffer where data is stored in.
     */ 
    Buffer* buffer;

	/*! @var TagMap* tagmap
     *  @brief Map for translating tags to their integer ids and vice versa.
     */
    TagMap* tagmap;

	/*! @var unsigned skip_subtree_depth
     *  @brief Depth tracker used to efficiently skip whole subtrees.
     */
	unsigned skip_subtree_depth;

	/*! @var unsigned keep_subtree_depth
     *  @brief Depth tracker used to efficiently keep whole subtrees.
     */
	unsigned keep_subtree_depth;
};

#endif // STREAMTRACKER_H
