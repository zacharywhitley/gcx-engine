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
 *  @brief Implementations of header file executor.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file executor.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 *  @license Software License Agreement (BSD License)
 */
#include "executor.h"

// query file parser
extern int parse_query(const char* query_file);

extern Expression* the_query;

Executor::Executor(const char* _query_file, const char* _xml_file, CmdLine* _cmd) :
query_file(_query_file), xml_file(_xml_file), query(NULL), buffer(NULL),
fsamap(NULL), parvarmap(NULL), penv(NULL), vt(NULL),
depset(new vector<DependencySet*>()), pt(NULL), ppt(NULL), pdfa(NULL), cmd(_cmd) {

    if (cmd->print_file_info) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "Query File: \"" << query_file << "\"" << endl;
        cout << "Stream File: \"" << xml_file << "\"" << endl;
        cout << "###############################################" << endl;
    }
}

Executor::~Executor() {

    if (cmd->debug) cout << endl << "[Executor] Cleaning Up Now..." << endl;
    if (cmd->debug) cout << "Deleting Query..." << endl;
    delete query;
    if (cmd->debug) cout << "Deleting Buffer..." << endl;
    delete buffer;
    if (cmd->debug) cout << "Deleting PathEnvironment..." << endl;
    delete penv;
    if (cmd->debug) cout << "Deleting VariableTree..." << endl;
    delete vt;
    if (cmd->debug) cout << "Deleting DependencySet..." << endl;
    for (unsigned i=0; i<depset->size(); i++) {
        delete (*depset)[i];
    }
    delete depset;
    if (cmd->debug) cout << "Deleting ProjectionTree..." << endl;
    delete pt;
    if (cmd->debug) cout << "Deleting PassiveProjectionTree..." << endl;
    delete ppt;
    if (cmd->debug) cout << "Deleting ProjectionDFA..." << endl;
    delete pdfa;

    // singleton class deletion
    if (cmd->debug) cout << "Deleting Error Object..." << endl;
    delete Error::getInstance();
    if (cmd->debug) cout << "Deleting FSAMap Object..." << endl;
    delete FSAMap::getInstance();
    if (cmd->debug) cout << "Deleting ParVarMap Object..." << endl;
    delete ParVarMap::getInstance();
    if (cmd->debug) cout << "Deleting PrettyPrint Object..." << endl;
    delete PrettyPrint::getInstance();
    if (cmd->debug) cout << "Deleting RoleList Object..." << endl;
    delete RoleList::getInstance();
    if (cmd->debug) cout << "Deleting StreamPreProcessor Object..." << endl;
    delete StreamPreProcessor::getInstance();
    if (cmd->debug) cout << "Deleting TagMap Object..." << endl;
    delete TagMap::getInstance();
    if (cmd->debug) cout << "Deleting VarName Object..." << endl;
    delete VarName::getInstance();  

    if (cmd->debug) cout << "Deleting CmdLine Object..." << endl;
    delete cmd;
}

/* ATTENTION: ORDER PLAYS A MAJOR ROLE */
void Executor::start() {

    if (!cmd->preprocess_stream_only && !cmd->preprocess_stream_only_no_deb) {

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

        // evaluation
        evalQuery();

        finalDebugPrint(false);

        if (cmd->someSet()) {
            cout << endl << "--- ALL DONE" << endl;
        }

    } else {

        // query processing
        parseQuery();
        optimizeQuery();

        // preprojection preparation
        collectQueryInformation();
        buildVariableTree();
        buildDependencies();
        buildProjectionTree();
        initProjectionDFA();

        // final preprocess the stream
        initStreamParser();
        preprocessStream();
        finalDebugPrint(true);
    }
}

void Executor::parseQuery() {

    // parse query
    parse_query(query_file);

    query=the_query;

    if (cmd->print_original_query) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "#################### Query ####################" << endl;
        cout << "###############################################" << endl;
        cout << *(query);
    }

    if (cmd->print_original_varsq) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "################### VARSq #####################" << endl;
        cout << "###############################################" << endl;
        cout << "VARSq = {";
        VarName::getInstance()->print();
        cout << "}" << endl;
    }
}

