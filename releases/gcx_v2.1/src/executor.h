
/*
 | Author: Michael Schmidt; modified by Steffi Scherzinger;
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
 *  @brief Header file for executor.cpp.
 *  @details Header file specifying constructors, destructor and functions for executor.cpp.
 *  @author Michael Schmidt
 *  @author modified by Steffi Scherzinger
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 * 
 *  @todo Possible extension: efficient JOINs
 *  @todo Possible extension: DTD support
 *  @todo Possible extension: xquery function fn:distinct-values()
 *  @todo Possible extension: attributes (in query and in xml)
 */
#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "cmdline.h"
#include "streammanager.h"
#include "inputstream.h"
#include "outputstream.h"
#include "fileinputstream.h"
#include "fileoutputstream.h"
#include "nullinputstream.h"
#include "nulloutputstream.h"
#include "socketinputstream.h"
#include "socketoutputstream.h"
#include "stdininputstream.h"
#include "stdoutoutputstream.h"
#include "exception.h"
#include "iostreamexception.h"
#include "parseexception.h"
#include "runtimeexception.h"
#include "invalidcastexception.h"
#include "pathenvironment.h"
#include "buffer.h"
#include "streampreprocessor.h"
#include "fsamap.h"
#include "docexpression.h"
#include "sequenceexpression.h"
#include "forexpression.h"
#include "varstepexpression.h"
#include "passiveprojectiontree.h"

