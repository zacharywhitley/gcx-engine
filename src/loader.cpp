
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
 *  @brief SAX-PARSER.
 *  @details SAX-PARSER.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "loader.h"

/*! @fn void saxBeginElement(const char* _tag)
 *  @brief SAX event handler for opening tags.
 *  @details Passes the opening tag to the StreamPreProcessor object.
 *  @param[in] _tag The opening that has been received.
 *  @retval void
 */
void saxBeginElement(const char *_tag) {
    StreamPreProcessor::getInstance()->processOpeningTag(_tag);
}

/*! @fn void saxBeginEndElement(const char* _tag)
 *  @brief SAX event handler for bachelor tags.
 *  @details Passes the bachelor tag to the StreamPreProcessor object. 
 *  @param[in] _tag The bachelor that has been received.
 *  @retval void
 */
void saxBeginEndElement(const char *_tag) {
    StreamPreProcessor::getInstance()->processBachelorTag(_tag);
}

#ifdef VALIDATION

/*! @fn void saxEndElement(const char* _tag)
 *  @brief SAX event handler for closing tags.
 *  @details Passes the closing tag to the StreamPreProcessor object.
 *  @param[in] _tag The closing tag that has been received.
 *  @retval void
 *  @note This method is used if -DVALIDATION is turned on.
 *  @exception Throws a ParseException if ill-formed XML is detected.
 */
void saxEndElement(const char *_tag) {
    StreamPreProcessor::getInstance()->processClosingTag(_tag);
}
#else

/*! @fn void saxEndElement()
 *  @brief SAX event handler for closing tags.
 *  @details Notifies the StreamPreProcessor object that a closing tag 
 *    has been read.
 *  @retval void
 *  @note This method is used if -DVALIDATION is turned off.
 *  @exception Throws a ParseException if ill-formed XML is detected.
 */
void saxEndElement() {
    StreamPreProcessor::getInstance()->processClosingTag();
}
#endif // #ifdef VALIDATION

/*! @fn void saxCharacters(const char* _text)
 *  @brief SAX event handler for PCDATA content.
 *  @details Passes the PCDATA content to the StreamPreProcessor object.
 *  @param[in] _text The PCDATA content that has been received.
 *  @retval void
 */
void saxCharacters(const char *_text) {
    StreamPreProcessor::getInstance()->processPCData(_text);
}

/*! @fn extern int yyxmllex()
 *  @brief Triggers lexing of the input stream.
 *  @details Triggers lexing of the input stream. This method is externally
 *    implemented in the lexer.
 *  @retval int See lexer.
 */
extern int yyxmllex();

//extern int yyxmllex_init();

/*! @fn int saxparse()
 *  @brief Triggers parsing of the input stream.
 *  @details Triggers lexing of the input stream. This method is externally
 *    implemented in the parser.
 *  @retval int See method yyxmllex().
 */
int saxparse() {
    return yyxmllex();
}

/*! @fn extern int yyxmllex_destroy()
 *  @brief Destroys the lexer.
 *  @details To be called after lexing has been finished.
 *  @retval int See lexer.
 */
extern int yyxmllex_destroy();

/*! @fn void destroyLexer()
 *  @brief Destroys the lexer.
 *  @details Wrapper that calls yyxmlex(), depending on the architecture.
 *  @retval int See yyxmllex_destroy().
 *  @note This method was made necessary by the fact that Flex does not
 *    not provide up-to-date versions under Windows.
 */
void destroyLexer() {
#if defined(LINUX) || defined(linux) || defined(_LINUX) || defined(_linux) || defined(__LINUX__) || defined(__linux__)
    yyxmllex_destroy();
#endif
}
