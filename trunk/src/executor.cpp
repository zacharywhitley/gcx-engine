
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
 *  @brief Implementations of header file executor.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file executor.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "executor.h"

/*! @fn extern int parse_query(DocExpression* _doc)
 *  @brief Input (X)Query parsing.
 *  @details Input (X)Query parsing.
 *  @param[in] _doc Pointer to a DocExpression object.
 *  @retval int <tt>0</tt> if input (X)Query has been parsed correctly,
 *   	<tt>1</tt> otherwise.
 */
extern int parse_query(DocExpression * _doc);

/*! @var extern Expression* parsed_query
 *  @brief Parsed (X)Query input query.
 *  @details Parsed (X)Query input query.
 */
extern Expression *parsed_query;

Executor::Executor(CmdLine * _cmd):
debug_ostream(NULL), eval_ostream(NULL), query(NULL),
buffer(NULL), fsamap(NULL), parvarmap(NULL), penv(NULL),
vt(NULL), depset(new vector < DependencySet * >()), pt(NULL),
ppt(NULL), pdfa(NULL), cmd(_cmd) {
}

Executor::~Executor() {

    bool debug_ostream_write = debug_ostream && debug_ostream->isWriteable();

    if (cmd->debug) {
        if (debug_ostream_write) {
            (*debug_ostream) << NEWLINE << "[Executor] Cleaning Up Now..." <<
                NEWLINE;
        } else {
            cout << endl << "[Executor] Cleaning Up Now..." << endl;
        }
    }
    if (cmd->debug) {
        if (debug_ostream_write) {
            (*debug_ostream) << "Deleting Query..." << NEWLINE;
        } else {
            cout << "Deleting Query..." << endl;
        }
    }
    delete query;

    if (cmd->debug) {
        if (debug_ostream_write) {
            (*debug_ostream) << "Deleting Buffer..." << NEWLINE;
        } else {
            cout << "Deleting Buffer..." << endl;
        }
    }
    delete buffer;

    if (cmd->debug) {
        if (debug_ostream_write) {
            (*debug_ostream) << "Deleting PathEnvironment..." << NEWLINE;
        } else {
            cout << "Deleting PathEnvironment..." << endl;
        }
    }
    delete penv;

    if (cmd->debug) {
        if (debug_ostream_write) {
            (*debug_ostream) << "Deleting VariableTree..." << NEWLINE;
        } else {
            cout << "Deleting VariableTree..." << endl;
        }
    }
    delete vt;

    if (cmd->debug) {
        if (debug_ostream_write) {
            (*debug_ostream) << "Deleting DependencySet..." << NEWLINE;
        } else {
            cout << "Deleting DependencySet..." << endl;
        }
    }
    for (unsigned i = 0; i < depset->size(); i++) {
        delete(*depset)[i];
    }
    delete depset;

    if (cmd->debug) {
        if (debug_ostream_write) {
            (*debug_ostream) << "Deleting ProjectionTree..." << NEWLINE;
        } else {
            cout << "Deleting ProjectionTree..." << endl;
        }
    }
    delete pt;

    if (cmd->debug) {
        if (debug_ostream_write) {
            (*debug_ostream) << "Deleting PassiveProjectionTree..." << NEWLINE;
        } else {
            cout << "Deleting PassiveProjectionTree..." << endl;
        }
    }
    delete ppt;

    if (cmd->debug) {
        if (debug_ostream_write) {
            (*debug_ostream) << "Deleting ProjectionDFA..." << NEWLINE;
        } else {
            cout << "Deleting ProjectionDFA..." << endl;
        }
    }
    delete pdfa;

    // singleton class deletion
    if (cmd->debug) {
        if (debug_ostream_write) {
            (*debug_ostream) << "Deleting FSAMap Object..." << NEWLINE;
        } else {
            cout << "Deleting FSAMap Object..." << endl;
        }
    }
    delete FSAMap::getInstance();

    if (cmd->debug) {
        if (debug_ostream_write) {
            (*debug_ostream) << "Deleting ParVarMap Object..." << NEWLINE;
        } else {
            cout << "Deleting ParVarMap Object..." << endl;
        }
    }
    delete ParVarMap::getInstance();

    if (cmd->debug) {
        if (debug_ostream_write) {
            (*debug_ostream) << "Deleting RoleList Object..." << NEWLINE;
        } else {
            cout << "Deleting RoleList Object..." << endl;
        }
    }
    delete RoleList::getInstance();

    if (cmd->debug) {
        if (debug_ostream_write) {
            (*debug_ostream) << "Deleting StreamPreProcessor Object..." <<
                NEWLINE;
        } else {
            cout << "Deleting StreamPreProcessor Object..." << endl;
        }
    }
    delete StreamPreProcessor::getInstance();

    if (cmd->debug) {
        if (debug_ostream_write) {
            (*debug_ostream) << "Deleting TagMap Object..." << NEWLINE;
        } else {
            cout << "Deleting TagMap Object..." << endl;
        }
    }
    delete TagMap::getInstance();

    if (cmd->debug) {
        if (debug_ostream_write) {
            (*debug_ostream) << "Deleting VarName Object..." << NEWLINE;
        } else {
            cout << "Deleting VarName Object..." << endl;
        }
    }
    delete VarName::getInstance();

    if (cmd->debug) {
        if (debug_ostream_write) {
            (*debug_ostream) << "Deleting CmdLine and StreamManager Object..."
                << NEWLINE;
        } else {
            cout << "Deleting CmdLine and StreamManager Object..." << endl;
        }
    }
    delete cmd;
    delete StreamManager::getInstance();


}

