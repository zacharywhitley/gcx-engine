
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
 *  @brief ENTRY POINT.
 *  @details ENTRY POINT OF THIS APPLICATION.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */

/*! @mainpage G(arbage) C(ollected) X(Query) Engine Documentation
 *  @section sec_abstract Abstract
 *  @par
 *    The G(arbage) C(ollected) X(Query) engine is the first streaming XQuery engine
 *    that implements active garbage collection, a novel buffer management strategy
 *     in which both static and dynamic analysis are exploited. This technique actively
 *     purges main memory buffers at runtime based on the current status of query evaluation.
 *     This approach aims at both keeping main memory consumption low at runtime and
 *     speeding up query evaluation. For detailed information on active garbage collection
 *     in XQuery engines please visit the GCX project homepage at \n \n
 *       http://dbis.informatik.uni-freiburg.de/index.php?project=GCX
 *  @section sec_readme Readme
 *  @par
 *    @verbinclude README.txt
 *  @section sec_installation Installation
 *    @verbinclude INSTALL.txt
 *  @section sec_dl_ml Download/Mailing Lists
 *  @par Download
 *    The latest version of GCX can be found at \n \n
 *      http://dbis.informatik.uni-freiburg.de/index.php?project=GCX \n \n
 *    or \n \n
 *      http://sourceforge.net/project/showfiles.php?group_id=258398
 *  @par Mailing Lists
 *    For feedback, such as questions, comments, bug reports or
 *    feature requests please use one of the following GCX mailing lists. \n \n
 *      http://lists.sourceforge.net/mailman/listinfo/gcx-engine-general \n
 *        Mailing list for general discussion about GCX (general questions, comments ...). \n \n
 *      http://lists.sourceforge.net/mailman/listinfo/gcx-engine-support \n
 *        Mailing list to ask questions about using and building GCX. \n \n
 *      http://lists.sourceforge.net/mailman/listinfo/gcx-engine-bugs \n
 *        Mailing list for bug reports and discussion about bugs in GCX. \n \n
 *      http://lists.sourceforge.net/mailman/listinfo/gcx-engine-requests \n
 *        Mailing list to request new or desired features for future releases. \n \n
 *    To get in direct communication with us, feel free to send an email to \n \n
 *      Michael Schmidt (mschmidt@informatik.uni-freiburg.de) \n \n
 *    or \n \n
 *      Gunnar Jehl (jehl@informatik.uni-freiburg.de).
 *  @section sec_members Project Members
 *  @par
 *    @li Michael Schmidt, Freiburg University, Contact Person
 *    @li Gunnar Jehl, Freiburg University, Contact Person
 *    @li Prof. Dr. Christoph Koch, Cornell University
 *    @li Prof. Dr. Georg Lausen, Freiburg University
 *    @li Stefanie Scherzinger, IBM B&ouml;blingen
 *  @par
 *    @author Michael Schmidt
 *    @author Gunnar Jehl
 *    @version 2.1
 *    @license Software License Agreement (BSD License)
 */
#include <getopt.h>
#include <cstring>
#include <cstdlib>
#include "executor.h"
#include "version.h"
#include "debug.h"
#include "invalidargumentexception.h"
#include "stringdefs.h"

//! @namespace std
using namespace std;

#ifdef DBG_YACC

/*! @def yydebug
 *  @brief YACC Parser debug option.
 *  @details YACC Parser debug option.
 */
extern int yydebug;
#endif

/*! @var Executor* exec
 *  @brief The Executor.
 *  @details The Executor.
 */
Executor *exec = NULL;

/*! @enum short_opts
 *  @brief Short options definition.
 *  @details Short options definition.
 */
enum short_opts {
    // query/xml/output informations
    opt_iqstream = '1',
    opt_ixstream = '2',
    opt_oestream = '3',
    opt_odstream = '4',
    opt_query_arg = 'q',
    opt_xml_arg = 'x',
    opt_eout_arg = 'e',
    opt_dout_arg = 'o',
    // debug options
    opt_debug = 'd',
    // processing options
    opt_streamdebug = 's',
    opt_streamnodebug = 'b',
    // misc options
    opt_fragmentxq = 'f',
    // info options
    opt_version = 'v',
    opt_about = 'a',
    opt_help = 'h',
};

/*! @struct option long_options[]
 *  @brief Long options definition.
 *  @details Long options definition.
 */
