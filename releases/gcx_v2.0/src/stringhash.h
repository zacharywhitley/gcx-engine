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
 * 	@brief Header file for stringhash.cpp.
 * 	@details Header file specifying constructors, destructor and functions for stringhash.cpp.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef STRINGHASH_H
#define STRINGHASH_H

#include <iostream>
#include <vector>

/*! @def HASH_BASE
 *  @base The hash base, defining the number of buckets that are available.
 *  @details To reach best results, this should be a prime number.
 */
#define HASH_BASE 31

//! @namespace std
using namespace std;

/*! @struct TagInfo
 * 	@brief Represents a (Tag,ID)-pair
 *  @details
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
struct TagInfo {
	/*! @brief Constructor.
     *  @param[in] _tag The tag component of the tuple.
     *  @param[in] _index The ID component of the tuple.
     */
    TagInfo(const char* _tag, unsigned _index) : tag(_tag), index(_index) {}

	/*! @brief Destructor.
     */
	~TagInfo() { delete[] tag; }

	/*! @var const char* tag
     *  @brief Stores the tag component of the tuple.
     */
    const char* tag;

	/*! @var unsigned index 
     *  @brief Stores the ID component of the tuple.
     */
    unsigned index;
};

/*! @class StringHash
 * 	@brief Represents a string hash function 
 * 	@details
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 *  @license Software License Agreement (BSD License)
 */
class StringHash {

public:
	/*! @brief Constructor.
     *  @details Creates an empty StringHash object.
     */
    StringHash();

	/*! @brief Destructor.
     *  @details
     */
    virtual ~StringHash();

	/*! @brief Hashes a string value.
     * @param[in] s The string to be hashed.
     * @retval unsigned The hash value.
     */
    unsigned hash(const char* s);

	/*! @brief Hashes a string value.
     *  @param[in] s The string to be hashed.
     *  @param[in] start_offset The start offset position inside the string.
     *  @param[in] end_offset The end offsetposition inside the string.
     *  @retval unsigned The hash value.
     */
    unsigned hash(const char* s, unsigned start_offset, unsigned end_offset);

private:
	/*! @brief Internal hash function.
     *  @param[in] s The string to be hashed.
     *  @retval unsigned The hash value.
     */
	unsigned h(const char* s);
	
	/*! @brief Internal hash function.
     *  @param[in] s The string to be hashed.
     *  @param[in] start_offset The start offset position inside the string.
     *  @param[in] end_offset The end offsetposition inside the string.
     *  @retval unsigned The hash value.
     */
	unsigned h(const char* s, unsigned start_offset, unsigned end_offset);
	
	/*! @var int base
     *  @brief The hash base.
     */
    int base;

	/*! @var vector<TagInfo>* matches
     *  @brief Hashed values.
     */
    vector<TagInfo*>* matches;

	/*! @var unsigned free_index
     *  @brief The next available free index position.
     */
    unsigned free_index;
};

#endif // STRINGHASH_H