// ATTENTION: ORDER PLAYS A MAJOR ROLE
void Executor::start() {

    if (!cmd->preprocess_stream_debug && !cmd->preprocess_stream_no_debug) {

        // stream initialization
        try {
            initStreams();
        }
        catch(IOStreamException & e) {
            cout << e.getDebugMsg();
            e.terminate();
        }
        catch(Exception & e) {
            cout << e.getDebugMsg();
            e.terminate();
        }

        try {
            // query processing
            parseQuery();
            optimizeQuery();

            // preprojection preparation
            collectQueryInformation();
            buildVariableTree();
            buildDependencies();
            buildProjectionTree();

            // query rewriting (signOff insertion)
            buildSignOffQuery();

            // stream preprojector setup
            initProjectionDFA();
            initStreamParser();

            // if xml input stream is null, don't even start evaluation
            if (StreamManager::getInstance()->getXMLInputStream()->
                getType() == it_null)
                return;

            evalQuery();
            finalDebugPrint(false);

        }
        catch(IOStreamException & e) {
            (*debug_ostream) << e.getDebugMsg();
            e.terminate();
        }
        catch(ParseException & e) {
            (*debug_ostream) << e.getDebugMsg();
            e.terminate();
        }
        catch(RuntimeException & e) {
            (*debug_ostream) << e.getDebugMsg();
            e.terminate();
        }
        catch(InvalidCastException & e) {
            (*debug_ostream) << e.getDebugMsg();
            e.terminate();
        }
        catch(Exception & e) {
            (*debug_ostream) << e.getDebugMsg();
            e.terminate();
        }

    } else {

        // stream initialization
        try {
            initStreams();
        }
        catch(IOStreamException & e) {
            cout << e.getDebugMsg();
            e.terminate();
        }
        catch(Exception & e) {
            cout << e.getDebugMsg();
            e.terminate();
        }

        try {
            // query processing
            parseQuery();
            optimizeQuery();

            // preprojection preparation
            collectQueryInformation();
            buildVariableTree();
            buildDependencies();
            buildProjectionTree();

            // stream preprojector setup
            initProjectionDFA();
            initStreamParser();

            // if xml input stream is null, don't even start stream processing
            if (StreamManager::getInstance()->getXMLInputStream()->
                getType() == it_null)
                return;

            // preprocess the stream
            preprocessStream();
            finalDebugPrint(true);

        }
        catch(IOStreamException & e) {
            (*debug_ostream) << e.getDebugMsg();
            e.terminate();
        }
        catch(ParseException & e) {
            (*debug_ostream) << e.getDebugMsg();
            e.terminate();
        }
        catch(RuntimeException & e) {
            (*debug_ostream) << e.getDebugMsg();
            e.terminate();
        }
        catch(InvalidCastException & e) {
            (*debug_ostream) << e.getDebugMsg();
            e.terminate();
        }
        catch(Exception & e) {
            (*debug_ostream) << e.getDebugMsg();
            e.terminate();
        }
    }
}

