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
 * 	@brief Header file for varname.cpp.
 * 	@details Header file specifying constructors, destructor and functions for varname.cpp.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 *  @license Software License Agreement (BSD License)
 */
#ifndef VARNAME_H
#define VARNAME_H

#include <vector>
#include <sstream>
#include <iostream>
#include "typeenums.h"

//! @namespace std
using namespace std;

/*! @class VarName
 * 	@brief Represents mapping of numeric values of variables to their string value.
 * 	@details Represents mapping of numeric values of variables to their string value, which takes
 * 			therefore a string, e.g. <tt>\$x</tt> or <tt>\$y</tt>, as arguments and returns the string value
 * 			for a (numeric value of a) variable.
 *  @note Variables are internally represented by numbers!
 *  @note SINGLETON PATTERN!
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 *  @license Software License Agreement (BSD License)
 */
class VarName {

public:
    /*! @fn static VarName* getInstance()
     * 	@brief Returns instance of VarName.
     * 	@details Returns instance of VarName, i.e. of this object.
     * 	@retval VarName* Pointer to a VarName object.
     */
    static VarName* getInstance();
    
    /*! @brief Destructor.
     * 	@details Destructor.
     */
    virtual ~VarName();

    /*! @fn unsigned insertVarname(const char* varname, bool force=false)
     * 	@brief Inserts a new string of a variable and returns the numeric value.
     * 	@details Inserts a new string of a variable and returns the numeric value.
     *  @param[in] varname Pointer to a char object (string of a variable).
     *  @param[in] force <tt>true</tt> (default: <tt>false</tt>) if force insertion of variable, <tt>false</tt> otherwise.
     * 	@retval unsigned Numeric value for string value of a variable.
     */
    unsigned insertVarname(const char* varname, bool force=false);
    
    /*! @fn const char* getVarname(unsigned id)
     * 	@brief Returns string value for the entered (numeric value of a) variable.
     * 	@details Returns string value for the entered (numeric value of a) variable.
     *  @param[in] id Numeric value of a variable.
     * 	@retval char* Pointer to a char object.
     */
    const char* getVarname(unsigned id);

    /*! @fn const char* getFreshVarname()
     * 	@brief Returns a new unused string to be used for a variable.
     * 	@details Returns a new unused string to be used for a variable.
     * 	@retval char* Pointer to a char object.
     */
    const char* getFreshVarname();

    /*! @fn void print()
     *  @brief Prints mapping of (numeric values of) variables to their string values.
     *  @details Prints mapping of (numeric values of) variables to their string values.
     *  @retval void
	 */
    void print();

private:
	/*! @var static VarName* instance
	 *  @brief Instance of VarName.
	 *  @details Instance of VarName to avoid duplicate existence of this object.
	 */
    static VarName* instance;
    
	/*! @brief Constructor.
	 * 	@details Constructor - creating object.
	 */
    VarName();
    
	/*! @var vector<const char*> varnames
	 * 	@brief The string values of all used variables.
	 * 	@details The string values of all used variables.
	 */
    vector<const char*> varnames;
    
	/*! @var int index
	 * 	@brief The highest numeric value of the (last) fresh variable.
	 * 	@details The highest numeric value of the (last) fresh variable,
	 * 			which has been got from method getFreshVarname().
	 */
    int index;
};

#endif // VARNAME_H