/*! @class Executor
 *  @brief Represents the main control instance.
 *  @details Represents the main control instance. The executor controls
 *  		the work flow and is responsible for calling the modules
 *  		(like statical analysis and evaluation) in a well-defined order.
 *  @author Michael Schmidt
 *  @author modified by Steffi Scherzinger
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class Executor {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object for a CmdLine object.
     *  @param[in] _cmd Pointer to a CmdLine object (the command line object stores all command line arguments).
     */
    Executor(CmdLine * _cmd);

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ Executor();

    /*! @fn void start()
     *  @brief Runs the executor.
     *  @details Runs the executor, i.e. executes the program according to the
     *                  specified command line arguments. In doing so, the method calls
     *                  (depending on the command line arguments) the private methods
     *                  of this class in a well-defined order.
     *  @retval void
     */
    void start();

  private:
    /*! @fn void initStreams()
     *  @brief Initializes all required input/output streams.
     *  @details Initializes all required input/output streams.
     *  @retval void
     */
    void initStreams();

    /*! @fn void parseQuery()
     *  @brief Parses the input query.
     *  @details Parses the input query.
     *  @retval void
     */
    void parseQuery();

    /*! @fn void optimizeQuery()
     *  @brief Statically computes an optimized version of the input query.
     *  @details Statically computes an optimized version of the input query.
     *                      In particular, this method merges sequences, rewrites aggregate functions etc.
     *  @retval void
     */
    void optimizeQuery();

    /*! @fn void collectQueryInformation()
     *  @brief Gathers informations from the input query that are required in further steps.
     *  @details Gathers informations from the input query that are required in further steps.
     *                      In particular, this method computes the first straight ancestor (FSA) map, ParVar map,
     *                      the path environment and initialized the role list accordingly. All information is 
     *                      stored in the respective member variables.
     *  @retval void
     */
    void collectQueryInformation();

    /*! @fn void buildVariableTree()
     *  @brief Builds the variable tree.
     *  @details Builds the variable tree.
     *  @retval void
     */
    void buildVariableTree();

    /*! @fn void buildDependencies()
     *  @brief Builds the dependency sets.
     *  @details Builds the dependency sets.
     *  @retval void
     */
    void buildDependencies();

    /*! @fn void buildProjectionTree()
     *  @brief Builds the projection tree.
     *  @details Builds the projection tree.
     *  @retval void
     */
    void buildProjectionTree();

    /*! @fn void buildSignOffQuery()
     *  @brief Builds the rewritten query with signOff-statements.
     *  @details Builds the rewritten query with signOff-statements.
     *                      The signOff-statements are inserted according to the role list.
     *  @note No new query object is created, but instead the original query is rewritten!
     *  @retval void
     */
    void buildSignOffQuery();

    /*! @fn void insertSignOffs(vector<SignOffExpression*>& signoffs)
     *  @brief Inserts the signOff-statements into the input query.
     *  @details Inserts the signOff-statements into the input query.
     *                      This method is a help method of buildSignOffQuery().
     *  @param[in] signoffs Pointer to a vector containing pointers to all SignOffExpression objects.
     *  @retval void
     */
    void insertSignOffs(vector < SignOffExpression * >&signoffs);

    /*! @fn void initProjectionDFA()
     *  @brief Initializes the projection DFA.
     *  @details Initializes the projection DFA.
     *  @retval void
     */
    void initProjectionDFA();

    /*! @fn void initStreamParser()
     *  @brief Initializes the stream preprocessor.
     *  @details Initializes the stream preprocessor. This initialization step
     *                      comprises the initialization of an (empty) buffer and the single
     *                      StreamPreProcessor object that will be used to project the input stream.
     *  @retval void
     */
    void initStreamParser();

    /*! @fn void preprocessStream()
     *  @brief Preprocess the input stream without query evaluation.
     *  @details Preprocess the input stream without query evaluation. This method
     *                      is used to preprocess the input stream without evaluating the
     *              input query. While not required in 'regular' query evaluation runs, it is
     *              helpful for debugging purposes.
     *  @retval void
     */
    void preprocessStream();

    /*! @fn void evalQuery()
     *  @brief Evaluates the (rewritten) input query.
     *  @details Evaluates the (rewritten) input query. Once everything is
     *                      initialized properly, this method can be used to start query
     *                      evaluation.
     *  @note To be called right at the end of the statical analysis phase!
     *  @retval void
     */
    void evalQuery();

    /*! @fn void finalDebugPrint(bool sppmode)
     *  @brief Prints final debug output.
     *  @details Prints final debug output. If the debug mode is turned on,
     *                      this methd prints some final debug information (like final buffer,
     *                      tagmap, projection DFA etc.).
     *  @param[in] sppmode <tt>true</tt> if in debug-mode, <tt>false</tt> otherwise.
     *  @retval void
     */
    void finalDebugPrint(bool sppmode);

    /*! @var OutputStream* debug_ostream
     *  @brief The debug OutputStream.
     *  @details The debug OutputStream.
     */
    OutputStream *debug_ostream;

    /*! @var OutputStream* eval_ostream
     *  @brief The evaluation OutputStream.
     *  @details The evaluation OutputStream.
     */
    OutputStream *eval_ostream;

    /*! @var Expression* query
     *  @brief The parsed input query.
     *  @details The parsed input query, which will be modified in the statical analysis phase,
     *                  e.g. by some optimization steps or enrichment with signOff-statements.
     */
    Expression *query;

    /*! @var Buffer* buffer
     *  @brief The Buffer used during evaluation.
     *  @details The Buffer used during evaluation.
     */
    Buffer *buffer;

    /*! @var FSAMap* fsamap
     *  @brief The map that contains variables to first straight ancestor (FSA) mappings.
     *  @details The map that contains variables to first straight ancestor (FSA) mappings.
     */
    FSAMap *fsamap;

    /*! @var ParVarMap* parvarmap
     *  @brief The map that contains variables to parent variable (ParVar) mappings.
     *  @details The map that contains variables to parent variable (ParVar) mappings.
     */
    ParVarMap *parvarmap;

    /*! @var PathEnvironment* penv
     *  @brief The path environment for the variables used inside the query.
     *  @details The path environment for the variables used inside the query.
     */
    PathEnvironment *penv;

    /*! @var VariableTree* vt
     *  @brief The constructed variable tree.
     *  @details The constructed variable tree.
     */
    VariableTree *vt;

    /*! @var vector<DependencySet*>* depset
     *  @brief The vector containing pointers to all DependencySet objects. 
     *  @details The vector containing pointers to all DependencySet objects.
     */
     vector < DependencySet * >*depset;

    /*! @var ProjectionTree* pt
     *  @brief The constructed projection tree used to construct the ProjectionDFA. 
     *  @details The constructed projection tree used to construct the ProjectionDFA.
     */
    ProjectionTree *pt;

    /*! @var PassiveProjectionTree* ppt
     *  @brief The passive projection tree used to construct the ProjectionDFA. 
     *  @details The passive projection tree used to construct the ProjectionDFA.
     */
    PassiveProjectionTree *ppt;

    /*! @var ProjectionDFA* pdfa
     *  @brief The projection DFA used for stream preprojection.
     *  @details The projection DFA used for stream preprojection.
     */
    ProjectionDFA *pdfa;

    /*! @var CmdLine* cmd
     *  @brief The command line argument object that stores the passed command line arguments.
     *  @details The command line argument object that stores the passed command line arguments.
     */
    CmdLine *cmd;
};

#endif // EXECUTOR_H