void Executor::initStreams() {

    InputStream *query_istream = NULL;
    InputStream *xml_istream = NULL;

    switch (cmd->query_istream_type) {
        case it_file:
            query_istream = new FileInputStream(cmd->query_arg);
            break;
        case it_null:
            query_istream = new NullInputStream();
            break;
        case it_socket:
            query_istream = new SocketInputStream();
            break;
        case it_stdin:
            query_istream = new StdinInputStream();
            break;
    }

    query_istream->open();
    if (!query_istream->isReadable()) {
        throw IOStreamException("Query Input Stream is Not Readable",
                                eid_stream_input);
    }

    switch (cmd->xml_istream_type) {
        case it_file:
            xml_istream = new FileInputStream(cmd->xml_arg);
            break;
        case it_null:
            xml_istream = new NullInputStream();
            break;
        case it_socket:
            xml_istream = new SocketInputStream();
            break;
        case it_stdin:
            xml_istream = new StdinInputStream();
            break;
    }

    // DO NOT OPEN XML INPUT STREAM HERE
    // (BECAUSE OF DUAL MODE WITH fn:doc())

    switch (cmd->dout_ostream_type) {
        case ot_file:
            debug_ostream = new FileOutputStream(cmd->dout_arg);
            break;
        case ot_null:
            debug_ostream = new NullOutputStream();
            break;
        case ot_socket:
            debug_ostream = new SocketOutputStream();
            break;
        case ot_stdout:
            debug_ostream = new StdoutOutputStream();
            break;
    }

    debug_ostream->open();
    if (!debug_ostream->isWriteable()) {
        throw IOStreamException("Debug Output Stream is Not Writeable",
                                eid_stream_output);
    }

    switch (cmd->eout_ostream_type) {
        case ot_file:
            eval_ostream = new FileOutputStream(cmd->eout_arg);
            break;
        case ot_null:
            eval_ostream = new NullOutputStream();
            break;
        case ot_socket:
            eval_ostream = new SocketOutputStream();
            break;
        case ot_stdout:
            eval_ostream = new StdoutOutputStream();
            break;
    }

    eval_ostream->open();
    if (!eval_ostream->isWriteable()) {
        throw IOStreamException("Result Output Stream is Not Writeable",
                                eid_stream_output);
    }

    StreamManager::initInstance(query_istream, xml_istream, debug_ostream,
                                eval_ostream);
}

