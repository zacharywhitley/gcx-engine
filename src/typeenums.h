
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
 *  @brief (TYPE) ENUMS.
 *  @details (TYPE) ENUMS.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef TYPEENUMS_H
#define TYPEENUMS_H

/*! @enum ROLE_TYPE
 *  @brief Role type definition.
 *  @details Role type definition, which includes roles for the <tt>root node</tt> of the XML tree,
 *   	<tt>variables of for-loops</tt>, <tt>condtional expressions</tt> and <tt>output expressions</tt>.
 */
typedef enum { rt_root, rt_variable, rt_condition, rt_output } ROLE_TYPE;

/*! @enum AXIS_TYPE
 *  @brief Axis type definition.
 *  @details Axis type definition, which includes axis for <tt>child</tt>, <tt>descendant</tt> and
 *  		<tt>descendant-or-self</tt> (only for internal use).
 */
typedef enum { at_child, at_descendant, at_dos } AXIS_TYPE;

/*! @enum NODETEST_TYPE
 *  @brief Node test type definition.
 *  @details Node test type definition, which includes node tests for <tt>tag</tt> (e.g. <tt>/bib</tt>), <tt>wildcard</tt> (<tt>/</tt><tt>*</tt>),
 *  		<tt>node</tt> (<tt>node</tt><tt>()</tt>) and <tt>text</tt> (<tt>text</tt><tt>()</tt>).
 */
typedef enum { ntt_tag, ntt_star, ntt_node, ntt_text } NODETEST_TYPE;

/*! @enum ATTRIBUTE_TYPE
 *  @brief Attribute type definition.
 *  @details Attribute type definition, which includes attributes for position (<tt>position</tt><tt>()=n</tt>) (only for internal use).
 */
typedef enum { at_position } ATTRIBUTE_TYPE;

/*! @enum COMP_TYPE
 *  @brief Relational comparison operator definition.
 *  @details Relational comparison operator definition, which includes the standard set of relational comparison
 *  		(<tt>\<</tt>, <tt>\<=</tt>, <tt>\>=</tt>, <tt>\></tt>, <tt>=</tt> and <tt>!=</tt>).
 */
typedef enum { ct_lt, ct_leq, ct_eq, ct_gt, ct_geq, ct_neq } COMP_TYPE;

/*! @enum EXP_TYPE
 *  @brief Expression type definition.
 *  @details Expression type definition, which includes all supported expressions.
 */
typedef enum { ect_and, ect_or, ect_not, ect_exists, ect_empty,
    ect_varstep, ect_operand, ect_true, ect_false,
    et_empty, et_stringconst, et_numericconst, et_for,
    et_where, et_if, et_doc, et_comment, et_nodeconstr,
    et_path, et_pathsteptag, et_pathstepstar, et_pathstepnode,
    et_pathsteptext, et_sequence, et_var, et_varstep, et_signoff,
    et_operandvar, et_operandvarstep, et_operandaggregatefunct,
    et_operandrounding, et_operandconst, et_aggregatefunctsum,
    et_aggregatefunctavg, et_aggregatefunctmin, et_aggregatefunctmax,
    et_aggregatefunctcount, et_aggregatefunctstddevsamp,
    et_aggregatefunctstddevpop, et_aggregatefunctvarsamp,
    et_aggregatefunctvarpop, et_aggregatefunctmedian, et_abs,
    et_ceiling, et_cover, et_floor, et_round, et_roundhalftoeven,
    et_truncate
} EXP_TYPE;

/*! @enum ISTREAM_TYPE
 *  @brief Input stream type definition.
 *  @details Input stream type definition, which includes input streams for <tt>files</tt>, <tt>null</tt> (no input/output),
 *  		<tt>sockets</tt> and <tt>standard input</tt>.
 */
typedef enum { it_file, it_null, it_socket, it_stdin } ISTREAM_TYPE;

/*! @enum OSTREAM_TYPE
 *  @brief Output stream type definition.
 *  @details Output stream type definition, which includes output streams for <tt>files</tt>, <tt>null</tt> (no input/output),
 *  		<tt>sockets</tt> and <tt>standard output</tt>.
 */
typedef enum { ot_file, ot_null, ot_socket, ot_stdout } OSTREAM_TYPE;

/*! @enum EXCEPTION_TYPE
 *  @brief Exception type definition.
 *  @details Exception type definition, which includes exceptions for <tt>command-line argument</tt> errors, <tt>input stream</tt> errors,
 *  		<tt>parsing</tt> errors, <tt>runtime</tt> errors, <tt>casting</tt> errors and (not) <tt>implementing</tt> errors.
 */
typedef enum { exct_argument, exct_iostream, exct_parse, exct_runtime,
    exct_cast, exct_impl
} EXCEPTION_TYPE;

/*! @enum EXCEPTION_LEVEL
 *  @brief Exception level definition.
 *  @details Exception level definition, which includes exception levels <tt>warning</tt>, <tt>error</tt> and <tt>fatal</tt>.
 */
typedef enum { excl_warning, excl_error, excl_fatal } EXCEPTION_LEVEL;

/*! @enum ERROR_ID
 *  @brief Error identifier definition.
 *  @details Error identifier definition specifying EXCEPTION_TYPE more precisely.
 */
typedef enum { eid_notset = 0,  // base id
    eid_argument = 100,         // base id
    eid_stream = 200,           // base id
    eid_stream_input = 201,
    eid_stream_output = 202,
    eid_parse = 300,            // base id
    eid_parse_query = 301,
    eid_parse_xml = 302,
    eid_runtime = 400,          // base id
    eid_runtime_illegalmode = 401,
    eid_runtime_bit = 402,
    eid_runtime_tokenconfig = 403,
    eid_runtime_ptlabel = 404,
    eid_cast = 500,             // base id
    eid_cast_stringnumeric = 501,
    eid_cast_numericstring = 502,
    eid_cast_summation = 503,
    eid_cast_power = 504,
    eid_cast_radical = 505,
    eid_cast_division = 506,
    eid_impl = 600              // base id
} ERROR_ID;

/*! @enum VALUE_TYPE
 *  @brief Value type definition.
 *  @details Value type definition, which includes value types <tt>numeric</tt> (xsd:numeric), <tt>string</tt> (xsd:string) and <tt>unknown</tt> (xsd:unknown).
 */
typedef enum { xsd_numeric, xsd_string, xsd_unknown } VALUE_TYPE;

/*! @def ROOTVAR
 *  @brief Predefined variable for internal use of variable <tt>\$.root</tt>.
 *  @details Predefined variable for internal use of variable <tt>\$.root</tt> (which denotes abolute paths).
 *  		This variable cannot be used/specified in the input query and is always implicitly bound to the (virtual) XML document root. 
 */
#define ROOTVAR ".root"

/*! @def TAGID_ROOT
 *  @brief The ID of variable <tt>\$root</tt>.
 *  @details The ID of variable <tt>\$root</tt>, which is,
 *    because of their first insertion into VarName and TagMap, always <tt>zero</tt>.
 *    This ID is only defined to highlight the special variable <tt>\$root</tt>.
 *    Changing this define value would lead to incorrect results (best case) or
 *    application crash (worst case). Therefore DO NOT CHANGE THIS VALUE!
 */
#define TAGID_ROOT 0

#endif // TYPEENUMS_H
