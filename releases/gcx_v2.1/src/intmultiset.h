
/*
 | Author: Michael Schmidt
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
 *  @brief Header file for intmultiset.cpp.
 *  @details Header file specifying constructors, destructor and functions for intmultiset.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef INTMULTISET_H
#define INTMULTISET_H

#include <vector>
#include "rolelist.h"

//! @namespace std
using namespace std;

/*! @class IntMultiSet 
 *  @brief Represents a multi set of integers, ranging from 0 to a fixed maximum
 *  @details The class provides an efficient implementation of common
 *    operations like adding, removing, or element access.
 *  @note This class is only used when compiling GCX without option
 *    -DROLE_REFCOUNT. 
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class IntMultiSet {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for integers within the range 0..max-1.
     */
    IntMultiSet();

    /*! @brief Constructor.
     *  @details Constructor - creating object for integers within
     *    the range 0..max-1, and inserts the elements specified in
     *    the vector argument into the multi set.
     *  @param[in] _elems Pointer to the vector of integer elements used
     *    for initialization.
     *  @note All elements in the input vector  _elems
     *    Must be contained inbetween range 0-max-1.
     */
    IntMultiSet(vector < unsigned >*_elems);

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ IntMultiSet();

    /*! @fn void addElems(IntMultiSet& set)
     *  @brief Appends the content of the input IntMultiSet object.
     *  @details Appends the content of the input IntMultiSet object
     *    to the current multi set object.
     *  @note the max value of the input IntMultiSet object must be less or equal
     *    than this object's max value.
     *  @param[in] set The IntMultiSet object whose elements will be added.
     *  @retval void
     */
    void addElems(IntMultiSet & set);

    /*! @fn void addElems(vector<unsigned>* _elems)
     *  @brief Add the elements specified in the input vector to the multi set.
     *  @details Appends the integer values specified in the passed vector
     *    to the current multi set object.
     *  @note The elements in the input vector must be contained inbetween range 0..max-1.
     *  @param[in] _elems Pointer to the element vector whose elements
     *  will be added. 
     *  @retval void
     */
    void addElems(vector < unsigned >*_elems);

    /*! @fn inline void addElem(unsigned i)
     *  @brief Adds a single element to the multi set.
     *  @details Adds a single elementto the current multi set object.
     *  @note The value i must be less than max.
     *  @param[in] i Value to be added.
     *  @retval void
     */
    inline void addElem(unsigned i) {
        elems[i]++;
    }
    /*! @fn virtual void print(OutputStream& dos)
     *  @brief Debug prints the int multi set.
     *  @details Debug prints the int multi set to the specified debug output stream.
     *  @param[in] dos The debug output stream to be used.
     *  @retval void
     */ virtual void print(OutputStream & dos);

    /*! @fn vector<unsigned> getElemVec()
     *  @brief Returns a vector containing all elements in the multi set. 
     *  @details The returned list is ordered (<) and may contain elements
     *    several time (according to the multi set), i.e. duplicates are not removed..
     *  @retval vector<unsigned>  The vector containing all multi set elements.
     */
    vector < unsigned >getElemVec();

    /*! @fn bool isEmpty()
     *  @brief Checks for the multiset being empty.
     *  @details Checks for the multiset being empty.
     *  @retval bool Returns true if the multiset is empty.
     */
    bool isEmpty();

    /*! @fn inline bool containsElem(unsigned i)
     *  @brief Checks for an element being contained in the multi set.
     *  @details Implements an efficient element check for the int multi set.
     *  @note The value i must be less than max.
     *  @param[in] i The value to be checked for.
     *  @retval bool Returns true if the element is contained, false otherwise.
     */
    inline bool containsElem(unsigned i) {
        return elems[i];
    }

    /*! @fn bool removeElem(unsigned i)
     *  @brief Removes a single element from the multi set.
     *  @details Removes a single element from the multi set, if it is present
     *    at least once.
     *  @note The value of i must be less than max.
     *    If the value is not contained in the multi set, nothing happens.
     *  @param[in] i Value to be removed. 
     *  @retval bool Returns false if element i was not contained in the multi set.
     */
    bool removeElem(unsigned i);

  private:
    /*! @var int max
     *  @brief The maximum number of distinct elements in the set.
     *  @details Furthermore, all elements must be in the range 0..max-1.
     */
    int max;

    /*! @var unsigned* elems
     *  @brief The internal data structure that stores the multi set.
     *  @details Each element i in 0..max-1 is represented by array
     *    position i, whereas the value in the array denotes
     *  the element's multiplicity in the multi set.
     */
    unsigned *elems;
};

#endif // INTMULTISET_H
