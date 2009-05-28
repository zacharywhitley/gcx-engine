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
 *  @brief ENTRY POINT.
 * 	@details ENTRY POINT OF THIS APPLICATION.
 * 	@author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 *  @license Software License Agreement (BSD License)
 */
/*! @mainpage Garbage Collected XQuery (GCX) Engine Documentation
 *  @section sec_time Time
 *  	@par
 *  	@verbinclude TIME.txt
 *  @section sec_abstract Abstract
 *  	Effective buffer management is crucial for efficient in-memory and streaming
 *  	XQuery processing. We propose a buffer management scheme that combines static
 *  	and dynamic analysis to keep main memory consumption low. Our approach relies
 *  	on a technique that we call active garbage collection and which actively purges
 *  	buffers at runtime based on the current status of query evaluation. This prototype
 *  	system implements a practical fragment of XQuery and employs our novel buffer
 *   	management scheme. The experimental results demonstrate the significant impact
 *   	of combined static and dynamic analysis on reducing main memory consumption and
 *   	running time.
 *  @subsection subsec_readme Readme
 * 		@par
 * 		@verbinclude README.txt
 *  @section sec_installation Installation
 *   	@par
 * 		The latest version of GCX can be found at: http://dbis.informatik.uni-freiburg.de/index.php?project=GCX \n
 *   	@par
 * 		The mailing list (for requests, discussion, and bug reports) can be found at: http://lists.math.uni-sb.de/mailman/listinfo/gcx-users
 *  @subsection subsec_instructions Instructions
 * 		@par
 * 		@verbinclude INSTALL.txt
 *  @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 *  @license Software License Agreement (BSD License)
 * 		@par
 * 		@verbinclude LICENSE.txt
 *  @section sec_changes Changes
 * 		@par
 * 		@verbinclude CHANGELOG.txt
 */
#include "executor.h"
#include "version.h"
#include "debug.h"

//! @namespace std
using namespace std;

#ifdef DBG_YACC
extern int yydebug;
#endif

Executor* exec=NULL;

/*! @fn void printVersionNumber()
 *  @brief Prints the current version number.
 *  @details Prints the current version number.
 *  @retval void
 */
void printVersionNumber() {
    cout << "This Is \"GCX\" Version " << VERSION_NUMBER << endl;
    cout << "Compiled With The Following Flags:";
	cout << endl;

	cout << "  ROLE REFCOUNT      => " <<
    #ifdef ROLE_REFCOUNT
	"on";
	#else
	"off";
	#endif // ifdef ROLE_REFCOUNT
	cout << endl;

	cout << "  NO_OPTIMIZATIONS   => " <<
    #ifdef NO_OPTIMIZATIONS
	"on";
	#else
	"off";
	#endif // ifdef NO_OPTIMIZATIONS
	cout << endl;

	cout << "  REWRITE_VARSTEPS   => " <<
    #ifdef REWRITE_VARSTEPS
	"on";
	#else
	"off";
	#endif // REWRITE_VARSTEPS
	cout << endl;

	cout << "  VALIDATION         => " <<
    #ifdef VALIDATION
	"on";
	#else
	"off";
	#endif // VALIDATION
	cout << endl;
}

/*! @fn void invalidCall(const char* exec_name)
 *  @brief Prints all available application call parameters.
 *  @details Prints all available application call parameters, if application was called with none or
 * 			less than three parameters.
 *  @param[in] exec_name Name of the application.
 *  @retval void
 */
