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
#include "executor.h"
#include "updateexpression.h"
#include "sequenceexpression.h"
#include "tagmap.h"
#include "forexpression.h"
#include "varexpression.h"
#include "varstepexpression.h"
#include "tagmap.h"
#include "error.h"
#include <sstream>

#ifdef GENERATE_STATS
#include "statistics.h"
#endif

// query file parser
extern int parse_query(const char* query_file);

extern Expression* the_query;


Executor::Executor(const char* _query_file, const char* _xml_file,
        CmdLine* _cmd)
    : query_file(_query_file), xml_file(_xml_file),
    query(NULL), buffer(NULL), fsamap(NULL), penv(NULL),
	pt(NULL), p_nfa(NULL), p_dfa(NULL), cmd(_cmd) {

    if (cmd->print_file_info) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "Query file: '" << query_file << "'" << endl;
        cout << "Stream file: '" << xml_file << "'" << endl;
        cout << "###############################################" << endl;
    }
}

Executor::~Executor() {

	// all this final cleanup principally just wastes time,
	// so for an official version we should comment out
	// all this stuff... nevertheless, its nice for debugging
	// and may be necessary to evaluate multiple queries in order 
		
    if (query!=NULL)
        delete query;
	if (penv!=NULL)
		delete penv;
    if (pt!=NULL)
        delete pt;
    if (p_dfa!=NULL)
        delete p_dfa;
    if (p_nfa!=NULL)
        delete p_nfa;
    if (buffer!=NULL)
        delete buffer;
    delete cmd;

	// singleton class deletion
	delete PrettyPrint::getInstance();
	delete TagMap::getInstance();
	delete VarName::getInstance();
	delete FSAMap::getInstance();
	delete RoleList::getInstance();
	delete TagNodeStack::getInstance();
	#ifdef GENERATE_STATS
	delete Statistics::getInstance();
	#endif // #ifdef GENERATE_STATS
}

/* ATTENTION: ORDER PLAYS A MAJOR ROLE */
void Executor::start() {

    if (!cmd->preprocess_stream_only && !cmd->preprocess_stream_only_no_deb) {

		// query processing
        parseQuery();
        optimizeQuery();
		
		// preprojection preparation
		collectQueryInformation();
        buildProjectionTree();
		buildProjectionNFA();
		buildInitialProjectionDFA();
		
		// query rewriting (signOff insertion)
        rewriteQuery();
	
		// final preparation (buffer setup etc)
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
        buildProjectionTree();
		buildProjectionNFA();
		buildInitialProjectionDFA();
		
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
        cout << "#################### QUERY ####################" << endl;
        cout << "###############################################" << endl;
        cout << *query;
    }
}

void Executor::optimizeQuery() {

    // optimize & print query
    query->mergeSequences();
	if (query->getType()==e_typevarstep) {
		VarStepExpression* sub=((VarStepExpression*)query)->clone();
		delete query;
		VarExpression* tmpvar=
			new VarExpression(VarName::getInstance()->getFreshVarname(),true);
		query=new ForExpression(tmpvar,sub,tmpvar->clone());
	}
	query->rewriteVarsteps();

    if (cmd->print_optimized_query) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "############## Optimized Query ################" << endl;
        cout << "###############################################" << endl;
        query->print(cout);
    }
}

void Executor::collectQueryInformation() {

    fsamap=FSAMap::getInstance();
    query->extractFSAMap(fsamap,0);
    if (cmd->print_fsamap) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "############## FSA COMPUTATION ################" << endl;
        cout << "###############################################" << endl;
        fsamap->print();
    }

	penv=new PathEnvironment();
	query->extractPathEnvironment(penv);
	if (cmd->print_pathenv) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "############## PATH ENVIRONMENT ###############" << endl;
        cout << "###############################################" << endl;
        penv->print();
	}

	// The role list class needs the path environment to compute the
	// relative paths used in the sendUpdate commands
	RoleList::getInstance()->init(penv);
}

void Executor::buildProjectionTree() {

    pt=new ProjectionTree();
    query->extractProjectionTree(pt);
    if (cmd->print_projection_tree) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "################ ProjectionTree ###############" << endl;
        cout << "###############################################" << endl;
        pt->print();
    }
}

void Executor::buildProjectionNFA() {
		
	p_nfa=new ProjectionNFA(pt);
	if (cmd->print_projection_nfa) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "################ ProjectionNFA ################" << endl;
        cout << "###############################################" << endl;
		p_nfa->print();
	}
}