static struct option long_options[] = {
    // query/xml/output informations
    {"iqstream", required_argument, 0, opt_iqstream},
    {"ixstream", required_argument, 0, opt_ixstream},
    {"oestream", required_argument, 0, opt_oestream},
    {"odstream", required_argument, 0, opt_odstream},
    {"query", required_argument, 0, opt_query_arg},
    {"xml", required_argument, 0, opt_xml_arg},
    {"eout", required_argument, 0, opt_eout_arg},
    {"dout", required_argument, 0, opt_dout_arg},
    // debug options
    {"debug", no_argument, 0, opt_debug},
    // processing options
    {"streamdebug", no_argument, 0, opt_streamdebug},
    {"streamnodebug", no_argument, 0, opt_streamnodebug},
    // misc options
    {"fragmentxq", no_argument, 0, opt_fragmentxq},
    // info options
    {"version", no_argument, 0, opt_version},
    {"about", no_argument, 0, opt_about},
    {"help", no_argument, 0, opt_help},
    {0, 0, 0, 0}
};

/*! @struct descriptions
 *  @brief Options description definition.
 *  @details Options description definition.
 */
static struct descriptions {
    /*! @var int val
     *  @brief The option index.
     *  @details The option index.
     */
    int val;

    /*! @var const char* desc
     *  @brief The option description.
     *  @details The option description.
     */
    const char *desc;
}

/*! @struct options_descriptions[]
 *  @brief Options description definition.
 *  @details Options description definition.
 */ options_descriptions[] = {
    // query/xml/output informations
    {
    opt_iqstream, ": input stream type of query"}, {
    opt_ixstream, ": input stream type of xml"}, {
    opt_oestream, ": output stream type of query result"}, {
    opt_odstream, ": output stream type of debug output"}, {
    opt_query_arg, ""}, {
    opt_xml_arg, ""}, {
    opt_eout_arg, ""}, {
    opt_dout_arg, ""},
        // debug options
    {
    opt_debug, ": print detailed debug information"},
        // processing options
    {
    opt_streamdebug, ": process stream only (with debug output)"}, {
    opt_streamnodebug, ": process stream only (without debug output)"},
        // misc options
    {
    opt_fragmentxq, ": print supported XQuery fragment (XQ)"},
        // info options
    {
    opt_version, ": print version number and compile flags"}, {
    opt_about, ": print about information"}, {
    opt_help, ": print this information"}
};

/*! @fn void printFragmentXQ()
 *  @brief Prints supported XQuery fragment (XQ).
 *  @details Prints supported XQuery fragment (XQ).
 *  @retval void
 */
void printFragmentXQ() {
    cout << FRAGMENT_TXT << endl;       // see stringdefs.h
}

/*! @fn void printVersionNumber()
 *  @brief Prints current version number and compile flags.
 *  @details Prints current version number and compile flags.
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

/*! @fn void printAbout()
 *  @brief Prints about (license and author) informations.
 *  @details Prints about (license and author) informations.
 *  @retval void
 */
void printAbout() {
    cout << LICENSE_TXT << endl;        // see stringdefs.h
    cout << "----------------------------------------" << endl << endl;
    cout <<
        "Contributors: Michael Schmidt (mschmidt@informatik.uni-freiburg.de), Author"
        << endl;
    cout <<
        "              Gunnar Jehl (jehl@informatik.uni-freiburg.de), Author" <<
        endl;
    cout << "              Christoph Koch (koch@cs.cornell.edu)" << endl;
    cout << "              Stefanie Scherzinger, Author" << endl;
}

/*! @fn void invalidCall(const char* exec_name)
 *  @brief Prints all available application call parameters.
 *  @details Prints all available application call parameters, if application was called
 *  		with none or wrong parameters.
 *  @param[in] exec_name Name of the application.
 *  @retval void
 */
