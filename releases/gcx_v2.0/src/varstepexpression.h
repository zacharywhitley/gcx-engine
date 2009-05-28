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
 * 	@brief Header file for varstepexpression.cpp.
 * 	@details Header file specifying constructors, destructor and functions for varstepexpression.cpp.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef VARSTEPEXPRESSION_H
#define VARSTEPEXPRESSION_H

#include "expression.h"
#include "varexpression.h"
#include "bufferiterator.h"

/*! @class VarStepExpression
 * 	@brief Represents an XQuery variable with following path (or a path without a preceding variable).
 * 	@details Represents an XQuery variable with following path (or a path without a preceding variable), which takes therefore
 * 			a VarExpression, e.g. <tt>\$x</tt>, <tt>\$z</tt> or <tt>\$root</tt>, and a PathExpression,
 * 			e.g. <tt>/bib//book</tt> or <tt>//node()/text()</tt>, as arguments and returns the sequence or BufferNode respectively
 * 			located through the entered VarExpression and PathExpression.
 *  @note A PathExpression without a preceeding variable is not initialized with <tt>NULL</tt> as
 * 			VarExpression but instead a VarExpression for variable <tt>\$root</tt> is created or entered respectively.
 *  @note ALL PathExpression, which have no preceeding variable, use variable <tt>\$root</tt>!!!
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 *  @license Software License Agreement (BSD License)
 */
class VarStepExpression : public Expression {

public:   
	/*! @brief Constructor.
	 * 	@details Constructor - creating object for a VarExpression and a PathExpression.
	 * 	@param[in] _var Pointer to a VarExpression object.
	 * 	@param[in] _path Pointer to a PathExpression object.
	 */
    VarStepExpression(VarExpression* _var, PathExpression* _path);
    
	/*! @brief Destructor.
	 *  @details Destructor.
	 */
    virtual ~VarStepExpression();
    
	/*! @fn virtual bool scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars)
	 * 	@copydoc Expression::scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars)
	 */
    virtual bool scopeCheck(vector<unsigned>& def_vars,
                                vector<unsigned>& introduced_vars);

	/*! @fn virtual void replaceVarId(unsigned old_id, unsigned new_id)
	 *  @copydoc Expression::replaceVarId(unsigned old_id, unsigned new_id)
	 */
    virtual void replaceVarId(unsigned old_id, unsigned new_id);
    
	/*! @fn virtual void extractDependencies(vector<DependencySet*>* depset)
	 * 	@copydoc Expression::extractDependencies(vector<DependencySet*>* depset)
	 */
	virtual void extractDependencies(vector<DependencySet*>* depset);
        
	/*! @fn virtual void print(ostream& o) const
	 * 	@copydoc Expression::print(ostream& o) const
	 */
	virtual void print(ostream& o) const;

	/*! @fn virtual vector<BufferNode*>* eval(Environment* env, unsigned modus)
	 *  @copydoc Expression::eval(Environment* env, unsigned modus)
	 */
	virtual void* eval(Environment* env, unsigned modus);
	
	/*! @fn inline VarExpression* getVar()
	 *  @brief Returns the VarExpression (variable of this object).
	 *  @details Returns the VarExpression (variable of this object), i.e. returns the member variable.
	 *  @retval VarExpression* Pointer to a VarExpression object.
	 */
    inline VarExpression* getVar() { return var; }
    
	/*! @fn inline PathExpression* getPath()
	 *  @brief Returns the PathExpression (path of this object).
	 *  @details Returns the PathExpression (path of this object), i.e. returns the member variable.
	 *  @retval PathExpression* Pointer to a PathExpression object.
	 */
     inline PathExpression* getPath() { return path; }
     
     // TODO: document this!!!
     void initBit(Environment* env);
     
     // TODO: document this!!!
     void resetBit();

    /*! @fn VarStepExpression* clone()
     *  @brief Returns a new, independent copy of this object.
     *  @details Returns a new, independent copy of this object.
     *  @retval VarStepExpression* Copy of this object.
	 */
    VarStepExpression* clone();

    /*! @fn VarStepExpression* cloneWithoutFinalDosNodeAndAttributes()
     *  @brief Returns a new, independent copy of this object. Additionally,
     *         if the final path step is dos::node(), this one will not
     *         be added to the copy. Also, attributes in the path step
     *         expressions will be ignored
     *  @retval VarStepExpression* The cloned path without final dos::node(). 
	 */
    VarStepExpression* cloneWithoutFinalDosNodeAndAttributes();

private:
	/*! @var VarExpression* var
	 * 	@brief The entered VarExpression.
	 * 	@details The entered VarExpression, which is the first argument of the constructor.
	 */
  VarExpression* var;
    
	/*! @var PathExpression* path
	 * 	@brief The entered PathExpression.
	 * 	@details The entered PathExpression, which is the second argument of the constructor.
	 */
    PathExpression* path;
    
    // TODO: document this!!!
    BufferIterator* bit;
};

#endif // VARSTEPEXPRESSION_H