void Executor::parseQuery() {

    DocExpression *doc = new DocExpression();

    parse_query(doc);

    query = parsed_query;

    if (!doc->isEmptyPath()) {
        StreamManager::getInstance()->getXMLInputStream()->setArg(doc->
                                                                  getPath());
    }

    StreamManager::getInstance()->getXMLInputStream()->open();
    if (!StreamManager::getInstance()->getXMLInputStream()->isReadable()) {
        throw IOStreamException("XML Input Stream is Not Readable",
                                eid_stream_input);
    }

    if (doc->isEmptyPath()) {
        doc->
            setPath(strdup
                    (StreamManager::getInstance()->getXMLInputStream()->
                     getArg()));
    }

    if (cmd->debug) {
        (*debug_ostream) << NEWLINE << NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        (*debug_ostream) << "############## Stream Information #############" <<
            NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        StreamManager::getInstance()->print();
    }

    if (cmd->debug) {
        (*debug_ostream) << NEWLINE << NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        (*debug_ostream) << "#################### Query ####################" <<
            NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        (*debug_ostream) << (*query) << NEWLINE;
    }

    if (cmd->debug) {
        (*debug_ostream) << NEWLINE << NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        (*debug_ostream) << "################### VARSq #####################" <<
            NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        (*debug_ostream) << "VARSq = {";
        VarName::getInstance()->print((*debug_ostream));
        (*debug_ostream) << "}" << NEWLINE;
    }
}

void Executor::optimizeQuery() {

    // ATTENTION: ORDER OF REWRITING IS IMPORTANT AND SHOULD NOT BE CHANGED
    query->rewriteWhereExps();
    query->rewriteEmptyFuncts();
    query->rewriteVarstepCondExps();
    query->rewriteAggregateFuncts();
    query->mergeSequences();

    if (cmd->debug) {
        (*debug_ostream) << NEWLINE << NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        (*debug_ostream) << "############# Preprocessed Query ##############" <<
            NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        (*debug_ostream) << (*query) << NEWLINE;
    }
#ifdef REWRITE_VARSTEPS
    query->rewriteVarsteps();
#endif // #ifdef REWRITE_VARSTEPS

    if (cmd->debug) {
        (*debug_ostream) << NEWLINE << NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        (*debug_ostream) << "############### Optimized Query ###############" <<
            NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
#ifndef NO_OPTIMIZATIONS
        (*debug_ostream) << (*query) << NEWLINE;
#else
        (*debug_ostream) << "Not Available!" << NEWLINE;
        (*debug_ostream) << "Please Recompile Disabling -DNO_OPTIMIZATIONS." <<
            NEWLINE;
#endif // #ifndef NO_OPTIMIZATIONS
    }

    if (cmd->debug) {
        (*debug_ostream) << NEWLINE << NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        (*debug_ostream) << "################## VARSq* #####################" <<
            NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
#ifndef NO_OPTIMIZATIONS
        (*debug_ostream) << "VARSq* = {";
        VarName::getInstance()->print((*debug_ostream));
        (*debug_ostream) << "}" << NEWLINE;
#else
        (*debug_ostream) << "Not Available!" << NEWLINE;
        (*debug_ostream) << "Please Recompile Disabling -DNO_OPTIMIZATIONS." <<
            NEWLINE;
#endif // #ifndef NO_OPTIMIZATIONS
    }
}

void Executor::collectQueryInformation() {

    fsamap = FSAMap::getInstance();
    query->extractFSAMap(fsamap, TAGID_ROOT);
    if (cmd->debug) {
        (*debug_ostream) << NEWLINE << NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        (*debug_ostream) << "############### FSA Computation ###############" <<
            NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        fsamap->print((*debug_ostream));
    }

    parvarmap = ParVarMap::getInstance();
    query->extractParVarMap(parvarmap);
    if (cmd->debug) {
        (*debug_ostream) << NEWLINE << NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        (*debug_ostream) << "############# ParVar Computation ##############" <<
            NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        parvarmap->print((*debug_ostream));
    }

    penv = new PathEnvironment();
    ppt = new PassiveProjectionTree(penv);
    if (cmd->debug) {
        (*debug_ostream) << NEWLINE << NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        (*debug_ostream) << "############## Path Environment ###############" <<
            NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        penv->print((*debug_ostream));
    }
    // The role list class needs the path environment to compute the
    // relative paths used in the signOff-statements
    RoleList::initInstance(penv);
}

