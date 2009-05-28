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
 * 	@brief Header file for cmdline.cpp.
 * 	@details Header file specifying constructors, destructor and functions for cmdline.cpp.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
#ifndef CMDLINE_H
#define CMDLINE_H

/*! @struct CmdLine
 * 	@brief Represents all application call parameters.
 * 	@details Represents all application call parameters, which have been passed to save them.
 * 	@author Michael Schmidt
 * 	@author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 * 	@version 1.01b
 * 	@license Software License Agreement (BSD License)
 */
struct CmdLine {
	/*! @brief Constructor.
	 * 	@details Constructor - creating object for all parameters, i.e. all member variables
	 * 			are	set to <tt>true</tt> except <tt>preprocess_stream_only</tt> and
	 * 			<tt>preprocess_stream_only_no_deb</tt>.
	 * 	@param[in] _debug <tt>true</tt> in debug-mode, <tt>false</tt> otherwise.
	 */
    CmdLine(bool _debug);
    
	/*! @brief Constructor.
	 * 	@details Constructor - creating object for only to application passed parameters, i.e. all affected member variables are
	 * 			set to <tt>true</tt> whereas all non-affected member variables are set to <tt>false</tt>.
	 * 	@param[in] _print_file_info <tt>true</tt> if print information on used files (parameter: <tt>--pfileinfo</tt>), <tt>false</tt> otherwise.
	 * 	@param[in] _print_original_query <tt>true</tt> if print original input query (parameter: <tt>--pqueryorig</tt>), <tt>false</tt> otherwise.
	 * 	@param[in] _print_original_varsq <tt>true</tt> if print set of variables (VARSq) of original query (parameter: <tt>--pvarsqorig</tt>), <tt>false</tt> otherwise.
	 * 	@param[in] _print_optimized_query <tt>true</tt> if print optimized query (parameter: <tt>--pqueryopt</tt>), <tt>false</tt> otherwise.
	 * 	@param[in] _print_optimized_varsq <tt>true</tt> if print set of variables (VARSq*) of optimized query (parameter: <tt>--pvarsqopt</tt>), <tt>false</tt> otherwise.
	 * 	@param[in] _print_rewritten_query <tt>true</tt> if print the rewritten query with signOffs (parameter: <tt>--pqueryrew</tt>), <tt>false</tt> otherwise.
	 * 	@param[in] _print_variable_tree <tt>true</tt> if print the constructed variable tree (parameter: <tt>--pvartree</tt>), <tt>false</tt> otherwise.
	 * 	@param[in] _print_original_query_dependencies_wo <tt>true</tt> if print original query dependencies without roles (parameter: <tt>--pquerydepsorig</tt>), <tt>false</tt> otherwise.
	 * 	@param[in] _print_optimized_query_dependencies_wo <tt>true</tt> if print optimized query dependencies without roles (parameter: <tt>--pquerydepswo</tt>), <tt>false</tt> otherwise.
	 * 	@param[in] _print_optimized_query_dependencies_w <tt>true</tt> if print original/optimized query dependencies with roles (parameter: <tt>--pquerydepsw</tt>), <tt>false</tt> otherwise.
	 * 	@param[in] _print_original_projection_tree <tt>true</tt> if print the original constructed projection tree (parameter: <tt>--pprojtreeorig</tt>), <tt>false</tt> otherwise.
	 *  @param[in] _print_passive_projection_tree <tt>true</tt> if print the constructed passive projection tree (parameter: <tt>--ppprojtree</tt>), <tt>false</tt> otherwise.
	 * 	@param[in] _print_optimized_projection_tree <tt>true</tt> if print the optimized projection tree (parameter: <tt>--pprojtreeopt</tt>), <tt>false</tt> otherwise.
	 * 	@param[in] _print_projection_tree_labels <tt>true</tt> if print the projection tree labels (parameter: <tt>--pprojtreelab</tt>), <tt>false</tt> otherwise.
	 * 	@param[in] _print_projection_dfa <tt>true</tt> if print the constructed projection deterministic finite automaton (DFA) (parameter: <tt>--pprojdfa</tt>), <tt>false</tt> otherwise.
	 * 	@param[in] _print_fsamap <tt>true</tt> if print variable to first straight ancestor (FSA) mapping (parameter: <tt>--pfsamap</tt>), <tt>false</tt> otherwise.
	 * 	@param[in] _print_parvarmap <tt>true</tt> if print variables to parent variable (ParVar) mapping (parameter: <tt>--pparvarmap</tt>), <tt>false</tt> otherwise.
	 * 	@param[in] _print_pathenv <tt>true</tt> if print variable to path (Path Environment) mapping (parameter: <tt>--ppathenv</tt>), <tt>false</tt> otherwise.
	 * 	@param[in] _print_initial_buffer <tt>true</tt> if print the initial buffer (parameter: <tt>--pinitbuf</tt>), <tt>false</tt> otherwise.
	 * 	@param[in] _print_final_buffer <tt>true</tt> if print the final buffer (parameter: <tt>--pfinalbuf</tt>), <tt>false</tt> otherwise.
	 * 	@param[in] _print_final_dfa <tt>true</tt> if print the final projection deterministic finite automaton (DFA) (parameter: <tt>--pfinaldfa</tt>), <tt>false</tt> otherwise.
	 * 	@param[in] _print_tag_map <tt>true</tt> if print the final numbers to tagnames mapping (parameter: <tt>--ptagmap</tt>), <tt>false</tt> otherwise.
	 */
    CmdLine(bool _print_file_info, bool _print_original_query,
            bool _print_original_varsq, bool _print_optimized_query,
            bool _print_optimized_varsq, bool _print_rewritten_query,
            bool _print_variable_tree, bool _print_original_query_dependencies_wo, 
            bool _print_optimized_query_dependencies_wo,
			bool _print_optimized_query_dependencies_w,
            bool _print_original_projection_tree,
            bool _print_passive_projection_tree,
			bool _print_optimized_projection_tree,
			bool _print_projection_tree_labels,
			bool _print_projection_dfa,
            bool _print_fsamap, bool _print_parvarmap,
			bool _print_pathenv, bool _print_initial_buffer,
            bool _print_final_buffer, bool _print_final_dfa,
            bool _print_tag_map);
    