void invalidCall(const char* exec_name) {
    cout << "Usage: " << exec_name << " [OPTIONS] <query_file> <xml_file>" << endl << endl;
    cout << "Options:" << endl;
    cout << "--pdebug\t\t-> print detailed information" << " (everything listed below)" << endl;
    cout << "--pfileinfo\t\t-> print information on used files" << endl;
    cout << "--pqueryorig\t\t-> print original input query" << endl;
    cout << "--pvarsqorig\t\t-> print set of variables (VARSq) of original query" << endl;
    cout << "--pqueryopt\t\t-> print optimized query" << endl;
    cout << "           \t\t   (available if build with optimizations (default:ON))" << endl;
    cout << "--pvarsqopt\t\t-> print set of variables (VARSq*) of optimized query" << endl;
    cout << "           \t\t   (available if build with optimizations (default:ON))" << endl;    
    cout << "--pqueryrew\t\t-> print the rewritten query with signOffs" << endl;    
    cout << "--pvartree\t\t-> print the constructed variable tree" << endl;    
    cout << "--pquerydepsorig\t-> print original query dependencies without roles" << endl;
    cout << "--pquerydepswo\t\t-> print optimized query dependencies without roles" << endl;
    cout << "              \t\t   (available if build with optimizations (default:ON))" << endl;
    cout << "--pquerydepsw\t\t-> print original/optimized query dependencies with roles" << endl;  
    cout << "--pprojtreeorig\t\t-> print the original constructed projection tree" << endl;
    cout << "--ppprojtree\t\t-> print the constructed passive projection tree" << endl;
    cout << "            \t\t   (available if build with optimizations (default:ON))" << endl;
    cout << "--pprojtreeopt\t\t-> print the optimized projection tree" << endl;
    cout << "              \t\t   (available if build with optimizations (default:ON))" << endl;
    cout << "--pprojtreelab\t\t-> print the projection tree labels" << endl;
    cout << "--pprojdfa\t\t-> print the constructed projection deterministic finite automaton (DFA)" << endl;
    cout << "--pfsamap\t\t-> print variables to first straight ancestor (FSA) mapping" << endl;
    cout << "--pparvarmap\t\t-> print variables to parent variable (ParVar) mapping" << endl;
    cout << "--ppathenv\t\t-> print variables to path (Path Environment) mapping" << endl;
    cout << "--pinitbuf\t\t-> print the initial buffer" << endl;
    cout << "--pfinalbuf\t\t-> print the final buffer" << endl;
    cout << "--pfinaldfa\t\t-> print the final projection deterministic finite automaton (DFA)" << endl;
    cout << "--ptagmap\t\t-> print the final numbers to tagnames mapping" << endl;
    cout << endl;
    cout << "--streamnodeb\t\t-> process stream only (no debug output)" << endl;
    cout << "--stream\t\t-> process stream only (with debug output)" << endl;
    cout << endl;
    cout << "--version\t\t-> print version number" << endl;
}

// TODO: document this!!!
void executeOnExit() {
	delete exec;
	exec=NULL;
}

/*! @fn int main(int argc, char **argv)
 *  @brief MAIN.
 *  @details MAIN.
 *  @param[in] argc Number of entered application call parameters.
 *  @param[in] **argv All entered application call parameters.
 *  @retval int <tt>EXIT_FAILURE or 1</tt> in case of an error, 
 * 			<tt>EXIT_SUCCESS or 0</tt> if application terminated with success or if version number is printed. 
 */