void Executor::buildVariableTree() {

    vt = new VariableTree();
    if (cmd->debug) {
        (*debug_ostream) << NEWLINE << NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        (*debug_ostream) << "################ Variable Tree ################" <<
            NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        vt->print((*debug_ostream));
    }
}

void Executor::buildDependencies() {

    vector < unsigned >*varsq = parvarmap->getVarsq();

    for (unsigned i = 0; i < varsq->size(); i++) {
        depset->push_back(new DependencySet((*varsq)[i]));
    }

    query->extractDependencies(depset);
    if (cmd->debug) {
        (*debug_ostream) << NEWLINE << NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        (*debug_ostream) << "####### Original Dependencies w/o Roles #######" <<
            NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        for (unsigned i = 0; i < depset->size(); i++) {
            // if (depset[i]->hasTuples()) { // remove to hide empty dependencies!
            (*debug_ostream) << "dep(" << "$"
                << VarName::getInstance()->getVarname((*depset)[i]->getVar())
                << ") = {";
            (*depset)[i]->print((*debug_ostream));
            (*debug_ostream) << "}" << NEWLINE;
            // }
        }
    }
#ifndef NO_OPTIMIZATIONS
    for (unsigned i = 0; i < depset->size(); i++) {
        if ((*depset)[i]->hasTuples()) {
            (*depset)[i]->removeSyntacticallyEqualTuple(ppt);
        }
    }

    for (unsigned i = 0; i < depset->size(); i++) {
        if ((*depset)[i]->hasTuples()) {
            (*depset)[i]->removeSemanticallyContainedTuple(ppt);
        }
    }
#endif // #ifndef NO_OPTIMIZATIONS

    if (cmd->debug) {
        (*debug_ostream) << NEWLINE << NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        (*debug_ostream) << "###### Optimized Dependencies w/o Roles #######" <<
            NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
#ifndef NO_OPTIMIZATIONS
        for (unsigned i = 0; i < depset->size(); i++) {
            // if (depset[i]->hasTuples()) { // remove to hide empty dependencies!
            (*debug_ostream) << "dep(" << "$" << VarName::getInstance()->
                getVarname((*depset)[i]->getVar()) << ") = {";
            (*depset)[i]->print((*debug_ostream));
            (*debug_ostream) << "}" << NEWLINE;
            // }
        }
#else
        (*debug_ostream) << "Not Available!" << NEWLINE;
        (*debug_ostream) << "Please Recompile Disabling -DNO_OPTIMIZATIONS." <<
            NEWLINE;
#endif // #ifndef NO_OPTIMIZATIONS
    }
}

void Executor::buildProjectionTree() {

    pt = new ProjectionTree(vt, depset);
    if (cmd->debug) {
        (*debug_ostream) << NEWLINE << NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        (*debug_ostream) << "### Original/Optimized Dependencies w/ Roles ##" <<
            NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        for (unsigned i = 0; i < depset->size(); i++) {
            // if (depset[i]->hasTuples()) { // remove to hide empty dependencies!
            (*debug_ostream) << "dep(" << "$"
                << VarName::getInstance()->getVarname((*depset)[i]->
                                                      getVar()) << ") = {";
            (*depset)[i]->print((*debug_ostream));
            (*debug_ostream) << "}" << NEWLINE;
            // }
        }
    }

    if (cmd->debug) {
        (*debug_ostream) << NEWLINE << NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        (*debug_ostream) << "########### Original ProjectionTree ###########" <<
            NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        pt->print((*debug_ostream));
    }
#ifndef NO_OPTIMIZATIONS
    pt->removeUnneededNodes(ppt);
    pt->removeRedundantRoles(ppt);
#endif // #ifndef NO_OPTIMIZATIONS

    if (cmd->debug) {
        (*debug_ostream) << NEWLINE << NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        (*debug_ostream) << "########## Optimized ProjectionTree ###########" <<
            NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
#ifndef NO_OPTIMIZATIONS
        pt->print((*debug_ostream));
#else
        (*debug_ostream) << "Not Available!" << NEWLINE;
        (*debug_ostream) << "Please Recompile Disabling -DNO_OPTIMIZATIONS." <<
            NEWLINE;
#endif // #ifndef NO_OPTIMIZATIONS
    }

    if (cmd->debug) {
        (*debug_ostream) << NEWLINE << NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        (*debug_ostream) << "############ PassiveProjectionTree ############" <<
            NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
#ifndef NO_OPTIMIZATIONS
        ppt->print((*debug_ostream));
#else
        (*debug_ostream) << "Not Available!" << NEWLINE;
        (*debug_ostream) << "Please Recompile Disabling -DNO_OPTIMIZATIONS." <<
            NEWLINE;
#endif // #ifndef NO_OPTIMIZATIONS
    }
    // call once AFTER all initializations have been applied
    pt->labelTree();
    if (cmd->debug) {
        (*debug_ostream) << NEWLINE << NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        (*debug_ostream) << "############ ProjectionTree Labels ############" <<
            NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        pt->getLabels()->print((*debug_ostream));
    }
}

