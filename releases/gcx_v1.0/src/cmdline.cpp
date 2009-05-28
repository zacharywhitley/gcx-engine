/*
 | Author: Michael Schmidt
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
 |  following disclaimer.
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
#include "cmdline.h"

CmdLine::CmdLine(bool debug)
    : print_file_info(debug),
        print_original_query(debug),
        print_optimized_query(debug),
        print_rewritten_query(debug),
        print_projection_tree(debug),
		print_projection_dfa(debug),
		print_projection_nfa(debug),
		print_fsamap(debug),
		print_pathenv(debug),
        print_initial_buffer(debug),
        print_final_buffer(debug),
        print_final_dfa(debug),
        print_tag_map(debug),
		print_stats(debug),
        preprocess_stream_only(false), 
        preprocess_stream_only_no_deb(false) {}

CmdLine::CmdLine(bool _print_file_info,
                    bool _print_original_query,
                    bool _print_optimized_query,
                    bool _print_rewritten_query,
                    bool _print_projection_tree,
					bool _print_projection_dfa,
					bool _print_projection_nfa,
					bool _print_fsamap,
					bool _print_pathenv,
                    bool _print_initial_buffer,
                    bool _print_final_buffer,
                    bool _print_final_dfa,
                    bool _print_tag_map,
                    bool _print_stats)
    : print_file_info(_print_file_info),
        print_original_query(_print_original_query),
        print_optimized_query(_print_optimized_query),
        print_rewritten_query(_print_rewritten_query),
        print_projection_tree(_print_projection_tree),
		print_projection_dfa(_print_projection_dfa),
		print_projection_nfa(_print_projection_nfa),
		print_fsamap(_print_fsamap),
		print_pathenv(_print_pathenv),
        print_initial_buffer(_print_initial_buffer),
        print_final_buffer(_print_final_buffer),
        print_final_dfa(_print_final_dfa),
        print_tag_map(_print_tag_map),
		print_stats(_print_stats),
        preprocess_stream_only(false), 
        preprocess_stream_only_no_deb(false) {}

CmdLine::~CmdLine() {}

bool CmdLine::someSet() {
    return print_file_info ||
        print_original_query ||
        print_optimized_query ||
        print_rewritten_query ||
        print_projection_tree ||
		print_projection_dfa ||
		print_projection_nfa ||
		print_fsamap ||
		print_pathenv ||
        print_initial_buffer ||
        print_final_buffer ||
        print_final_dfa ||
        print_tag_map ||
		print_stats;
}
