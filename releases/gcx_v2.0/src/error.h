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
 * 	@brief Header file for error.cpp.
 * 	@details Header file specifying constructors, destructor and functions for error.cpp.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef ERROR_H
#define ERROR_H

#include <iostream>

//! @namespace std
using namespace std;

/*! @class Error
 * 	@brief Represents output of an error message and erroneous termination of application.
 * 	@details Represents output of an error message and erroneous termination of application.
 *  @note SINGLETON PATTERN!
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 *  @license Software License Agreement (BSD License)
 */
class Error {

public:
    /*! @fn static Error* getInstance()
     * 	@brief Returns instance of Error.
     * 	@details Returns instance of Error, i.e. returns instance of this object.
     * 	@retval Error* Pointer to an Error object.
     */
    static Error* getInstance();
    
    /*! @brief Destructor.
     * 	@details Destructor.
     */
    virtual ~Error();

    /*! @fn void throwError(const char* text, int error_code=EXIT_FAILURE)
     * 	@brief Outputs error message and terminates application with error code.
     * 	@details Outputs error message and terminates application with error code.
     *  @param[in] text The text or error message respectively to output.
     * 	@param[in] error_code The error code of application <tt>(default: EXIT_FAILURE
	 * 	           or 1 respectively)</tt> when terminating.
     * 	@retval void
     */
    void throwError(const char* text, int error_code=EXIT_FAILURE);

private:
	/*! @var static Error* instance
	 *  @brief Instance of Error.
	 *  @details Instance of Error to avoid duplicate existence of this object.
	 */
    static Error* instance;
    
	/*! @brief Constructor.
	 * 	@details Constructor - creating object.
	 */
    Error();
};

#endif // ERROR_H