void Executor::optimizeQuery() {

    // optimize & print query
    query->mergeSequences();
    if (query->getType()==et_varstep) {
        VarStepExpression* sub=((VarStepExpression*)query)->clone();
        delete query;
        VarExpression* tmpvar=new VarExpression(VarName::getInstance()->getFreshVarname(),true,true);
        query=new ForExpression(tmpvar,sub,tmpvar->clone());
    }
    
    query->rewriteAggregateFuncts();

#ifdef REWRITE_VARSTEPS
    query->rewriteVarsteps();
#endif // #ifdef REWRITE_VARSTEPS     
 
    if (cmd->print_optimized_query) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "############### Optimized Query ###############" << endl;
        cout << "###############################################" << endl;
#ifdef REWRITE_VARSTEPS
        cout << *(query);
#else
        cout << "Not Available!" << endl;
        cout << "Please Recompile Enabling -REWRITE_VARSTEPS." << endl;  
#endif // #ifndef REWRITE_VARSTEPS
    }

    if (cmd->print_optimized_varsq) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "################## VARSq* #####################" << endl;
        cout << "###############################################" << endl;
#ifdef REWRITE_VARSTEPS
        cout << "VARSq* = {";
        VarName::getInstance()->print();
        cout << "}" << endl;
#else
        cout << "Not Available!" << endl;
        cout << "Please Recompile Enabling -REWRITE_VARSTEPS." << endl;
#endif // #ifndef REWRITE_VARSTEPS
    }
}

void Executor::collectQueryInformation() {

    fsamap=FSAMap::getInstance();
    query->extractFSAMap(fsamap, TAGID_ROOT);
    if (cmd->print_fsamap) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "############### FSA Computation ###############" << endl;
        cout << "###############################################" << endl;
        fsamap->print();
    }

    parvarmap=ParVarMap::getInstance();
    query->extractParVarMap(parvarmap);
    if (cmd->print_parvarmap) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "############# ParVar Computation ##############" << endl;
        cout << "###############################################" << endl;
        parvarmap->print();
    }

    penv=new PathEnvironment();
    ppt=new PassiveProjectionTree(penv);
    if (cmd->print_pathenv) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "############## Path Environment ###############" << endl;
        cout << "###############################################" << endl;
        penv->print();
    }    

    // The role list class needs the path environment to compute the
    // relative paths used in the SignOff-commands
    RoleList::initInstance(penv);
}

void Executor::buildVariableTree() {

    vt=new VariableTree();
    if (cmd->print_variable_tree) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "################ Variable Tree ################" << endl;
        cout << "###############################################" << endl;
        vt->print();
    }
}

void Executor::buildDependencies() {

    vector<unsigned>* varsq = parvarmap->getVarsq();
    for (unsigned i=0; i<varsq->size(); i++) {
        depset->push_back(new DependencySet((*varsq)[i]));
    }
    
    query->extractDependencies(depset);
    if (cmd->print_original_query_dependencies_wo) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "####### Original Dependencies w/o Roles #######" << endl;
        cout << "###############################################" << endl;
        for (unsigned i=0; i<depset->size(); i++) {
            // if (depset[i]->hasTuples()) { // remove to hide empty dependencies!
            cout << "dep(" << "$"
                    << VarName::getInstance()->getVarname((*depset)[i]->getVar())
                    << ") = {";
            (*depset)[i]->print();
            cout << "}" << endl;
            // }
        }
    }
    
#ifndef NO_OPTIMIZATIONS    
    for (unsigned i=0; i<depset->size(); i++) {
        if ((*depset)[i]->hasTuples()) {
            (*depset)[i]->removeSyntacticallyEqualTuple(ppt);
        }
    }
    
    for (unsigned i=0; i<depset->size(); i++) {
        if ((*depset)[i]->hasTuples()) {
            (*depset)[i]->removeSemanticallyContainedTuple(ppt);
        }
    }
