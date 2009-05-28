/*
 | Author: Michael Schmidt
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
 * 	@brief Header file for intmultiset.cpp.
 * 	@details Header file specifying constructors, destructor and functions for intmultiset.cpp.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef INTMULTISET_H
#define INTMULTISET_H

#include <iostream>
#include <vector>
#include "rolelist.h"

//! @namespace std
using namespace std;

/*! @class IntMultiSet 
 * 	@brief Represents a multi set of integers, ranging from 0 to a fixed maximum
 * 	@details The class provides an efficient implementation of common
 * 	         operations like adding, removing, or element access. 
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
class IntMultiSet {

public:
	/*! @brief Constructor.
     *  @details Constructor - creates an IntMultiSet for integers within
     *           the range 0..max-1.
     */
    IntMultiSet();

	/*! @brief Constructor.
     *  @details Constructor - creates an IntMultiSet for integers within
     *           the range 0..max-1, and inserts the elements specified in
     *           the vector argument into the multi set. 
     *           ATTENTION: all elements in the input vector  _elems
     *           MUST be contained inbetween range 0-max-1.
     */
    IntMultiSet(vector<unsigned>* _elems);

	/*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~IntMultiSet();

	/*! @brief Appends the content of the input IntMultiSet object.
     *  @details ATTENTION: the max value of the input IntMultiSet object
     *           must be less or equal than this object's max value.
     *  @param[in] set The IntMultiSet object whose elements will be added.
     *  @retval void
     */
    void addElems(IntMultiSet& set);
    
	/*! @brief Add the elements specified in the input vector to the multi set.
     *  @details ATTENTION: the elements in the input vector must be contained
     *           inbetween range 0..max-1.
     *  @param[in] _elems Pointer to the element vector whose elements
     *                    will be added. 
     *  @retval void
     */
    void addElems(vector<unsigned>* _elems);

	/*! @brief Adds a single element to the multi set.
     *  @details ATTENTION: the value i must be less than max.
     *  @param[in] i Value to be added.
     *  @retval void
     */
    inline void addElem(unsigned i) { elems[i]++; }

	/*! @brief Removes a single element from the multi set.
     *  @details ATTENTION: the value i must be less than max.
     *           If the value is not contained in the multi set,
     *           nothing happens.
     *  @param[in] i Value to be removed. 
     *  @retval bool Returns false if element i was not contained 
     *               in the multi set.
     */
    bool removeElem(unsigned i);
    
	/*! @brief Returns a vector containing all elements in the multi set. 
     *  @details The returned list is ordered (<) and may contain elements
     *          several time (according to the multi set), i.e. duplicates
     *          are not removed..
     * @retval vector<unsigned>  The vector containing all multi set elements.
     */
    vector<unsigned> getElemVec();
    
	/*!
     *  @brief Checks for the multiset being empty
     *  @details:
     *  @retval bool Returns true if the multiset is empty.
     */
	bool isEmpty();
    

	/*! @brief Checks for an element being contained in the multi set.
     *  @details ATTENTION: the value i must be less than max.
     *  @param[in] i The value to be checked for.
     *  @retval bool Returns true if the element is contained, false otherwise.
     */
    inline bool containsElem(unsigned i) { return elems[i]; }

	/*! @brief Debug prints the int multi set to STDOUT.
     *  @details
     *  @retval void
     */
    virtual void print();

private:
	/*! @var int max
     *  @brief The maximum number of distinct elements in the set.
     *  @details Furthermore, all elements must be in the range 0..max-1.
     */
	int max;

	/*! @var unsigned* elems
     *  @brief The internal data structure that stores the multi set.
     *  @details Each element i in 0..max-1 is represented by array
     *           position i, whereas the value in the array denotes
     *           the element's multiplicity in the multi set.
     */
    unsigned* elems;
};

#endif // INTMULTISET_H