void invalidCall(const char *exec_name) {
    cout << "Usage: " << exec_name << " [STD EVAL MODE] or" << endl;
    cout << "       " << exec_name << " [EXT EVAL MODE] or" << endl;
    cout << "       " << exec_name << " [INFO MODE]";
    cout << endl << endl;
    cout <<
        "[STD EVAL MODE] ::= --query <query_file> [--xml <xml_file>] [OPTION]?"
        << endl;
    cout <<
        "                   (--xml <xml_file> required if document is not given in query)"
        << endl;
    cout << "[EXT EVAL MODE] ::= [STREAM SPEC]+ [OPTION]?" << endl;
    cout << "  [OPTION] ::= choose one of the following" << endl;
    for (unsigned i = 8; long_options[i].name && i < 11; i++) {
        cout << "    --" << long_options[i].
            name << " " << options_descriptions[i].desc << endl;
    }
    cout << endl;
    cout << "  [STREAM SPEC] ::= choose one of the following" << endl;
    for (unsigned i = 0; long_options[i].name && i < 2; i++) {
        cout << "    --" << long_options[i].
            name << " [INPUT TYPE] [PARAM]? " << options_descriptions[i].
            desc << endl;
    }
    for (unsigned i = 2; long_options[i].name && i < 4; i++) {
        cout << "    --" << long_options[i].
            name << " [OUTPUT TYPE] [PARAM]? " << options_descriptions[i].
            desc << endl;
    }
    cout << endl;
    cout << "  [INPUT TYPE] ::= choose one of the following" << endl;
    cout << "    file : file input (DEFAULT)" << endl;
    cout <<
        "      -> when used with --iqstream provide parameter --query <query_file>"
        << endl;
    cout <<
        "      -> when used with --ixstream provide parameter --xml <xml_file>"
        << endl;
    cout << "         (required if document is not given in query)" << endl;
    cout <<
        "    null : no input (support only for --ixstream for debugging purposes)"
        << endl;
    cout << "    stdin : standard input (either for query or for xml document)"
        << endl;
    cout << endl;
    cout << "  [OUTPUT TYPE] ::= choose one of the following" << endl;
    cout << "    file : file output" << endl;
    cout <<
        "      -> when used with --oestream provide parameter --eout <eval_output_file>"
        << endl;
    cout <<
        "      -> when used with --odstream provide parameter --dout <debug_output_file>"
        << endl;
    cout << "    null : no output" << endl;
    cout << "    stdout : standard output (DEFAULT)" << endl;
    cout << endl;
    cout << "EXAMPLES:" << endl;
    cout << exec_name << " --query query.xq --xml doc.xml" << endl;
    cout <<
        "  => query input from file \"query.xq\" and xml input from file \"doc.xml\""
        << endl;
    cout << "  => query result output to stdout" << endl;
    cout << exec_name <<
        " --iqstream stdin --xml doc.xml --odstream file --dout debug.out --debug"
        << endl;
    cout << "  => query input from stdin and xml input from file \"doc.xml\"" <<
        endl;
    cout <<
        "  => debug output to file \"debug.out\" and query result output to stdout"
        << endl;
    cout << exec_name <<
        " --query query.xq --xml doc.xml --oestream file --eout result.xml --odstream null --debug"
        << endl;
    cout <<
        "  => query input from file \"query.xq\" and xml input from file \"doc.xml\""
        << endl;
    cout <<
        "  => discard debug output and query result output to file \"result.xml\""
        << endl;
    cout << endl;
    cout << "[INFO MODE] ::= choose one of the following" << endl;
    for (unsigned i = 11; long_options[i].name; i++) {
        cout << "    --" << long_options[i].
            name << " " << options_descriptions[i].desc << endl;
    }
}

/*! @fn ISTREAM_TYPE getInputStreamType(const char* arg)
 *  @brief Returns the input stream type from a string (application call parameter).
 *  @details Returns the input stream type from a string (application call parameter).
 *  @param[in] arg Pointer to a char object (application call parameter), which should be checked for matching.
 *  @retval ISTREAM_TYPE.
 */
ISTREAM_TYPE getInputStreamType(const char *arg) {
    if (strcmp(arg, "file") == 0) {
        return it_file;
    } else if (strcmp(arg, "null") == 0) {
        return it_null;
    } else if (strcmp(arg, "socket") == 0) {
        return it_socket;
    } else {
        return it_stdin;
    }
}

/*! @fn OSTREAM_TYPE getOutputStreamType(const char* arg)
 *  @brief Returns the output stream type from a string (application call parameter).
 *  @details Returns the output stream type from a string (application call parameter).
 *  @param[in] arg Pointer to a char object (application call parameter), which should be checked for matching.
 *  @retval OSTREAM_TYPE.
 */
OSTREAM_TYPE getOutputStreamType(const char *arg) {
    if (strcmp(arg, "file") == 0) {
        return ot_file;
    } else if (strcmp(arg, "null") == 0) {
        return ot_null;
    } else if (strcmp(arg, "socket") == 0) {
        return ot_socket;
    } else {
        return ot_stdout;
    }
}

/*! @fn void executeOnExit()
 *  @brief Method, which is always invoked/executed on exit.
 *  @details Method, which is always invoked/executed on exit.
 *  @retval void
 */
void executeOnExit() {
    delete exec;

    exec = NULL;
}

/*! @fn int main(int argc, char **argv)
 *  @brief MAIN.
 *  @details MAIN.
 *  @param[in] argc Number of entered application call parameters.
 *  @param[in] argv All entered application call parameters.
 *  @retval int <tt>EXIT_FAILURE or 1</tt> in case of an error, 
 *    <tt>EXIT_SUCCESS or 0</tt> if application terminated with success. 
 */
