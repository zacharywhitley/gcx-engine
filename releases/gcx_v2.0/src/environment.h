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
 * 	@brief Header file for environment.cpp.
 * 	@details Header file specifying constructors, destructor and functions for environment.cpp.
 * 	@author Michael Schmidt
 * 	@author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <map>
#include "typeenums.h"
#include "buffernode.h"
#include "varname.h"

/*! @typedef map<unsigned, BufferNode*> Env
 * 	@brief The underlying map of this object.
 * 	@details The underlying map of this object, which maps variables to a BufferNode.
 */
typedef map<unsigned, BufferNode*> Env;

/*! @class Environment
 * 	@brief Represents (current) binding of variables to a buffer node.
 * 	@details Represents (current) binding of variables to a buffer node, which takes therefore
 * 			a variable and a BufferNode as arguments.
 *  @note Variables are internally represented by numbers!
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
class Environment {

public:
	/*! @brief Constructor.
	 * 	@details Constructor - creating object.
	 */
    Environment();
    
	/*! @brief Constructor.
	 * 	@details Constructor - creating object for a BufferNode.
	 * 	@param[in] _root Pointer to a BufferNode object.
	 */
    Environment(BufferNode* _root);
    
	/*! @brief Destructor.
	 * 	@details Destructor.
	 */
    virtual ~Environment();
    
    /*! @fn void insertNodeBinding(unsigned var, BufferNode* node)
     *  @brief Inserts (or sets) a new variable with its (current) binding to a BufferNode.
     *  @details Inserts (or sets) a new variable with its (current) binding to a BufferNode.
     *  @param[in] var Variable for which (current) binding holds.
     *  @param[in] node Pointer to a BufferNode object.
     *  @retval void
	 */
    void insertNodeBinding(unsigned var, BufferNode* node);
    
    /*! @fn BufferNode* getNodeBinding(unsigned var)
     *  @brief Returns the (current) binding to a BufferNode for the entered variable.
     *  @details Returns the (current) binding to a BufferNode for the entered variable.
     *  @param[in] var Variable for which (current) binding should be obtained. 
     *  @retval BufferNode* Pointer to a BufferNode object.
	 */
    BufferNode* getNodeBinding(unsigned var);
    
    /*! @fn void print()
     *  @brief Prints all variables with their (current) binding to a BufferNode.
     *  @details Prints all variables with their (current) binding to a BufferNode.
     *  @retval void
	 */
    void print();

    /*! @fn Environment* clone()
     *  @brief Returns a new, independent copy of all variables with their (current) binding to a BufferNode
     * 			or of this object respectively.
     *  @details Returns a new, independent copy of all variables with their (current) binding to a BufferNode
     * 			or of this object respectively.
     *  @retval Environment* Copy of all variables with their (current) binding to a BufferNode
     * 			or of this object respectively.
	 */
    Environment* clone();

private:
	/*! @var Env env
	 * 	@brief The underlying map of this object.
	 * 	@details The underlying map of this object, which maps variables to a BufferNode.
	 */
    Env env;
};

#endif // ENVIRONMENT_H
