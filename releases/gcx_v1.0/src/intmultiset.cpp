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
#include "intmultiset.h"

IntMultiSet::IntMultiSet()
    : IntSet(), elems(NULL) {}

IntMultiSet::IntMultiSet(unsigned max)
    : IntSet(max), elems(new unsigned[max+1]) {
    for (unsigned i=0; i<=max; i++)
        elems[i]=0;
}

IntMultiSet::IntMultiSet(unsigned max, vector<unsigned>* _elems)
    : IntSet(max), elems(new unsigned[max+1]) {

    for (unsigned i=0; i<=max; i++) {
        elems[i]=0;
    }
    for (unsigned i=0; i<_elems->size(); i++) {
        elems[(*_elems)[i]]++;
    }
}


IntMultiSet::~IntMultiSet() {
    delete []elems;
}

vector<unsigned> IntMultiSet::getElemVec() {
    vector<unsigned> retvec;
    for (unsigned i=0; (int)i<=max; i++) {
        for (unsigned j=0; j<elems[i]; j++) {
            retvec.push_back(i);
        }
    }
    return retvec;
}

void IntMultiSet::print() {
    cout << "INTMULTISET DEBUG:" << endl;
    for (unsigned i=0; (int)i<=max; i++) {
        cout << i << " (" << elems[i] << ")" << endl;
    }
}