int main(int argc, char **argv) {

#ifdef DBG_YACC
    yydebug = 1;
#endif

    ISTREAM_TYPE query_istream_type(it_file);
    ISTREAM_TYPE xml_istream_type(it_file);
    OSTREAM_TYPE eout_ostream_type(ot_stdout);
    OSTREAM_TYPE dout_ostream_type(ot_stdout);
    const char *query_arg(NULL);
    const char *xml_arg(NULL);
    const char *eout_arg(NULL);
    const char *dout_arg(NULL);
    bool debug(false);
    bool preprocess_stream_debug(false);
    bool preprocess_stream_no_debug(false);

    if (argc == 1) {
        invalidCall(argv[0]);
        return EXIT_SUCCESS;
    }

    unsigned idx = 0;
    unsigned req_arg = 0;

    for (unsigned i = 0; long_options[i].name; i++) {
        if (long_options[i].has_arg == required_argument) {
            req_arg++;
        }
    }
    char short_options[(sizeof (long_options) / sizeof (long_options[0]) - 1) +
                       req_arg];
    for (unsigned i = 0; long_options[i].name; i++) {
        short_options[idx++] = long_options[i].val;
        if (long_options[i].has_arg == required_argument) {
            short_options[idx++] = ':';
        }
    }

    while (1) {
        int option_index = 0;

        short_options[idx] = 0;
        int option_int = getopt_long(argc, argv, short_options, long_options,
                                     &option_index);

        if (option_int == -1) {
            break;
        }

        switch (option_int) {
            case 0:
                break;
            case opt_iqstream:
                if (optarg) {
                    query_istream_type = getInputStreamType(optarg);
                }
                break;
            case opt_ixstream:
                if (optarg) {
                    xml_istream_type = getInputStreamType(optarg);
                }
                break;
            case opt_oestream:
                if (optarg) {
                    eout_ostream_type = getOutputStreamType(optarg);
                }
                break;
            case opt_odstream:
                if (optarg) {
                    dout_ostream_type = getOutputStreamType(optarg);
                }
                break;
            case opt_query_arg:
                if (optarg) {
                    query_arg = optarg;
                }
                break;
            case opt_xml_arg:
                if (optarg) {
                    xml_arg = optarg;
                }
                break;
            case opt_eout_arg:
                if (optarg) {
                    eout_arg = optarg;
                }
                break;
            case opt_dout_arg:
                if (optarg) {
                    dout_arg = optarg;
                }
                break;
            case opt_debug:
                if (!preprocess_stream_no_debug) {
                    debug = true;
                }
                break;
            case opt_streamdebug:
                if (!preprocess_stream_no_debug && !debug) {
                    preprocess_stream_debug = true;
                    debug = true;
                }
                break;
            case opt_streamnodebug:
                if (!preprocess_stream_debug && !debug) {
                    preprocess_stream_no_debug = true;
                    debug = false;
                }
                break;
            case opt_fragmentxq:
                if (!preprocess_stream_no_debug && !preprocess_stream_debug
                    && !debug) {
                    printFragmentXQ();
                    return EXIT_SUCCESS;
                }
                break;
            case opt_version:
                if (!preprocess_stream_no_debug && !preprocess_stream_debug
                    && !debug) {
                    printVersionNumber();
                    return EXIT_SUCCESS;
                }
                break;
            case opt_about:
                if (!preprocess_stream_no_debug && !preprocess_stream_debug
                    && !debug) {
                    printAbout();
                    return EXIT_SUCCESS;
                }
                break;
            case opt_help:
                if (!preprocess_stream_no_debug && !preprocess_stream_debug
                    && !debug) {
                    invalidCall(argv[0]);
                    return EXIT_SUCCESS;
                }
                break;
            case '?':
                return EXIT_FAILURE;
            default:
                return EXIT_FAILURE;
        }
    }

    if (optind < argc) {
        cout << endl;
        cout << "non-option ARGV-element(s): ";
        while (optind < argc) {
            cout << argv[optind++] << endl;
        }
        return EXIT_FAILURE;
    }

    CmdLine *cmd =
        new CmdLine(query_istream_type, xml_istream_type, eout_ostream_type,
                    dout_ostream_type, query_arg, xml_arg, eout_arg, dout_arg,
                    debug, preprocess_stream_debug,
                    preprocess_stream_no_debug);

    try {
        cmd->checkIntegrity();
    }
    catch(InvalidArgumentException & e) {
        cout << e.getDebugMsg();
        return EXIT_FAILURE;
    }
    catch(Exception & e) {
        cout << e.getDebugMsg();
        return EXIT_FAILURE;
    }

    exec = new Executor(cmd);
    atexit(executeOnExit);
    exec->start();
    delete exec;

    exec = NULL;

    return EXIT_SUCCESS;
}
