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
#include "statistics.h"

Statistics* Statistics::instance=NULL;

Statistics* Statistics::getInstance() {
	if (instance==NULL)
		instance=new Statistics();

	return instance;
}

Statistics::Statistics()
	: max_tagnodes(0), max_charnodes(0), max_clobnodes(0), max_nodes(0) {}

Statistics::~Statistics() {}

void Statistics::addTagNode() {
	if (shot.size()==0) {
		shot.push_back(Shot(1,0,0));
	} else {
		Shot new_shot(shot[shot.size()-1].tagnode+1,
						shot[shot.size()-1].charnode,
						shot[shot.size()-1].clobnode);
		shot.push_back(new_shot);
	}
}

void Statistics::addCharNode() {
	if (shot.size()==0) {
		shot.push_back(Shot(0,1,0));
	} else {
		Shot new_shot(shot[shot.size()-1].tagnode,
						shot[shot.size()-1].charnode+1,
						shot[shot.size()-1].clobnode);
		shot.push_back(new_shot);
	}
}

void Statistics::addClobNode() {
	
	if (shot.size()==0) {
		shot.push_back(Shot(0,0,1));
	} else {
		Shot new_shot(shot[shot.size()-1].tagnode,
						shot[shot.size()-1].charnode,
						shot[shot.size()-1].clobnode+1);
		shot.push_back(new_shot);
	} // other case should never happen
}

void Statistics::removeTagNode() {
	if (shot.size()!=0) {
		Shot new_shot(shot[shot.size()-1].tagnode-1,
						shot[shot.size()-1].charnode,
						shot[shot.size()-1].clobnode);
		shot.push_back(new_shot);
	} // other case should never happen
}

void Statistics::removeCharNode() {
	if (shot.size()!=0) {
		Shot new_shot(shot[shot.size()-1].tagnode,
						shot[shot.size()-1].charnode-1,
						shot[shot.size()-1].clobnode);
		shot.push_back(new_shot);
	} // other case should never happen
}

void Statistics::removeClobNode() {
	if (shot.size()!=0) {
		Shot new_shot(shot[shot.size()-1].tagnode,
						shot[shot.size()-1].charnode,
						shot[shot.size()-1].clobnode-1);
		shot.push_back(new_shot);
	} // other case should never happen
}


void Statistics::computeStats() {

	for (unsigned i=0; i<shot.size(); i++) {
		if (shot[i].tagnode > max_tagnodes)
			max_tagnodes=shot[i].tagnode;
		if (shot[i].charnode > max_charnodes)
			max_charnodes=shot[i].charnode;
		if (shot[i].clobnode > max_clobnodes)
			max_clobnodes=shot[i].clobnode;
	
		unsigned sum=shot[i].sum();
		if (sum>max_nodes) {
			max_nodes=sum;
		}	
	}
}

void Statistics::print() {

	computeStats();
		
    cout << endl;
    cout << "###############################################" << endl;
    cout << "################# STATISTICS ##################" << endl;
    cout << "###############################################" << endl;
	cout << endl;
	cout << "-----------------------------------------------" << endl;
	cout << "MAX (#Nodes)       =" << max_nodes << endl;
	cout << "-----------------------------------------------" << endl;
	cout << "MAX (#TagNodes)    =" << max_tagnodes << endl;
	cout << "MAX (#CharNodes)   =" << max_charnodes << endl;
	//cout << "MAX (#ClobNodes)   =" << max_clobnodes << endl;
	
}