void Executor::buildSignOffQuery() {

    RoleList *role_list = RoleList::getInstance();

    vector < Role * >*roles = role_list->getRoles();

    if (cmd->debug) {
        (*debug_ostream) << NEWLINE << NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        (*debug_ostream) << "######## SignOff Query (XQ Rewriting) #########" <<
            NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;

        (*debug_ostream) << "### ROLES TO INSERT SIGNOFFS FOR (" << roles->
            size() << ")" << NEWLINE;
        for (unsigned i = 0; i < roles->size(); i++) {
            (*roles)[i]->printExtended((*debug_ostream));
            (*debug_ostream) << NEWLINE;
        }
        (*debug_ostream) << NEWLINE;
    }

    role_list->sortRolesForSignOffs();

    if (cmd->debug) {
        (*debug_ostream) << "### SORTED ROLES FOR SIGNOFFS" << NEWLINE;
        for (unsigned i = 0; i < roles->size(); i++) {
            (*roles)[i]->printExtended((*debug_ostream));
            (*debug_ostream) << NEWLINE;
        }
        (*debug_ostream) << NEWLINE;
    }

    vector < SignOffExpression * >signoffs;
    for (unsigned i = 0; i < roles->size(); i++) {
        signoffs.push_back((*roles)[i]->getSignOffExpression());
        if (cmd->debug) {
            (*debug_ostream) << "---> " << *(signoffs[signoffs.size() - 1]) <<
                NEWLINE;
        }
    }

    insertSignOffs(signoffs);
    query->mergeSequences();

    if (cmd->debug) {
        (*debug_ostream) << NEWLINE;
        (*debug_ostream) << "### REWRITTEN QUERY:" << NEWLINE;
        (*debug_ostream) << (*query);
        (*debug_ostream) << NEWLINE << NEWLINE;
        (*debug_ostream) << "****************************************"
            << "***************************************"
            << NEWLINE
            << "ATTENTION-IMPORTANT-ATTENTION-IMPORTANT-"
            << "ATTENTION-IMPORTANT-ATTENTION-IMPORTANT"
            << NEWLINE
            << "****************************************"
            << "***************************************"
            << NEWLINE
            << "!!!    While rewriting if-expressions is made explicit "
            << "in the paper, the    !!!"
            << NEWLINE
            << "!!!    implementation does NOT rewrite if-expressions but "
            << "instead solves    !!!"
            << NEWLINE
            << "!!!    this issue internally by a different evaluation "
            << "strategy.            !!!"
            << NEWLINE
            << "****************************************"
            << "***************************************"
            << NEWLINE
            << "ATTENTION-IMPORTANT-ATTENTION-IMPORTANT-"
            << "ATTENTION-IMPORTANT-ATTENTION-IMPORTANT"
            << NEWLINE
            << "****************************************"
            << "***************************************" << NEWLINE;
    }
}

