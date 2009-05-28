
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
 *  @brief Implementations of header file streampreprocessor.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file streampreprocessor.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "streampreprocessor.h"

StreamPreProcessor *StreamPreProcessor::instance = NULL;

void StreamPreProcessor::initInstance(Buffer * _buffer, ProjectionDFA * _pdfa) {
    instance = new StreamPreProcessor(_buffer, _pdfa);
}

StreamPreProcessor *StreamPreProcessor::getInstance() {
    return instance;
}

StreamPreProcessor::StreamPreProcessor(Buffer * _buffer, ProjectionDFA * _pdfa):
tagmap(TagMap::getInstance()), st(_pdfa, _buffer) {
    InputStream *xml_istream =
        StreamManager::getInstance()->getXMLInputStream();
    yyxmlin = xml_istream->getParserStream();
}

StreamPreProcessor::~StreamPreProcessor() {
    destroyLexer();
}

void StreamPreProcessor::readNext() {
    if (!saxparse()) {
#ifdef VALIDATION
        st.processClosingTag((TAG) TAGID_ROOT); // close the root 
#else
        st.processClosingTag(); // close the root 
#endif // #ifdef VALIDATION
    }
}

void StreamPreProcessor::readAll() {
    while (saxparse()) {
    }

#ifdef VALIDATION
    st.processClosingTag((TAG) TAGID_ROOT);     // close the root
#else
    st.processClosingTag();     // close the root
#endif // #ifdef VALIDATION
}
