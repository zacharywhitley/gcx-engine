
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
 *  @brief Header file for tagmap.cpp.
 *  @details Header file specifying constructors, destructor and functions for tagmap.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef TAGMAP_H
#define TAGMAP_H

#include "stringhash.h"

/*! @typedef unsigned TAG
 *  @brief Definition of tagnames to be numeric.
 *  @details Definition of tagnames to be numeric.
 */
typedef unsigned TAG;

// forward declarations
class OutputStream;

/*! @class TagMap
 *  @brief Represents mapping of numeric values of tagnames to their string value.
 *  @details Represents mapping of numeric values of tagnames to their string value, which takes
 *    therefore a string, e.g. <tt>\<tagname\>\</tagname\></tt> or <tt>\<tagname/\></tt>, as arguments
 *    and returns the string value for a (numeric value of a) tagname.
 *  @note Tagnames are internally represented by numbers!
 *  @note SINGLETON PATTERN!
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class TagMap {

  public:
    /*! @fn static TagMap* getInstance()
     *  @brief Returns instance of TagMap.
     *  @details Returns instance of TagMap, i.e. of this object.
     *  @retval TagMap* Pointer to a TagMap object.
     */
    static TagMap *getInstance();

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ TagMap();

    /*! @fn unsigned insertTag(const char* tag)
     *  @brief Inserts a new string of a tagname and returns the numeric value.
     *  @details Inserts a new string of a tagname and returns the numeric value.
     *  @param[in] tag Pointer to a char object (string of a tagname).
     *  @retval unsigned Numeric value for string value of a tagname.
     */
    unsigned insertTag(const char *tag);

    /*! @fn unsigned insertTagWithOffset(const char* tag, unsigned start_offset, unsigned end_offset)
     *  @brief Inserts a new string of a tagname and returns the numeric value.
     *  @details Inserts a new string of a tagname and returns the numeric value.
     *    Thereby the string is copied starting from position <tt>start</tt> up to position <tt>end</tt>.
     *  @param[in] tag Pointer to a char object (string of a tagname).
     *  @param[in] start_offset The start offset (from the left).
     *  @param[in] end_offset The end offset (from the right).
     *  @retval unsigned Numeric value for string value of a tagname.
     */
    unsigned insertTagWithOffset(const char *tag, unsigned start_offset,
                                 unsigned end_offset);

    /*! @fn void print(OutputStream& dos)
     *  @brief Prints mapping of (numeric values of) tagnames to their string values.
     *  @details Prints mapping of (numeric values of) tagnames to their string values.
     *  @param[in] dos Pointer to a OutputStream object.
     *  @retval void
     */
    void print(OutputStream & dos);

    /*! @fn inline const char* getTag(unsigned id)
     *  @brief Returns string value for the entered (numeric value of a) tagname.
     *  @details Returns string value for the entered (numeric value of a) tagname.
     *  @param[in] id Numeric value of a tagname.
     *  @retval char* Pointer to a char object.
     */
    inline const char *getTag(unsigned id) {
        return tags[id];
  } private:
    /*! @var static TagMap* instance
     *  @brief Instance of TagMap.
     *  @details Instance of TagMap to avoid duplicate existence of this object.
     */
    static TagMap *instance;

    /*! @brief Constructor.
     *  @details Constructor - creating object.
     */
    TagMap();

    /*! @var vector<const char*> tags
     *  @brief The string values of all used tagnames.
     *  @details The string values of all used tagnames.
     */
    vector < const char *>tags;

    /*! @var StringHash sh
     *  @brief StringHash object to hash string values to avoid duplicate insertion.
     *  @details StringHash object to hash string values to avoid duplicate insertion.
     */
    StringHash sh;
};

#endif // TAGMAP_H