int main(int argc, char **argv) {

#ifdef DBG_YACC
    yydebug = 1;
#endif

    if (argc<3) {
        if (argc==2 && strcmp(argv[1], "--version")==0) {
            printVersionNumber();
            return EXIT_SUCCESS;
        } else {
            invalidCall(argv[0]);
            return EXIT_FAILURE;
        }
    }

    bool debug(false); // print everything
    bool print_file_info(false);
    bool print_original_query(false);
    bool print_original_varsq(false);
    bool print_optimized_query(false);
    bool print_optimized_varsq(false);
    bool print_rewritten_query(false);
    bool print_variable_tree(false);
    bool print_original_query_dependencies_wo(false);
    bool print_optimized_query_dependencies_wo(false);
    bool print_optimized_query_dependencies_w(false);
    bool print_original_projection_tree(false);
    bool print_passive_projection_tree(false);
    bool print_optimized_projection_tree(false);
    bool print_projection_tree_labels(false);
    bool print_projection_dfa(false);
    bool print_fsamap(false);
    bool print_parvarmap(false);
    bool print_pathenv(false);
    bool print_initial_buffer(false);
    bool print_final_buffer(false);
    bool print_final_dfa(false);
    bool print_tag_map(false);
    bool process_stream_only_no_deb=false;
    bool process_stream_only=false;

    for (int i=1; i<argc-2; i++) {
        if (strcmp(argv[i], "--pdebug")==0) {
            debug=true;
        } else if (strcmp(argv[i], "--pfileinfo")==0) {
            print_file_info=true;
        } else if (strcmp(argv[i], "--pqueryorig")==0) {
            print_original_query=true;
        } else if (strcmp(argv[i], "--pvarsqorig")==0) {
            print_original_varsq=true;
        } else if (strcmp(argv[i], "--pqueryopt")==0) {
            print_optimized_query=true;            
        } else if (strcmp(argv[i], "--pvarsqopt")==0) {
            print_optimized_varsq=true;
        } else if (strcmp(argv[i], "--pqueryrew")==0) {
            print_rewritten_query=true;
        } else if (strcmp(argv[i], "--pvartree")==0) {
            print_variable_tree=true;
        }else if (strcmp(argv[i], "--pquerydepsorig")==0) {
        	print_original_query_dependencies_wo=true;
        } else if (strcmp(argv[i], "--pquerydepswo")==0) {
            print_optimized_query_dependencies_wo=true;
        } else if (strcmp(argv[i], "--pquerydepsw")==0) {
            print_optimized_query_dependencies_w=true;
        } else if (strcmp(argv[i], "--pprojtreeorig")==0) {
            print_original_projection_tree=true;
        } else if (strcmp(argv[i], "--ppprojtree")==0) {
        	print_passive_projection_tree=true;
        } else if (strcmp(argv[i], "--pprojtreeopt")==0) {
        	print_optimized_projection_tree=true;            
    	} else if (strcmp(argv[i], "--pprojtreelab")==0) {
			print_projection_tree_labels=true;
        } else if (strcmp(argv[i], "--pprojdfa")==0) {
            print_projection_dfa=true;
        } else if (strcmp(argv[i], "--pfsamap")==0) {
            print_fsamap=true;
        } else if (strcmp(argv[i], "--pparvarmap")==0) {
            print_parvarmap=true;
        } else if (strcmp(argv[i], "--ppathenv")==0) {
            print_pathenv=true;
        } else if (strcmp(argv[i], "--pinitbuf")==0) {
            print_initial_buffer=true;
        } else if (strcmp(argv[i], "--pfinalbuf")==0) {
            print_final_buffer=true;
        } else if (strcmp(argv[i], "--pfinaldfa")==0) {
            print_final_dfa=true;
        } else if (strcmp(argv[i], "--ptagmap")==0) {
            print_tag_map=true;
        } else if (strcmp(argv[i], "--streamnodeb")==0) {
            process_stream_only_no_deb=true;
        } else if (strcmp(argv[i], "--stream")==0) {
            process_stream_only=true;
        } else if (strcmp(argv[i], "--version")==0) {
            printVersionNumber();
        } else {
            cerr << "Unknown Option: " << argv[i] << "..." << endl << endl;
            return EXIT_FAILURE;
        }
    }

    const char* query_file=argv[argc-2];
    const char* xml_file=argv[argc-1];

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
                print_original_varsq,
                print_optimized_query,
                print_optimized_varsq,                
                print_rewritten_query,
                print_variable_tree,
                print_original_query_dependencies_wo,
                print_optimized_query_dependencies_wo,
                print_optimized_query_dependencies_w,
                print_original_projection_tree,
                print_passive_projection_tree,
                print_optimized_projection_tree,
				print_projection_tree_labels,
                print_projection_dfa,
                print_fsamap,
                print_parvarmap,
                print_pathenv,
                print_initial_buffer,
                print_final_buffer,
                print_final_dfa,
                print_tag_map);
    }

    exec=new Executor(query_file,xml_file,cmd);
    atexit(executeOnExit);
    exec->start();
    delete exec;
    exec=NULL;

    return EXIT_SUCCESS;
}