#endif // #ifndef NO_OPTIMIZATIONS     

    if (cmd->print_optimized_query_dependencies_wo) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "###### Optimized Dependencies w/o Roles #######" << endl;
        cout << "###############################################" << endl;
#ifndef NO_OPTIMIZATIONS        
        for (unsigned i=0; i<depset->size(); i++) {
            // if (depset[i]->hasTuples()) { // remove to hide empty dependencies!
            cout << "dep(" << "$" << VarName::getInstance()->getVarname((*depset)[i]->getVar()) << ") = {";
            (*depset)[i]->print();
            cout << "}" << endl;
            // }
        }
#else
        cout << "Not Available!" << endl;
        cout << "Please Recompile Disabling -DNO_OPTIMIZATIONS." << endl;
#endif // #ifndef NO_OPTIMIZATIONS     
    }
    
}

void Executor::buildProjectionTree() {

    pt=new ProjectionTree(vt, depset);    
    if (cmd->print_optimized_query_dependencies_w) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "### Original/Optimized Dependencies w/ Roles ##" << endl;
        cout << "###############################################" << endl;
        for (unsigned i=0; i<depset->size(); i++) {
            // if (depset[i]->hasTuples()) { // remove to hide empty dependencies!
            cout << "dep(" << "$"
                << VarName::getInstance()->getVarname((*depset)[i]->getVar()) << ") = {";
            (*depset)[i]->print();
            cout << "}" << endl;
            // }
        }
    }
    
    if (cmd->print_original_projection_tree) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "########### Original ProjectionTree ###########" << endl;
        cout << "###############################################" << endl;
        pt->print();
    }
    
#ifndef NO_OPTIMIZATIONS
     pt->removeUnneededNodes(ppt);
     pt->removeRedundantRoles(fsamap,ppt);
#endif // #ifndef NO_OPTIMIZATIONS     
    
	if (cmd->print_passive_projection_tree) {
		cout << endl << endl;
		cout << "###############################################" << endl;
		cout << "############ Passive ProjectionTree ###########" << endl;
		cout << "###############################################" << endl;
 #ifndef NO_OPTIMIZATIONS        
		ppt->print();
 #else
		cout << "Not Available!" << endl;
		cout << "Please Recompile Disabling -DNO_OPTIMIZATIONS." << endl;
 #endif // #ifndef NO_OPTIMIZATIONS
     } 
     
    if (cmd->print_optimized_projection_tree) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "########## Optimized ProjectionTree ###########" << endl;
        cout << "###############################################" << endl;
#ifndef NO_OPTIMIZATIONS        
        pt->print();
#else
        cout << "Not Available!" << endl;
        cout << "Please Recompile Disabling -DNO_OPTIMIZATIONS." << endl;
#endif // #ifndef NO_OPTIMIZATIONS
    }  

    // call once AFTER all initializations have been applied
    pt->labelTree();
    if (cmd->print_projection_tree_labels) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "############ ProjectionTree Labels ############" << endl;
        cout << "###############################################" << endl;
        pt->getLabels()->print();
    }
}

