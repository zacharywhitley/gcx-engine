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
#ifndef CMDLINE_H
#define CMDLINE_H

struct CmdLine {

    CmdLine(bool all);
    CmdLine(bool _print_file_info,
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
            bool _print_stats);
    virtual ~CmdLine();

    inline void setPreprocessStreamMode() { preprocess_stream_only=true; }
    inline void setPreprocessStreamModeNoDeb() { preprocess_stream_only_no_deb=true; }
    bool someSet();
    
    bool print_file_info;
    bool print_original_query;
    bool print_optimized_query;
    bool print_rewritten_query;
    bool print_projection_tree;
    bool print_projection_dfa;
    bool print_projection_nfa;
	bool print_fsamap;
	bool print_pathenv;
    bool print_initial_buffer;
    bool print_final_buffer;
    bool print_final_dfa;
    bool print_tag_map;
    bool print_stats;

    bool preprocess_stream_only;
    bool preprocess_stream_only_no_deb;
};

#endif // CMDLINE_H
