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
#include "intsingleset.h"

IntSingleSet::IntSingleSet()
    : IntSet(), elems(NULL) {}

IntSingleSet::IntSingleSet(unsigned max)
    : IntSet(max), elems(new bool[max+1]) {
    for (unsigned i=0; i<=max; i++) {
        elems[i]=false;
    }
}

IntSingleSet::IntSingleSet(unsigned max, vector<unsigned>* _elems)
    : IntSet(max), elems(new bool[max+1]) {

    for (unsigned i=0; i<=max; i++) {
        elems[i]=false;
    }
    for (unsigned i=0; i<_elems->size(); i++) {
        elems[(*_elems)[i]]=true;
    }
}

IntSingleSet::~IntSingleSet() {
    if (elems!=NULL)
        delete []elems;
}

IntSingleSet::IntSingleSet(IntSingleSet& set) 
    : IntSet(set.getMax()), elems(new bool[max+1]) {

    vector<unsigned> _elems=set.getElemVec();
    for (int i=0; i<=max; i++) {
        elems[i]=false;
    }
    for (unsigned i=0; i<_elems.size(); i++) {
        elems[_elems[i]]=true;
    }


}

vector<unsigned> IntSingleSet::getElemVec() {
    vector<unsigned> retvec;
    for (int i=0; i<=max; i++) {
        if (elems[i])    
            retvec.push_back(i);
    }
    return retvec;
}

void IntSingleSet::print() {
    cout << "INT SINGLE SET DEBUG:" << endl;
    for (int i=0; i<=max; i++) {
        if (elems[i])
            cout << i << endl;
    }
}
