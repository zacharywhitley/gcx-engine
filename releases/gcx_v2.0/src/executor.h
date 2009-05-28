/*
 | Author: Michael Schmidt; modified by Steffi Scherzinger;
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
 *  @brief Header file for executor.cpp.
 *  @details Header file specifying constructors, destructor and functions for executor.cpp.
 *  @author Michael Schmidt
 *  @author modified by Steffi Scherzinger
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 *  @license Software License Agreement (BSD License)
 *
 * @todo grep for and remove TODOs in sources! (gje)
 * @todo INCOMMENT YYLEX PARTS in:
 * 		 -> query_parser.h method parse_query()
 *       -> loader.cpp method destroyLexer() !!! (gje, msc)
 * @todo valgrind (gje): manually check aggregate funct queries for functions that are
 *       NOT used in the standard testsuite queries (due to incompatibility)
 * @todo check experiment result for ALL xmark docs with galax (msc)
 * @todo check runtime and memory consumption for xmark docs 50/100 WITHOUT
 *       rewriteVarsteps() BUT WITH ALL other optimizations!!!
 * @todo sorting of methods. (drop or gje)
 *
 * @todo AFTER EXPERIMENTS:
 * @li LICENSE, README etc; replace Saarland University; add new license to header file (msc)
 * @li finish documentation (gje)
 * @li remove ALL empty files and copy experiment queries into folder gcx_queries_final (gje)
 */
#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "pathenvironment.h"
#include "buffer.h"
#include "streampreprocessor.h"
#include "fsamap.h"
#include "cmdline.h"
#include "sequenceexpression.h"
#include "forexpression.h"
#include "varstepexpression.h"
#include "passiveprojectiontree.h"

/*! @class Executor
 *     @brief Represents the main control instance.
 *     @details The Executor controls the work flow and is responsible
 *              for calling the modules (like statical analysis and evaluation)
 *              in a well-defined order.
 *     @author Michael Schmidt
 *     @author modified by Steffi Scherzinger
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
class Executor {

public:
    /*! @brief Constructor.
     *  @details Constructor - creates an executor object.
     *  @param[in] _query_file Name of the query input file.
     *  @param[in] _xml_file Name of the input document file.
     *  @param[in] _cmd The command line object that stores the command line options.
     */
    Executor(const char* _query_file, const char* _xml_file, CmdLine* _cmd);
    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~Executor();

    /*! @brief Runs the Executor.
     *  @details Executes the program according to the command line options specified in
     *          the cmd member. In doing so, the method calls (depending on the command
     *          line options) the private methods of this class in well-defined order.
     *  @retval void
     */
    void start();