void Executor::buildInitialProjectionDFA() {

	p_dfa=new ProjectionDFA(p_nfa);
	if (cmd->print_projection_dfa) {
		cout << endl << endl;
		cout << "###############################################" << endl;
		cout << "########## ProjectionDFA (initial) ############" << endl;
		cout << "###############################################" << endl;
		p_dfa->print();
	}
}
		
void Executor::rewriteQuery() {

	RoleList* role_list=RoleList::getInstance();
	vector<Role*>* roles=role_list->getRoles();

    if (cmd->print_rewritten_query) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "############### XQ Rewriting ##################" << endl;
        cout << "###############################################" << endl;

        cout << "### ROLES TO INSERT UPDATES FOR ("
			   << roles->size() << ")" << endl;
		for (unsigned i=0; i<roles->size(); i++) {
			(*roles)[i]->print();
			cout << endl;
		}
		cout << endl;
    }
   	
    vector<UpdateExpression*> updates;
    for (unsigned i=0; i<roles->size(); i++) {
	    updates.push_back((*roles)[i]->toUpdateExpression());
	    if (cmd->print_rewritten_query) {
	        cout << "---> " << *(updates[updates.size()-1]) << endl;
	    }
    }

    insertUpdates(updates);
    query->mergeSequences();

    if (cmd->print_rewritten_query) {
        cout << "### REWRITTEN QUERY:" << endl;
        cout << *query;
		cout << endl << endl;
		cout <<
			"!!! ATTENTION: While if-rewriting is made explicit in the paper, the" 
			<< endl <<
			"!!!            implementation does NOT rewrite if-expressions but instead"
		 	<< endl << 
			"!!!            solves this issue internally by a different evaluation strategy.";
    }
}

void Executor::initStreamParser() {
    
    // set up a new stream preprocessor
    // ATTENTION: TO BE DONE BEFORE CALLING saxparse()
    buffer=new Buffer(p_dfa);
    StreamPreProcessor::initInstance(buffer,
										xml_file,
										p_dfa);
    if (cmd->print_initial_buffer) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "################### STREAM ####################" << endl;
        cout << "###############################################" << endl;    
        buffer->debugPrint();
    }
}

void Executor::preprocessStream() {

	cout << endl << endl;
	if (cmd->preprocess_stream_only) {

	    cout << "###############################################" << endl;
	    cout << "################### STREAM ####################" << endl;
	    cout << "###############################################" << endl;    
	    cout << "StreamPreprocessMode... patience please..." << endl << endl;
		StreamPreProcessor::getInstance()->readAll();   
	    buffer->debugPrint();

	} else {
		StreamPreProcessor::getInstance()->readAll();   
	    buffer->print();
	}        
	cout << endl;
}

void Executor::insertUpdates(vector<UpdateExpression*>& updates) {

    // place $root-updates at top level
    for (unsigned i=0; i<updates.size(); i++) {
        if (updates[i]->getVar()==0) {
            SequenceExpression* sexp=
                new SequenceExpression(query,updates[i]);
            query=sexp;
        }
    }

    // distribute remaining update-statements
    query=query->placeUpdates(updates);
}

void Executor::evalQuery() {
    
    Environment* env=new Environment(buffer->getRoot());
    
    if (cmd->someSet()) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "################## RESULT #####################" << endl;
        cout << "###############################################" << endl;
    }
    query->eval(env,Expression::EVAL_QUERY);
    query->eval(env,Expression::EVAL_UPD_IN_SEQ);
	cout << endl;
	
	delete env;
}


void Executor::finalDebugPrint(bool sppmode) {

    if (cmd->print_final_buffer && !sppmode) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "################# FINAL BUFFER ################" << endl;
        cout << "###############################################" << endl;
        buffer->debugPrint();
    }
    
    if (cmd->print_tag_map) {
        cout << endl << endl;
        cout << "###############################################" << endl;
        cout << "############### FINAL TAG MAP #################" << endl;
        cout << "###############################################" << endl;
        TagMap::getInstance()->print();
    }

    if (cmd->print_final_dfa) {
        cout << endl;
        cout << "###############################################" << endl;
        cout << "############ FINAL Projection DFA #############" << endl;
        cout << "###############################################" << endl;
        p_dfa->print();
    }

	if (cmd->print_stats) {
		#ifdef GENERATE_STATS
		Statistics::getInstance()->print();
		#else
		cout << endl;
	    cout << "###############################################" << endl;
    	cout << "################# STATISTICS ##################" << endl;
    	cout << "###############################################" << endl;
		cout << "No statistics available!" << endl;
		cout << "Please recompile using -DGENERATE_STATS" << endl;	
		#endif // GENERATE_STATS
	}
}
