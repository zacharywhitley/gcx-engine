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
#include "streamtracker.h"

StreamTracker::StreamTracker(ProjectionDFA* _p_dfa,
								Buffer* _buffer) :
	p_dfa(_p_dfa), cur(p_dfa->getInitialState()), buffer(_buffer), 
	tagmap(TagMap::getInstance()), tns(TagNodeStack::getInstance()) {
}

StreamTracker::~StreamTracker() {
}

void StreamTracker::processOpeningTag(const char* _tag) {

	//cout << "[OPENING TAG '" << _tag << "']" << endl;

	if (cur->outOfBounds()) {
		cur->increaseDepth();
	} else {
		TAG tag=tagmap->insertTag(_tag);
		cur=cur->takeTransition(tag);

		if (!cur->outOfBounds()) {

			switch (cur->getType()) {

				case e_dom:
				case e_out:
					// node that we will mark the node as output when reading
					// the closing tag, in order to avoid buffer manipulation
					if (!cur->isEcOptimizable() || !cur->ecNodeOutput()) {
						buffer->appendTag(tag);
						buffer->appendIndices(cur->getCumulativeIndices(),
												cur->getNoncumulativeIndices());
					} // else node will be ignored
					break;

				#ifdef USE_CLOBS
				case e_clob:

					buffer->appendCLOB(tag);
					tns->addTag(tag);
					break;
				#endif // USE_CLOBS

				default:
					break;
			}
		}
	}	
	//cout << endl;
	//buffer->debugPrint();
	//cout << endl;
	//cout << "[/OPENING TAG '" << _tag << "']" << endl;
	//cout << endl;
	//p_dfa->print();
	//cout << endl;
	//cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
}

void StreamTracker::processClosingTag() {

  	//cout << "[CLOSING TAG]" << endl;
	if (!cur->outOfBounds()) {
			
		switch (cur->getType()) { 

			case e_dom:
			case e_out:
				if (!cur->isEcOptimizable()) {
					buffer->closeTag();
					break;
				} else if (!cur->ecNodeOutput()) {
					buffer->closeTag();
					cur->setEcFlag();
				} // else node will be ignored
			#ifdef USE_CLOBS
			case e_clob:
				buffer->closeCLOB(tns->removeTag());
				break;
			#endif // ifdef USE_CLOBS
			default:
				break;
				
		}
	}
	cur=cur->traceBack();
	//buffer->debugPrint();
	//cout << endl;
	//cout << "[/CLOSING TAG]" << endl;
	//p_dfa->print();
	//cout << endl;
	//cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
}

void StreamTracker::processPCData(const char* data) {

  	//cout << "[PCDATA '" << data << "']" << endl;

	#ifdef USE_CLOBS
  	// TODO: verify
	if (cur->getType()==e_clob) {
		buffer->appendCLOB(data);
	} else if (cur->getType()==e_out) {
		buffer->appendPCData(data);
	}
	#else
	if (cur->getType()==e_out) {
		buffer->appendPCData(data);
	}
	#endif // USE_CLOBS

	//cout << "[/PCDATA]" << endl;
}