void Executor::buildSignOffQuery() {

    RoleList* role_list=RoleList::getInstance();
    vector<Role*>* roles=role_list->getRoles();

    if (cmd->print_rewritten_query) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "######## SignOff Query (XQ Rewriting) #########" << endl;
        cout << "###############################################" << endl;

        cout << "### ROLES TO INSERT SIGNOFFS FOR (" << roles->size() << ")" << endl;
        for (unsigned i=0; i<roles->size(); i++) {
            (*roles)[i]->printExtended();
            cout << endl;
        }
        cout << endl;
    }

	role_list->sortRolesForSignOffs();

    if (cmd->print_rewritten_query) {
    	cout << "### SORTED ROLES FOR SIGNOFFS" << endl;
        for (unsigned i=0; i<roles->size(); i++) {
            (*roles)[i]->printExtended();
            cout << endl;
        }
        cout << endl;
    }
    
    vector<SignOffExpression*> signoffs;
    for (unsigned i=0; i<roles->size(); i++) {
        signoffs.push_back((*roles)[i]->getSignOffExpression());
        if (cmd->print_rewritten_query) {
            cout << "---> " << *(signoffs[signoffs.size()-1]) << endl;
        }
    }

    insertSignOffs(signoffs);
    query->mergeSequences();

    if (cmd->print_rewritten_query) {
        cout << endl;
        cout << "### REWRITTEN QUERY:" << endl;
        cout << *(query);
        cout << endl << endl;
        cout << "****************************************"
                << "***************************************"
                << endl
                << "ATTENTION-IMPORTANT-ATTENTION-IMPORTANT-"
                << "ATTENTION-IMPORTANT-ATTENTION-IMPORTANT"
                << endl
                << "****************************************"
                << "***************************************"
                << endl
                << "!!!    While rewriting if-expressions is made explicit "
                << "in the paper, the    !!!"
                << endl
                << "!!!    implementation does NOT rewrite if-expressions but "
                << "instead solves    !!!"
                << endl
                << "!!!    this issue internally by a different evaluation "
                << "strategy.            !!!"
                << endl
                << "****************************************"
                << "***************************************" 
                << endl
                << "ATTENTION-IMPORTANT-ATTENTION-IMPORTANT-"
                << "ATTENTION-IMPORTANT-ATTENTION-IMPORTANT"
                << endl
                << "****************************************"
                << "***************************************"
                << endl;
    }
}

void Executor::insertSignOffs(vector<SignOffExpression*>& signoffs) {

    // place $root-signoffs at top level
    for (unsigned i=0; i<signoffs.size(); i++) {
        if (signoffs[i]->getVar()==TAGID_ROOT) {
            SequenceExpression* sexp= new SequenceExpression(query,signoffs[i]);
            query=sexp;
        }
    }

    // distribute remaining signoff commands
    query=query->placeSignOffs(signoffs);
}

void Executor::initProjectionDFA() {
	
    pdfa=new ProjectionDFA(pt,ppt);
    if (cmd->print_projection_dfa) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "########### Initial Projection DFA  ###########" << endl;
        cout << "###############################################" << endl;
        pdfa->print();
    }
}

void Executor::initStreamParser() {

    // set up a new stream preprocessor
    // ATTENTION: TO BE DONE BEFORE CALLING saxparse()
    buffer=new Buffer(pdfa);
    StreamPreProcessor::initInstance(buffer, xml_file, pdfa);
    if (cmd->print_initial_buffer) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "################### Stream ####################" << endl;
        cout << "###############################################" << endl;
        buffer->debugPrint();
    }
}

void Executor::preprocessStream() {

    cout << endl << endl;
    if (cmd->preprocess_stream_only) {
        cout << "###############################################" << endl;
        cout << "################### Stream ####################" << endl;
        cout << "###############################################" << endl;
        cout << "StreamPreprocessMode... Patience Please..." << endl << endl;
        StreamPreProcessor::getInstance()->readAll();
        buffer->debugPrint();

    } else {
        StreamPreProcessor::getInstance()->readAll();
        buffer->print();
    }
    cout << endl;
}

void Executor::evalQuery() {

    Environment* env=new Environment(buffer->getRoot());
    if (cmd->someSet()) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "################### Result ####################" << endl;
        cout << "###############################################" << endl;
    }
    query->eval(env,EVAL_QUERY);
    buffer->getRoot()->clear();
    cout << endl;
    
    delete env;
}

void Executor::finalDebugPrint(bool sppmode) {
    
    if (cmd->print_final_buffer && !sppmode) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "################ Final Buffer #################" << endl;
        cout << "###############################################" << endl;
        buffer->debugPrint();
    }

    if (cmd->print_tag_map) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "############## Final Tagname Map ##############" << endl;
        cout << "###############################################" << endl;
        TagMap::getInstance()->print();
    }

    if (cmd->print_final_dfa) {
        cout << endl;
        cout << "###############################################" << endl;
        cout << "############ Final Projection DFA #############" << endl;
        cout << "###############################################" << endl;
        pdfa->print();
    }
}