void Executor::insertSignOffs(vector < SignOffExpression * >&signoffs) {

    // place $root-signOffs at top level
    for (unsigned i = 0; i < signoffs.size(); i++) {
        if (signoffs[i]->getVar() == TAGID_ROOT) {
            SequenceExpression *sexp =
                new SequenceExpression(query, signoffs[i]);
            query = sexp;
        }
    }

    // distribute remaining signOff commands
    query = query->placeSignOffs(signoffs);
}

void Executor::initProjectionDFA() {

    pdfa = new ProjectionDFA(pt, ppt);
    if (cmd->debug) {
        (*debug_ostream) << NEWLINE << NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        (*debug_ostream) << "########### Initial Projection DFA  ###########" <<
            NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        pdfa->print((*debug_ostream));
    }
}

void Executor::initStreamParser() {

    // ATTENTION: TO BE DONE BEFORE CALLING saxparse()
    buffer = new Buffer(pdfa);
    StreamPreProcessor::initInstance(buffer, pdfa);

    if (cmd->debug) {
        (*debug_ostream) << NEWLINE << NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        (*debug_ostream) << "################### Stream ####################" <<
            NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        buffer->debugPrint((*debug_ostream));
    }
}

void Executor::preprocessStream() {

    (*debug_ostream) << NEWLINE << NEWLINE;
    if (cmd->debug) {
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        (*debug_ostream) << "################### Stream ####################" <<
            NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        (*debug_ostream) << "StreamPreprocessMode... Patience Please..." <<
            NEWLINE << NEWLINE;
        StreamPreProcessor::getInstance()->readAll();
        buffer->debugPrint((*debug_ostream));
    } else {
        StreamPreProcessor::getInstance()->readAll();
        buffer->print((*debug_ostream));
    }
    (*debug_ostream) << NEWLINE;
}

void Executor::evalQuery() {

    Environment *env = new Environment(buffer->getRoot());

    if (cmd->debug) {
        (*debug_ostream) << NEWLINE << NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        (*debug_ostream) << "################### Result ####################" <<
            NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        if (debug_ostream->getType() != ot_stdout
            || eval_ostream->getType() != ot_stdout) {
            (*debug_ostream) << "Result Output Stream Redirected To ";
            switch (eval_ostream->getType()) {
                case ot_file:
                    (*debug_ostream) << "FILE";
                    break;
                case ot_null:
                    (*debug_ostream) << "NULL";
                    break;
                case ot_socket:
                    (*debug_ostream) << "SOCKET";
                    break;
                case ot_stdout:
                    (*debug_ostream) << "STDOUT";
                    break;
            }
            if (eval_ostream->getArg()) {
                (*debug_ostream) << ": \"" << eval_ostream->getArg() << "\"";
            }
        }
    }

    query->init(buffer->getRoot());
    query->eval((*eval_ostream), env, EVAL_QUERY);
    buffer->getRoot()->clear();
    (*debug_ostream) << NEWLINE;

    delete env;
}

void Executor::finalDebugPrint(bool sppmode) {

    if (cmd->debug && !sppmode) {
        (*debug_ostream) << NEWLINE << NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        (*debug_ostream) << "################ Final Buffer #################" <<
            NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        buffer->debugPrint((*debug_ostream));
    }

    if (cmd->debug) {
        (*debug_ostream) << NEWLINE << NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        (*debug_ostream) << "############## Final Tagname Map ##############" <<
            NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        TagMap::getInstance()->print((*debug_ostream));
    }

    if (cmd->debug) {
        (*debug_ostream) << NEWLINE << NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        (*debug_ostream) << "############ Final Projection DFA #############" <<
            NEWLINE;
        (*debug_ostream) << "###############################################" <<
            NEWLINE;
        pdfa->print((*debug_ostream));
    }
}
