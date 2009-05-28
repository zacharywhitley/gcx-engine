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
 * 	@brief (TYPE) ENUMS.
 *  @details (TYPE) ENUMS.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef TYPEENUMS_H
#define TYPEENUMS_H

/*! @enum ROLE_TYPE
 *  @brief Role type definition.
 * 	@details There exists role types for variable roles, condition roles,
 * 	         output roles, and the root role.
 */
typedef enum {rt_root, rt_variable, rt_condition, rt_output} ROLE_TYPE;

/*! @enum AXIS_TYPE
 *  @brief Axis type definition.
 * 	@details The implementation currently support child and descendant axes and, internally
 * 	         descendant-or-self axes.
 */
typedef enum {at_child, at_descendant, at_dos} AXIS_TYPE;

/*! @enum NODETEST_TYPE
 *  @brief Node test type definition.
 * 	@details The implementation supports tag node tests, star nodetests,
 * 	         node node tests (node()), and text node tests (text()).
 */
typedef enum {ntt_tag, ntt_star, ntt_node, ntt_text} NODETEST_TYPE;

/*! @enum ATTRIBUTE_TYPE
 *  @brief Attribute type definition.
 * 	@details The implementation currently supports the position()=<nr> attribute
 * 	         (used internally).
 */
typedef enum {at_position} ATTRIBUTE_TYPE;

/*! @enum COMP_TYPE
 *  @brief Relational comparison operator definition.
 * 	@details The implementation supports the standard set of relational comparison
 * 	         operators <, <=, =, !=, >, and >=.
 */
typedef enum {ct_lt, ct_leq, ct_eq, ct_gt, ct_geq, ct_neq} COMP_TYPE;

/*! @enum EXP_TYPE
 *  @brief Expression type definitions.
 * 	@details The implementation supports different types of expressions, such as
 * 	         for-, where-, sequence-, aggregation-, variable-, varstep-, ...,
 *           and different condition-expressions.
 */
typedef enum {ect_and, ect_or, ect_not, ect_exists,
              ect_operand, ect_true, ect_false,
              et_empty, et_const, et_for, et_if,
              et_nodeconstr, et_path, et_pathsteptag,
              et_pathstepstar, et_pathstepnode, et_pathsteptext,
              et_sequence, et_var, et_varstep, et_signoff,
              et_operandvar, et_operandvarstep, et_operandaggregatefunct,
              et_operandconst, et_aggregatefunctsum, et_aggregatefunctavg,
              et_aggregatefunctmin, et_aggregatefunctmax, et_aggregatefunctcount,
              et_aggregatefunctstddevsamp, et_aggregatefunctstddevpop,
              et_aggregatefunctvarsamp, et_aggregatefunctvarpop,
              et_aggregatefunctlist} EXP_TYPE;             

/*! @def ROOTVAR
 *  @brief Name of the root variable.
 *  @details The root variable is used internally, i.e. can not be
 *  specified in input queries. It is always implicitly bound to the
 *  (virtual) XML document root.
 */
#define ROOTVAR ".root"
              
/*! @def TAGID_ROOT
 *  @brief The ID of variable <tt>\$root</tt>.
 *  @details The ID of variable <tt>\$root</tt>, which is,
 * 			because of their first insertion into VarName and TagMap, always <tt>zero</tt>.
 * 			This ID is only defined to highlight the special variable <tt>\$root</tt>.
 * 			Changing this define value would lead to incorrect results (best case) or
 * 			application crash (worst case). Therefore DO NOT CHANGE THIS VALUE!
 */
#define TAGID_ROOT 0        

#endif // TYPEENUMS_H