private:
    /*! @brief Parse the input query.
     *  @details
     *  @retval void
     */
    void parseQuery();

    /*! @brief Statically computes an optimized version of the query.
     *  @details In particular, the method merges sequences, rewrites aggregate functions, etc.
     *  @retval void
     */
    void optimizeQuery();

    /*! @brief Gathers information from the input query that are required in further steps.
     *  @details In particular, the method computes the first straight ancestor (FSA) map, ParVar map, the path environment
     *           and initialized the role list accordingly. All information is stored in the
     *           respective member variables.
     *  @retval void
     */
    void collectQueryInformation();

    /*! @brief Builds the variable tree.
     * @details The variable tree is stored in the member variable vt.
     * @retval void
     */
    void buildVariableTree();

    /*! @brief Builds the dependency set.
     *  @details The dependency set is stored in the depset member variable depset. 
     *  @retval void
     */
    void buildDependencies();

    /*! @brief Builds the projection tree.
     *  @details The projection tree is stored in the depset member variable pt. 
     *  @retval void
     */
    void buildProjectionTree();

    /*! @brief Enriches the query with signOff statements.
     *  @details The signOffs are inserted according to the role list. Note that no new query
     *          object is created, but instead the original query is rewritten.
     *  @retval void
     */
    void buildSignOffQuery();

    /*! @brief Insert a vector of signOff expressions into the query.
     *  @details The method gets passed a vector of signOff expressions and inserts them, one by one,
     *          into the query, thereby modifying the original query. This is a help method of
     *          buildSignOffQuery.
     *  @param[in] signoffs A vector of pointers to SignOffExpression objects.
     *  @retval void 
     */
     void insertSignOffs(vector<SignOffExpression*>& signoffs);

    /*! @brief Initializes the projection DFA.
     *  @details The method uses the member variable pt to set up an initial projection DFA.
     *          The projection DFA will then be expanded on demand, whenever entering
     *          states that have not been visited before; hence, the initial projection
     *          DFA is ready to be used by the stream preprojector.
     *  @retval void
     */
    void initProjectionDFA();

    /*! @brief Initializes the stream preprocessor. 
     *  @details This initialization step comprises the initialization of an (empty) buffer
     *          and the single StreamPreProcessor object that will be used to project the
     *          input stream.
     *  @retval void
     */
    void initStreamParser();

    /*! @brief Evaluates the input query.
     *  @details Once everything is initialized properly, this method can be used to start
     *          query evaluation. To be called right at the end of the statical analysis phase.
     *  @retval void
     */
    void evalQuery();

    /*! @brief Preprocess the input stream without query evaluation.
     *  @details This method is used to preprocess the input stream without evaluating the
     *          input query. While not required in 'regular' query evaluation runs, it is
     *          helpful for debugging purposes.
     *  @retval void
     */
    void preprocessStream();

    /*! @brief Do some final debug output.
     *  @details If the debug mode is turned on, this methd prints some final debug information
     *          (like final buffer, tagmap, projection DFA) to STDOUT
     *  @retval void
     */
    void finalDebugPrint(bool sppmode);

    /*! @var const char* query_file
     *  @brief The name of the input query file.
     *  @details
     */
    const char* query_file;

    /*! @var const char* xml_file
     *  @brief The name of the input query file.
     *  @details
     */
    const char* xml_file;

    /*! @var const Expression* query
     *  @brief The parsed input query.
     *  @details The query expression will be modified in the statical analysis phase,
     *          e.g. by some optimization steps or enrichment with signOff statements.
     */
    Expression* query;

    /*! @var Buffer* buffer
     *  @brief Pointer to the Buffer object used during evaluation.
     *  @details
     */
    Buffer* buffer;

    /*! @var FSAMap* fsamap 
     *  @brief Pointer to the map that contains variable-to-first straight ancestor (FSA) mappings.
     *  @details
     */
    FSAMap* fsamap;

    /*! @var ParVarMap* parvarmap
     *  @brief Pointer to the map that contains variable-to-parent-variable mappings.
     *  @details
     */
    ParVarMap* parvarmap;

    /*! @var PathEnvironment* penv
     *  @brief The path environment for the variables used inside the query expression.
     *  @details
     */
    PathEnvironment* penv;

    /*! @var VariableTree* vt
     *  @brief Pointer to the variable tree.
     *  @details
     */
    VariableTree* vt;

    /*! @var vector<DependencySet*>* depset
     *  @brief Pointer to a vector thet contains all the dependencies 
     *  @details
     */
    vector<DependencySet*>* depset;

    /*! @var ProjectionTree* pt 
     *  @brief Pointer to the projection tree used to construct the ProjectionDFA pdfa. 
     *  @details
     */
    ProjectionTree* pt;

    /*! @var PassiveProjectionTree* ppt 
     *  @brief Pointer to the passive projection tree used to construct the ProjectionDFA pdfa. 
     *  @details
     */
    PassiveProjectionTree* ppt;

    /*! @var ProjectionDFA* pdfa
     *  @brief Pointer to the projection DFA used for stream preprojection.
     *  @details
     */
    ProjectionDFA* pdfa;

    /*! @var CmdLine* cmd
     *  @brief Pointer to the CmdLine object that stores the passed command line options.
     *  @details
     */
    CmdLine* cmd;
};

#endif // EXECUTOR_H
