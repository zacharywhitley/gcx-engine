
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
 *  @brief Implementations of header file stringhash.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file stringhash.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "stringhash.h"
#include <cstring>
#include <cstdlib>

StringHash::StringHash():
base(HASH_BASE), matches(new vector < TagInfo * >[base]), free_index(0) {
    hash("ROOT");
}

StringHash::~StringHash() {
    for (int i = 0; i < base; i++) {
        for (unsigned j = 0; j < matches[i].size(); j++) {
            delete matches[i][j];
        }
    }

    delete[]matches;
}

unsigned StringHash::hash(const char *s) {
    unsigned key = h(s);

    if (matches[key].size() != 0) {
        for (unsigned i = 0; i < matches[key].size(); i++) {
            if (strcmp(matches[key][i]->tag, s) == 0) {
                return matches[key][i]->index;
            }
        }
    }

    char *_s = new char[strlen(s) + 1];

    strcpy(_s, s);

    matches[key].push_back(new TagInfo(_s, free_index));
    return free_index++;
}

unsigned StringHash::hash(const char *s, unsigned start_offset,
                          unsigned end_offset) {
    unsigned key = h(s, start_offset, end_offset);

    // if there is an entry for this bucket, scan all entries in the bucket
    if (matches[key].size() != 0) {
        for (unsigned i = 0; i < matches[key].size(); i++) {
            if (strncmp(s + start_offset, matches[key][i]->tag,
                        strlen(s) - (start_offset + end_offset)) == 0) {
                return matches[key][i]->index;
            }
        }
    }
    // else we store this guy in the hash map
    unsigned total_offset = start_offset + end_offset;
    char *_s = new char[strlen(s) - total_offset + 1];

    strncpy(_s, s + start_offset, strlen(s) - total_offset);
    _s[strlen(s) - (start_offset + end_offset)] = '\0';

    matches[key].push_back(new TagInfo(_s, free_index));
    return free_index++;
}

unsigned StringHash::h(const char *s) {
    unsigned sum = 0;

    for (unsigned i = 0; i < strlen(s); i++) {
        sum += s[i];
    }
    sum %= base;

    return sum;
}

unsigned StringHash::h(const char *s, unsigned start_offset,
                       unsigned end_offset) {
    unsigned sum = 0;

    for (unsigned i = start_offset; i < strlen(s) - end_offset; i++) {
        sum += s[i];
    }
    sum %= base;

    return sum;
}
