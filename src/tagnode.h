
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
 *  @brief Header file for tagnode.cpp.
 *  @details Header file specifying constructors, destructor and functions for tagnode.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef TAGNODE_H
#define TAGNODE_H

#include "tagmap.h"
#include "buffernodelist.h"

/*! @struct TagNode
 *  @brief Represents a tag buffer node (element node of an XML tree).
 *  @details Represents a tag buffer node (element node of an XML tree)
 *    which carries the tag id (numeric value of the element name), a list of children
 *    and a flag that indicates whether the tag node is closed (i.e. if the closing tag has been read)
 *    or not.
 *  @note Tag buffer nodes (element nodes of an XML tree) are internally represented by numbers!
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
struct TagNode {
    /*! @brief Constructor.
     *  @details Constructor - creating object for a tag (numeric value of the element name).
     *  @param[in] _tag The numeric value of the element name.
     */
    TagNode(TAG _tag);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ TagNode();

    /*! @var TAG tag
     *  @brief The entered numeric value of the element name.
     *  @details The entered numeric value of the element name, which is the
     *    argument of the constructor.
     */
    TAG tag;

    /*! @var bool is_closed
     *  @brief The boolean value if the closing tag has already been read.
     *  @details The boolean value if the closing tag has already been read, i.e.
     *    <tt>true</tt> if the closing tag has been read, <tt>false</tt> otherwise.
     */
    bool is_closed;

    /*! @var BufferNodeList children
     *  @brief The list of children of the tag node.
     *  @details The list of children of the tag node.
     */
    BufferNodeList children;
};

#endif // TAGNODE_H
