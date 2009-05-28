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
 * 	@brief Header file for prettyprint.cpp.
 * 	@details Header file specifying constructors, destructor and functions for prettyprint.cpp.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef PRETTYPRINT_H
#define PRETTYPRINT_H

#include <iostream>

/*! @def TABSIZE
 *  @brief The number to multipy whitespaces with.
 *  @details The number to multipy whitespaces with.
 */
#define TABSIZE 2

//! @namespace std
using namespace std;

/*! @class PrettyPrint
 * 	@brief Represents nice output formatting.
 * 	@details Represents nice output formatting, which indents text or output respectively by putting
 * 			whitespaces before it.
 *  @note SINGLETON PATTERN!
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
class PrettyPrint {

public:
    /*! @fn static PrettyPrint* getInstance()
     * 	@brief Returns instance of PrettyPrint.
     * 	@details Returns instance of PrettyPrint, i.e. of this object.
     * 	@retval PrettyPrint* Pointer to a PrettyPrint object.
     */
    static PrettyPrint* getInstance();
    
    /*! @brief Destructor.
     * 	@details Destructor.
     */
    virtual ~PrettyPrint();

    /*! @fn void printTabs()
     * 	@brief Prints current number of whitespaces.
     * 	@details Prints current number of whitespaces, which are defined by the member variable.
     * 	@retval void
     */
    void printTabs();
    
    /*! @fn inline void increaseTab()
     * 	@brief Increments number of whitespaces.
     * 	@details Increments number of whitespaces, i.e. increments the member variable.
     * 	@retval void
     */
    inline void increaseTab() { tab++; }
    
    /*! @fn inline void decreaseTab()
     * 	@brief Decrements number of whitespaces.
     * 	@details Decrements number of whitespaces, i.e. decrements the member variable.
     * 	@retval void
     */
    inline void decreaseTab() { tab--; }

private:
	/*! @var static PrettyPrint* instance
	 *  @brief Instance of PrettyPrint.
	 *  @details Instance of PrettyPrint to avoid duplicate existence of this object.
	 */
    static PrettyPrint* instance;
    
	/*! @brief Constructor.
	 * 	@details Constructor - creating object.
	 */
    PrettyPrint();

	/*! @var int tab
	 *  @brief Number of whitespaces.
	 *  @details Number of whitespaces to put before text or output respectively.
	 */
    int tab;
};

#endif // PRETTYPRINT_H