    /*! @brief Destructor.
     *  @details Destructor.
	 */
    virtual ~CmdLine();

    /*! @fn inline void setPreprocessStreamMode()
     *  @brief Sets member variable <tt>preprocess_stream_only</tt> to <tt>true</tt>.
     * 	@details Sets member variable <tt>preprocess_stream_only</tt> to <tt>true</tt>.
     * 	@retval void
	 */
    inline void setPreprocessStreamMode() { preprocess_stream_only=true; }
    
    /*! @fn inline void setPreprocessStreamModeNoDeb()
     * 	@brief Sets member variable <tt>preprocess_stream_only_no_deb</tt> to <tt>true</tt>.
     * 	@details Sets member variable <tt>preprocess_stream_only_no_deb</tt> to <tt>true</tt>.
     * 	@retval void
	 */
    inline void setPreprocessStreamModeNoDeb() { preprocess_stream_only_no_deb=true; }
    
    /*! @fn bool someSet()
     * 	@brief Returns <tt>true</tt> if at least one parameter, i.e. at least one member variable, has been set.
     * 	@details Returns <tt>true</tt> if at least one parameter, i.e. at least one member variable, has been set to <tt>true</tt>.
     * 	@retval bool <tt>true</tt> if at least one parameter, i.e. at least one member variable, has been set to <tt>true</tt>, <tt>false</tt> otherwise.
	 */
    bool someSet();

	/*! @var bool print_file_info
	 * 	@brief Parameter <tt>--pfileinfo</tt>.
	 * 	@details Parameter <tt>--pfileinfo</tt> for printing information on used files.
	 */
    bool print_file_info;
    
	/*! @var bool print_original_query
	 * 	@brief Parameter <tt>--pqueryorig</tt>.
	 * 	@details Parameter <tt>--pqueryorig</tt> for printing original input query.
	 */
    bool print_original_query;
    
	/*! @var bool print_original_varsq
	 * 	@brief Parameter <tt>--pvarsqorig</tt>.
	 * 	@details Parameter <tt>--pvarsqorig</tt> for printing set of variables (VARSq) of original (input) query.
	 */
    bool print_original_varsq;
    
	/*! @var bool print_optimized_query
	 * 	@brief Parameter <tt>--pqueryopt</tt>.
	 * 	@details Parameter <tt>--pqueryopt</tt> for printing optimized query.
	 */
    bool print_optimized_query;
    
	/*! @var bool print_optimized_varsq
	 * 	@brief Parameter <tt>--pvarsqopt</tt>.
	 * 	@details Parameter <tt>--pvarsqopt</tt> for printing set of variables (VARSq*) of optimized query.
	 */
    bool print_optimized_varsq;
    
	/*! @var bool print_rewritten_query
	 * 	@brief Parameter <tt>--pqueryrew</tt>.
	 * 	@details Parameter <tt>--pqueryrew</tt> for printing the rewritten query with signOffs.
	 */
    bool print_rewritten_query;
    
	/*! @var bool print_variable_tree
	 * 	@brief Parameter <tt>--pvartree</tt>.
	 * 	@details Parameter <tt>--pvartree</tt> for printing the constructed variable tree.
	 */
    bool print_variable_tree;
    
