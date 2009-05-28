
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
 *  @brief Header file for stringhash.cpp.
 *  @details Header file specifying constructors, destructor and functions for stringhash.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef STRINGHASH_H
#define STRINGHASH_H

#include <vector>

/*! @def HASH_BASE
 *  @brief The hash base, defining the number of buckets that are available.
 *  @details To reach best results, this should be a prime number.
 */
#define HASH_BASE 31

//! @namespace std
using namespace std;

/*! @struct TagInfo
 *  @brief Represents a (Tag,ID)-pair
 *  @details Stores, for a fixed tag name (as string) its associated id.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
struct TagInfo {
    /*! @brief Constructor.
     *  @details Constructor - creating object.
     *  @param[in] _tag The tag component of the tuple.
     *  @param[in] _index The ID component of the tuple.
     */
    TagInfo(const char *_tag, unsigned _index):tag(_tag),
     index(_index) {
    }
    /*! @brief Destructor.
     *  @details Destructor.
     */ ~TagInfo() {
        delete[]tag;
    }

    /*! @var const char* tag
     *  @brief Stores the tag component of the tuple.
     *  @details Pointer to the text representation of the tag.
     */
    const char *tag;

    /*! @var unsigned index 
     *  @brief The ID component of the tuple.
     *  @details The ID component of the tuple, i.e. the integer ID
     *    of the associated tag.
     */
    unsigned index;
};

/*! @class StringHash
 *  @brief Represents a string hash function 
 *  @details Implements a simple yet efficient string hash function.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class StringHash {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object.
     */
    StringHash();

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ StringHash();

    /*! @fn unsigned hash(const char* s)
     *  @brief Hashes a string value.
     *  @details Hashes a complete string value, i.e. computes the hash value of the string.
     *  @param[in] s The string to be hashed.
     *  @retval unsigned The hash value.
     */
    unsigned hash(const char *s);

    /*! @fn unsigned hash(const char* s, unsigned start_offset, unsigned end_offset)
     *  @brief Hashes a string value.
     *  @details Hashes a substring of the passed string value.
     *  @param[in] s The string to be hashed.
     *  @param[in] start_offset The start offset position inside the string.
     *  @param[in] end_offset The end offsetposition inside the string.
     *  @retval unsigned The hash value.
     */
    unsigned hash(const char *s, unsigned start_offset, unsigned end_offset);

  private:
    /*! @fn unsigned h(const char* s)
     *  @brief Internal hash function.
     *  @details The hash function sums up the character values and
     *    divides the result through the specified base.
     *  @param[in] s The string to be hashed.
     *  @retval unsigned The hash value.
     */
    unsigned h(const char *s);

    /*! @fn unsigned h(const char* s, unsigned start_offset, unsigned end_offset)
     *  @brief Internal hash function, to hash a substring of the passed string.
     *  @details The hash function sums up the substring character values and
     *    divides the result through the specified base.
     *  @param[in] s The string to be hashed.
     *  @param[in] start_offset The start offset position inside the string.
     *  @param[in] end_offset The end offsetposition inside the string.
     *  @retval unsigned The hash value.
     */
    unsigned h(const char *s, unsigned start_offset, unsigned end_offset);

    /*! @var int base
     *  @brief The hash base.
     *  @details The hash base, initialized with valur HASH_BASE (which should
     *    always be set to a prime number).
     */
    int base;

    /*! @var vector<TagInfo*>* matches
     *  @brief Hashed values.
     *  @details The buckets containing previous matches computed by
     *    the hash function.
     */
     vector < TagInfo * >*matches;

    /*! @var unsigned free_index
     *  @brief The next available free index position.
     *  @details The next available free index position in member matches.
     */
    unsigned free_index;
};

#endif // STRINGHASH_H
