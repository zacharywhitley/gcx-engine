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
#include "stdio.h"
#include <iostream>
#include <unistd.h>
#include "executor.h"
#include "cmdline.h"
#include "intsingleset.h"
#include "stringhash.h"
#include "version.h"

#include "debug.h"

using namespace std;

#ifdef DBG_YACC
extern int yydebug;
#endif

void printVersionNumber() {
	cout << "This is 'gcx' version " << VERSION_NUMBER << endl << endl; 
	exit(1);
}

void invalidCall(const char* exec_name) {
    cout << "Usage: " << exec_name
			<< " [OPTIONS] <query_file> <xml_file>" << endl << endl; 
    cout << "Options:" << endl;
    cout << "--pdebug\t\t-> print detailed information"
			<< " (everything listed below)" << endl;
    cout << "--pfileinfo\t\t-> prints information on used files" << endl;
    cout << "--pqueryorig\t\t-> print original query" << endl;
    cout << "--pqueryopt\t\t-> print optimized query" << endl;
    cout << "--pqueryrew\t\t-> print the rewritten query (updates)" << endl;
    cout << "--pprojtree\t\t-> print the constructed projection tree" << endl;
    cout << "--pprojdfa\t\t-> print the constructed projection DFA" << endl;
    cout << "--pprojnfa\t\t-> print the constructed projection NFA" << endl;
    cout << "--pfsamap\t\t-> print variable to FSA matching" << endl;
    cout << "--ppathenv\t\t-> print variable to path matching" << endl;
    cout << "--pinitbuf\t\t-> print the initial buffer" << endl;
    cout << "--pfinalbuf\t\t-> print the final buffer" << endl;
    cout << "--pfinaldfa\t\t-> print the final projection DFA" << endl;
    cout << "--ptagmap\t\t-> print the tag map" << endl;
    cout << "--pstats\t\t-> print statistics if available" << endl;
    cout << endl;
    cout << "--streamnodeb\t\t-> process stream only (no debug output)" << endl;
    cout << "--stream\t\t-> process stream only (with debug output)" << endl;
	cout << endl;
	cout << "--version\t\t-> print version number" << endl;
}

int main(int argc,char **argv) {

#ifdef DBG_YACC
  yydebug = 1;
#endif


    if (argc<3) {
		if (argc==2 && strcmp(argv[1],"--version")==0) {
			printVersionNumber();
		} else {
        	invalidCall(argv[0]);
        	return -1;
		}
    }
    
    ///////////////////////////////////////////////////////////
    // parse command line options
    bool debug(false); // print everything
    bool print_file_info(false);
    bool print_original_query(false);
    bool print_optimized_query(false);
    bool print_rewritten_query(false);
    bool print_projection_tree(false);
    bool print_projection_dfa(false);
    bool print_projection_nfa(false);
	bool print_fsamap(false);
	bool print_pathenv(false);
    bool print_initial_buffer(false);
    bool print_final_buffer(false);
    bool print_final_dfa(false);
    bool print_tag_map(false);
    bool print_stats(false);
    bool process_stream_only_no_deb=false;
    bool process_stream_only=false;
    
    for (int i=1;i<argc-2;i++) {
        if (strcmp(argv[i],"--pdebug")==0) {
            debug=true;
        } else if (strcmp(argv[i],"--pfileinfo")==0) {
            print_file_info=true;
        } else if (strcmp(argv[i],"--pqueryorig")==0) {
            print_original_query=true;
        } else if (strcmp(argv[i],"--pqueryopt")==0) {
            print_optimized_query=true;
        } else if (strcmp(argv[i],"--pqueryrew")==0) {
            print_rewritten_query=true;
        } else if (strcmp(argv[i],"--pprojtree")==0) {
            print_projection_tree=true;
        } else if (strcmp(argv[i],"--pprojdfa")==0) {
            print_projection_dfa=true;
        } else if (strcmp(argv[i],"--pprojnfa")==0) {
            print_projection_nfa=true;
		} else if (strcmp(argv[i],"--pfsamap")==0) {
			print_fsamap=true;
		} else if (strcmp(argv[i],"--ppathenv")==0) {
			print_pathenv=true;
        } else if (strcmp(argv[i],"--pinitbuf")==0) {
            print_initial_buffer=true;
        } else if (strcmp(argv[i],"--pfinalbuf")==0) {
            print_final_buffer=true;
        } else if (strcmp(argv[i],"--pfinaldfa")==0) {
            print_final_dfa=true;
        } else if (strcmp(argv[i],"--ptagmap")==0) {
            print_tag_map=true;
        } else if (strcmp(argv[i],"--pstats")==0) {
            print_stats=true;
        } else if (strcmp(argv[i],"--streamnodeb")==0) {
            process_stream_only_no_deb=true;
        } else if (strcmp(argv[i],"--stream")==0) {
            process_stream_only=true;
		} else if (strcmp(argv[i],"--version")==0) {
			printVersionNumber();
        } else {
            cout << "Unknown option: '" << argv[i] << "'...";
            exit(-1);
        }
    }

    const char* query_file=argv[argc-2];
    const char* xml_file=argv[argc-1];

    
    ///////////////////////////////////////////////////////////
    // set up cmdline object 
    CmdLine* cmd;
    if (process_stream_only) {
        cmd=new CmdLine(true); // automatically set debug mode
        cmd->setPreprocessStreamMode();
    } else if (process_stream_only_no_deb) {
        cmd=new CmdLine(false); // automatically set debug mode
        cmd->setPreprocessStreamModeNoDeb();
    } else if (debug) {
        cmd=new CmdLine(true);
    } else {
        cmd=new CmdLine(
            print_file_info,
            print_original_query,
            print_optimized_query,
            print_rewritten_query,
            print_projection_tree,
            print_projection_dfa,
            print_projection_nfa,
			print_fsamap,
			print_pathenv,
            print_initial_buffer,
            print_final_buffer,
            print_final_dfa,
            print_tag_map,
			print_stats);
    }


    ///////////////////////////////////////////////////////////
    // start execution
    cout << endl;
    Executor* e=new Executor(query_file,xml_file,cmd);
    e->start();
    delete e;

    return 0;
}
