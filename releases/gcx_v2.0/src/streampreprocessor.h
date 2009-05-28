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
 * 	@brief Header file for streampreprocessor.cpp.
 * 	@details Header file specifying constructors, destructor and functions for streampreprocessor.cpp.
 * 	@author Michael Schmidt
 * 	@author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef STREAMPREPROCESSOR_H
#define STREAMPREPROCESSOR_H

#include "buffer.h"
#include "streamtracker.h"

// xml file and xml file parser
extern FILE* yyxmlin;
extern int saxparse();
extern void destroyLexer();

/*! @class StreamPreProcessor
 * 	@brief The StreamPreprocessor manages prefiltering of the input stream
 * 	@details The stream preprocessor controls static stream projection,
 * 	         which bases on an analysis of the paths used in the input query.
 * 	         It provides the getNext()-function, which allows the query
 * 	         evaluator to project new data into the buffer on demand.
 * 	         This is realized by passing data to the StreamTracker, which
 * 	         itself communicates directly with the buffer.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
class StreamPreProcessor {

public:
	/*! @fn static void initInstance(Buffer* _buffer,const char* _xml_file, ProjectionDFA* _pdfa)
     * @brief Initializes the one and only stream preprocessor instance (singleton).
     * @details Initializes the stream preprocessor. The initialized instance can then
     *          be accessed using method getInstance().
     * @note CALL THIS METHOD BEFORE METHOD getInstance()!
     * @param[in] _buffer A pointer to the buffer where data is stored in.
     * @param[in] _xml_file The file name of the XML input document.
     * @param[in] _pdfa The projection DFA that is used for stream preprojection.
     * @retval void
     */

    static void initInstance(Buffer* _buffer, const char* _xml_file,
                             ProjectionDFA* _pdfa);
    
	/*! @fn static StreamPreProcessor* getInstance()
     * @brief Returns the one and only instance of the stream preprocessor.
     * @details Returns the one and only instance of the stream preprocessor. The
     *          method should be called after having initialized the instance
     *          using method initInstance().
     * @note DO NOT CALL THIS METHOD BEFORE METHOD initInstance().
     * @retval StreamPreprocessor* The global stream preprocessor object.
     */
    static StreamPreProcessor* getInstance();

	/*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~StreamPreProcessor();

	/*! @brief Process the passed opening tag.
     *  @details The opening tag parameter is, depending on the state of
     *           projection dfa, either appended to the buffer or discarded.
     *           If appended, roles are also added to the buffer.
     *  @param[in] tag The name of the tag that has been encountered in the input stream
     *  @retval void
     */
    inline void processOpeningTag(const char* tag) { st.processOpeningTag(tag); }

	/*! @brief Process the passed bachelor tag.
     *  @details The bachelor tag parameter is, depending on the state of
     *           projection dfa, either appended to the buffer or discarded.
     *           If appended, roles are also added to the buffer.
     *  @param[in] tag The name of the bachelor tag that read in the input stream
     *  @retval void
     */
    inline void processBachelorTag(const char* tag) { st.processBachelorTag(tag); }

	/*! @brief Process closing tag message.
     *  @details If the corresponding opening that has not been projected away,
     *           the active buffer opening tag is marked as closed. 
     *  @param[in] tag The name of the tag that has been encountered in the input stream
     *  @retval void
     */
	#ifdef VALIDATION
    inline void processClosingTag(const char* tag) { st.processClosingTag(tag); }
	#else
    inline void processClosingTag() { st.processClosingTag(); }
	#endif // #ifdef VALIDATION

	/*! @brief Process PCDATA content.
     *  @details The PCDATA content is, depending on the state of
     *           projection dfa, either appended to the buffer or discarded.
     *  @param[in] data The PCDATA content that has been encountered in the input stream
     *  @retval void
     */
    inline void processPCData(const char* data) { st.processPCData(data); }

	/*! @brief Projects another token into the buffer.
     *  @details This is done by querying the next token from the SAX parser.
     *           As a response, typically one of the three methods
     *           processOpeningTag(), processPCData(), or closeTag() will be called.
     *  @retval void
     */
    void readNext();

	/*! @brief Projects the whole input stream into the buffer. 
     *  @details This method is not used in typical runs, but required if only
     *           input stream projection is carried out. Be aware that, when
     *           using this method, the whole document will be loaded into
     *           the buffer and the benefit of the signOff mechanism will be
     *           totally lost.
     *  @retval void
     */
    void readAll();

private:
	/*! @brief The constructor. 
     *  @details The constructor is private, due to the singleton pattern.
     *  @param[in] _buffer A pointer to the buffer where data is stored in.
     *  @param[in] _xml_file The file name of the XML input document.
     *  @param[in] _pdfa The projection DFA that is used for stream preprojection.
     */
    StreamPreProcessor(Buffer* _buffer, const char* _xml_file, ProjectionDFA* _pdfa);

	/*! @var static StreamPreProcessor* instance
     *  @brief Instance of StreamPreProcessor.
     *  @details Instance of StreamPreProcessor to avoid duplicate existence of this object.
     */
    static StreamPreProcessor* instance;

	/*! @var TagMap* tagmap
     *  @brief Map for translating tags to their integer ids and vice versa.
     */
    TagMap* tagmap;

	/*! @var TAG tag
     *  @brief The last recently processed opening tag
     */
    TAG tag;

	/*! @var StreamTracker st 
     *  @brief The stream tracker gets passed all data from the StreamPreprocessor
     *         and communicates directly with the buffer.
     */
    StreamTracker st;
};

#endif // STREAMPREPROCESSOR_H
