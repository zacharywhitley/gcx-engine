
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
 *  @brief Header file for streampreprocessor.cpp.
 *  @details Header file specifying constructors, destructor and functions for streampreprocessor.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef STREAMPREPROCESSOR_H
#define STREAMPREPROCESSOR_H

#include "buffer.h"
#include "streamtracker.h"
#include "streammanager.h"
#include "parseexception.h"

/*! @var extern FILE* yyxmlin
 *  @brief The associated XML file.
 *  @details This is an extern variable, which is defined in sax.C.
 */
extern FILE *yyxmlin;

/*! @fn extern int saxparse()
 *  @brief The main parsing method.
 *  @details This is an extern method, which is defined in loader.cpp.
 *  @retval int See the original definition in loader.C.
 */
extern int saxparse();

/*! @fn extern void destroyLexer()
 *  @brief Destroys the lexer.
 *  @details This is an extern method, which is defined in loader.cpp.
 *  @retval void
 */
extern void destroyLexer();

/*! @class StreamPreProcessor
 *  @brief The StreamPreprocessor manages prefiltering of the input stream
 *  @details The stream preprocessor controls static stream projection,
 *    which bases on an analysis of the paths used in the input query.
 *    It provides the getNext()-function, which allows the query
 *    evaluator to project new data into the buffer on demand.
 *    This is realized by passing data to the StreamTracker, which
 *    itself communicates directly with the buffer.
 *  @note The class is implemented as Singleton pattern, so there cannot
 *    be more than one instance of this class.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class StreamPreProcessor {

  public:
    /*! @fn static void initInstance(Buffer* _buffer, ProjectionDFA* _pdfa)
     *  @brief Initializes the one and only stream preprocessor instance (singleton).
     *  @details Initializes the stream preprocessor. The initialized instance can then
     *    be accessed using method getInstance().
     *  @note CALL THIS METHOD BEFORE METHOD getInstance()!
     *  @param[in] _buffer A pointer to the buffer where data is stored in.
     *  @param[in] _pdfa The projection DFA that is used for stream preprojection.
     *  @retval void
     */
    static void initInstance(Buffer * _buffer, ProjectionDFA * _pdfa);

    /*! @fn static StreamPreProcessor* getInstance()
     *  @brief Returns the one and only instance of the stream preprocessor.
     *  @details Returns the one and only instance of the stream preprocessor. The
     *    method should be called after having initialized the instance
     *    using method initInstance().
     *  @note Do not call this method before having called initInstance().
     *  @retval StreamPreprocessor* The global stream preprocessor object.
     */
    static StreamPreProcessor *getInstance();

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ StreamPreProcessor();

    /*! @fn inline void processOpeningTag(const char* tag)
     *  @brief Process the passed opening tag.
     *  @details The opening tag parameter is, depending on the state of
     *    projection dfa, either appended to the buffer or discarded.
     *    If appended, roles are also added to the buffer.
     *  @param[in] tag The name of the tag that has been encountered in the input stream.
     *  @retval void
     */
    inline void processOpeningTag(const char *tag) {
        st.processOpeningTag(tag);
    }
    /*! @fn inline void processBachelorTag(const char* tag)
     *  @brief Process the passed bachelor tag.
     *  @details The bachelor tag parameter is, depending on the state of
     *    projection dfa, either appended to the buffer or discarded.
     *    If appended, roles are also added to the buffer.
     *  @param[in] tag The name of the bachelor tag that read in the input stream.
     *  @retval void
     */ inline void processBachelorTag(const char *tag) {
        st.processBachelorTag(tag);
    }


#ifdef VALIDATION
    /*! @fn inline void processClosingTag(const char* tag)
     *  @brief Process closing tag message.
     *  @details If the corresponding opening that has not been projected away,
     *    the active buffer opening tag is marked as closed. 
     *  @param[in] tag The name of the tag that has been encountered in the input stream.
     *  @retval void
     *  @note This method is used only if GCX compile flag -DVALIDATION is turned on.
     */
    inline void processClosingTag(const char *tag) {
        st.processClosingTag(tag);
    }
#else
    /*! @fn inline void processClosingTag()
     *  @brief Process closing tag message.
     *  @details If the corresponding opening that has not been projected away,
     *    the active buffer opening tag is marked as closed. 
     *  @retval void
     *  @note This method is used only if GCX compile flag -DVALIDATION is turned off.
     */
    inline void processClosingTag() {
        st.processClosingTag();
    }
#endif // #ifdef VALIDATION

    /*! @fn inline void processPCData(const char* data)
     *  @brief Process PCDATA content.
     *  @details The PCDATA content is, depending on the state of
     *    projection dfa, either appended to the buffer or discarded.
     *  @param[in] data The PCDATA content that has been encountered in the input stream
     *  @retval void
     */
    inline void processPCData(const char *data) {
        st.processPCData(data);
    }

    /*! @fn void readNext()
     *  @brief Projects another token into the buffer.
     *  @details This is done by querying the next token from the SAX parser.
     *    As a response, typically one of the three methods
     *    processOpeningTag(), processPCData(), or closeTag() will be called.
     *  @retval void
     */
    void readNext();

    /*! @fn void readAll()
     *  @brief Projects the whole input stream into the buffer. 
     *  @details This method is not used in typical runs, but required if only
     *    input stream projection is carried out. Be aware that, when
     *    using this method, the whole document will be loaded into
     *    the buffer and the benefit of the signOff mechanism will be
     *    totally lost. The purpose of this method is for debugging.
     *  @retval void
     */
    void readAll();

  private:
    /*! @var static StreamPreProcessor* instance
     *  @brief Instance of StreamPreProcessor.
     *  @details Instance of StreamPreProcessor to avoid duplicate existence of this object.
     */
    static StreamPreProcessor *instance;

    /*! @brief Constructor.
     *  @details Constructor - creating object for a Buffer and a ProjectionDFA.
     *  @param[in] _buffer A pointer to the buffer where data is stored in.
     *  @param[in] _pdfa The projection DFA that is used for stream preprojection.
     */
    StreamPreProcessor(Buffer * _buffer, ProjectionDFA * _pdfa);

    /*! @var TagMap* tagmap
     *  @brief Map for translating tags to their integer ids and vice versa.
     *  @details See class TagMap for more information.
     */
    TagMap *tagmap;

    /*! @var TAG tag
     *  @brief The last recently processed opening tag
     *  @details The last recently processed opening tag, mainly required to
     *    perform document validation.
     */
    TAG tag;

    /*! @var StreamTracker st 
     *  @brief The StreamTracker object used to keep track of the input stream.
     *  @details The StreamTracker object used to keep track of the input stream.
     *    Basically, the StreamPreprocessor passes the input stream to this object,
     *    which communicates directly with the buffer.
     */
    StreamTracker st;
};

#endif // STREAMPREPROCESSOR_H