	/*! @var bool print_original_query_dependencies_wo
	 * 	@brief Parameter <tt>--pquerydepsorig</tt>.
	 * 	@details Parameter <tt>--pquerydepsorig</tt> for printing original query dependencies without roles.
	 */
    bool print_original_query_dependencies_wo;
    
	/*! @var bool print_optimized_query_dependencies_wo
	 * 	@brief Parameter <tt>--pquerydepswo</tt>.
	 * 	@details Parameter <tt>--pquerydepswo</tt> for printing optimized query dependencies without roles.
	 */
    bool print_optimized_query_dependencies_wo;
    
	/*! @var bool print_optimized_query_dependencies_w
	 * 	@brief Parameter <tt>--pquerydepsw</tt>.
	 * 	@details Parameter <tt>--pquerydepsw</tt> for printing original/optimized query dependencies with roles.
	 */
    bool print_optimized_query_dependencies_w;
    
	/*! @var bool print_original_projection_tree
	 * 	@brief Parameter <tt>--pprojtreeorig</tt>.
	 * 	@details Parameter <tt>--pprojtreeorig</tt> for printing the original constructed projection tree.
	 */
    bool print_original_projection_tree;
    
	/*! @var print_passive_projection_tree
	 * 	@brief Parameter <tt>--ppprojtree</tt>.
	 * 	@details Parameter <tt>--ppprojtree</tt> for printing the constructed passive projection tree.
	 */
    bool print_passive_projection_tree;
    
	/*! @var bool print_optimized_projection_tree
	 * 	@brief Parameter <tt>--pprojtreeopt</tt>.
	 * 	@details Parameter <tt>--pprojtreeopt</tt> for printing the optimized projection tree.
	 */
    bool print_optimized_projection_tree;
    
	/*! @var bool print_projection_tree_labels
	 * 	@brief Parameter <tt>--pprojtreelab</tt>.
	 * 	@details Parameter <tt>--pprojtreelab</tt> for printing the projection tree labels.
	 */
	bool print_projection_tree_labels;
	
	/*! @var bool print_projection_dfa
	 * 	@brief Parameter <tt>--pprojdfa</tt>.
	 * 	@details Parameter <tt>--pprojdfa</tt> for printing the constructed projection deterministic finite automaton (DFA).
	 */
    bool print_projection_dfa;
    
	/*! @var bool print_fsamap
	 * 	@brief Parameter <tt>--pfsamap</tt>.
	 * 	@details Parameter <tt>--pfsamap</tt> for printing variable to first straight ancestor (FSA) mapping.
	 */
    bool print_fsamap;
    
	/*! @var bool print_parvarmap
	 * 	@brief Parameter <tt>--pparvarmap</tt>.
	 * 	@details Parameter <tt>--pparvarmap</tt> for printing variables to parent variable (ParVar) mapping.
	 */
    bool print_parvarmap;
    
	/*! @var bool print_pathenv
	 * 	@brief Parameter <tt>--ppathenv</tt>.
	 * 	@details Parameter <tt>--ppathenv</tt> for printing variable to path (Path Environment) mapping.
	 */
    bool print_pathenv;
    
	/*! @var bool print_initial_buffer
	 * 	@brief Parameter <tt>--pinitbuf</tt>.
	 * 	@details Parameter <tt>--pinitbuf</tt> for printing the initial buffer.
	 */
    bool print_initial_buffer;
    
	/*! @var bool print_final_buffer
	 * 	@brief Parameter <tt>--pfinalbuf</tt>.
	 * 	@details Parameter <tt>--pfinalbuf</tt> for printing the final buffer.
	 */
    bool print_final_buffer;
    
	/*! @var bool print_final_dfa
	 * 	@brief Parameter <tt>--pfinaldfa</tt>.
	 * 	@details Parameter <tt>--pfinaldfa</tt> for printing the final projection deterministic finite automaton (DFA).
	 */
    bool print_final_dfa;
    
	/*! @var bool print_tag_map
	 * 	@brief Parameter <tt>--ptagmap</tt>.
	 * 	@details Parameter <tt>--ptagmap</tt> for printing the final numbers to tagnames mapping.
	 */
    bool print_tag_map;
    
	/*! @var bool preprocess_stream_only
	 * 	@brief Parameter <tt>--streamnodeb</tt>.
	 * 	@details Parameter <tt>--streamnodeb</tt> for processing stream only (no debug output).
	 */
    bool preprocess_stream_only;
    
	/*! @var bool preprocess_stream_only_no_deb
	 * 	@brief Parameter <tt>--stream</tt>.
	 * 	@details Parameter <tt>--stream</tt> for processing stream only (with debug output).
	 */
    bool preprocess_stream_only_no_deb;

	/*! @var bool debug 
	 * 	@brief Set to true if debug mode is enabled 
	 */
    bool debug;
};

#endif // CMDLINE_H
