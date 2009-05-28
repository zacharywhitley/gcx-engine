
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
 *  @brief Implementations of header file roundingexpression.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file roundingexpression.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "roundingexpression.h"

RoundingExpression::RoundingExpression(EXP_TYPE _type,
                                       AggregateFunctExpression *
                                       _aggfunct):OperandExpression(_type,
                                                                    et_operandrounding),
aggfunct(_aggfunct) {
}

RoundingExpression::~RoundingExpression() {
    delete aggfunct;
}

void RoundingExpression::scopeCheck(vector < unsigned >&def_vars,
                                    vector < unsigned >&introduced_vars,
                                    vector < unsigned >&violating_vars) {
    aggfunct->scopeCheck(def_vars, introduced_vars, violating_vars);
}

void RoundingExpression::replaceVarId(unsigned old_id, unsigned new_id) {
    aggfunct->replaceVarId(old_id, new_id);
}

void RoundingExpression::extractFSAMap(FSAMap * fsamap, unsigned parent_var) {
    aggfunct->extractFSAMap(fsamap, parent_var);
}

void RoundingExpression::extractParVarMap(ParVarMap * parvarmap) {
    aggfunct->extractParVarMap(parvarmap);
}

void RoundingExpression::extractDependencies(vector < DependencySet * >*depset) {
    aggfunct->extractDependencies(depset);
}

Expression *RoundingExpression::placeSignOffs(vector <
                                              SignOffExpression * >&signoffs) {
    aggfunct->placeSignOffs(signoffs);

    return this;
}

void RoundingExpression::rewriteAggregateFuncts() {
    aggfunct->rewriteAggregateFuncts();
}

void RoundingExpression::print(OutputStream & dos) const {
    switch (type) {
        case et_abs:
            dos << "fn:abs(" << (*aggfunct) << ")";
            break;
        case et_ceiling:
            dos << "fn:ceiling(" << (*aggfunct) << ")";
            break;
        case et_cover:
            dos << "fn:cover(" << (*aggfunct) << ")";
            break;
        case et_floor:
            dos << "fn:floor(" << (*aggfunct) << ")";
            break;
        case et_round:
            dos << "fn:round(" << (*aggfunct) << ")";
            break;
        case et_roundhalftoeven:
            dos << "fn:round-half-to-even(" << (*aggfunct) << ")";
            break;
        case et_truncate:
            dos << "fn:truncate(" << (*aggfunct) << ")";
            break;
        default:               // should never happen
            break;
    }
}

void RoundingExpression::init(BufferNode * root) {
    aggfunct->init(root);
}

void RoundingExpression::prepareOperand(OutputStream & eos, Environment * env,
                                        unsigned modus) {
    eval(eos, env, EVAL_QUERY_SILENT);
}

Value *RoundingExpression::getNextValue() {
    if (initial) {
        initial = false;
        return &cur_val;
    } else {
        initial = true;
        return NULL;
    }
}
