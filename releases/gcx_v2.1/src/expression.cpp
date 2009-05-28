
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
 *  @brief Implementations of header file expression.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file expression.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "expression.h"

Expression::Expression(EXP_TYPE _type):tagmap(TagMap::getInstance()),
type(_type) {
}

Expression::~Expression() {
}

void Expression::scopeCheck(vector < unsigned >&def_vars,
                            vector < unsigned >&introduced_vars,
                            vector < unsigned >&violating_vars) {
}

void Expression::replaceVarId(unsigned old_id, unsigned new_id) {
}

void Expression::mergeSequences() {
}

void Expression::extractFSAMap(FSAMap * fsamap, unsigned parent_var) {
}

void Expression::extractParVarMap(ParVarMap * parvarmap) {
}

void Expression::extractDependencies(vector < DependencySet * >*depset) {
}

Expression *Expression::placeSignOffs(vector < SignOffExpression * >&signoffs) {
    return this;
}

void Expression::rewriteWhereExps() {
}

void Expression::rewriteEmptyFuncts() {
}

void Expression::rewriteVarstepCondExps() {
}

void Expression::rewriteAggregateFuncts() {
}

void Expression::rewriteVarsteps() {
}

void Expression::init(BufferNode * root) {
}

void Expression::eval(OutputStream & eos, Environment * env, unsigned modus) {
}

bool Expression::containsDirectOutput() {
    return false;
}
