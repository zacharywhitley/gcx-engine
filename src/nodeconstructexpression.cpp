
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
 *  @brief Implementations of header file nodeconstructexpression.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file nodeconstructexpression.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "nodeconstructexpression.h"

NodeConstructExpression::NodeConstructExpression(const char *_tag,
                                                 Expression *
                                                 _exp):Expression
    (et_nodeconstr),
tag(tagmap->insertTag(_tag)),
exp(_exp) {
}

NodeConstructExpression::~NodeConstructExpression() {
    delete exp;
}

void NodeConstructExpression::scopeCheck(vector < unsigned >&def_vars,
                                         vector < unsigned >&introduced_vars,
                                         vector < unsigned >&violating_vars) {
    exp->scopeCheck(def_vars, introduced_vars, violating_vars);
}

void NodeConstructExpression::replaceVarId(unsigned old_id, unsigned new_id) {
    exp->replaceVarId(old_id, new_id);
}

void NodeConstructExpression::mergeSequences() {
    exp->mergeSequences();
}

void NodeConstructExpression::extractFSAMap(FSAMap * fsamap,
                                            unsigned parent_var) {
    exp->extractFSAMap(fsamap, parent_var);
}

void NodeConstructExpression::extractParVarMap(ParVarMap * parvarmap) {
    exp->extractParVarMap(parvarmap);
}

void NodeConstructExpression::extractDependencies(vector <
                                                  DependencySet * >*depset) {
    exp->extractDependencies(depset);
}

Expression *NodeConstructExpression::placeSignOffs(vector <
                                                   SignOffExpression *
                                                   >&signoffs) {
    exp = exp->placeSignOffs(signoffs);

    return this;
}

void NodeConstructExpression::rewriteWhereExps() {
    exp->rewriteWhereExps();
}

void NodeConstructExpression::rewriteEmptyFuncts() {
    exp->rewriteEmptyFuncts();
}

void NodeConstructExpression::rewriteVarstepCondExps() {
    exp->rewriteVarstepCondExps();
}

void NodeConstructExpression::rewriteAggregateFuncts() {
    exp->rewriteAggregateFuncts();
}

void NodeConstructExpression::rewriteVarsteps() {
    if (exp->getType() == et_varstep) {
        VarStepExpression *sub = ((VarStepExpression *) exp)->clone();
        delete exp;
        VarExpression *tmpvar =
            new VarExpression(VarName::getInstance()->getFreshVarname(),
                              true);
        ForExpression *forexp = new ForExpression(tmpvar, sub, tmpvar->clone());

        exp = forexp;
    } else {
        exp->rewriteVarsteps();
    }
}

void NodeConstructExpression::print(OutputStream & dos) const {
    EXP_TYPE subtype = exp->getType();

    if (subtype == et_empty) {
        dos << "<" << tagmap->getTag(tag) << "/>";
        return;
    }
    dos << "<" << tagmap->getTag(tag) << "> {";
    if (subtype == et_for || subtype == et_if || subtype == et_nodeconstr
        || subtype == et_sequence || subtype == et_signoff) {
        dos << NEWLINE << incrementIndents() << writeIndents() << (*exp) <<
            NEWLINE;
        dos << decrementIndents() << writeIndents();
    } else {
        dos << (*exp);
    }
    dos << "} </" << tagmap->getTag(tag) << ">";
}

void NodeConstructExpression::init(BufferNode * root) {
    exp->init(root);
}

void NodeConstructExpression::eval(OutputStream & eos, Environment * env,
                                   unsigned modus) {

    switch (modus) {

        case EVAL_QUERY:
            if (exp->getType() == et_empty) {
                eos << "<" << tagmap->getTag(tag) << "/>";
            } else {
                eos << "<" << tagmap->getTag(tag) << ">";
                exp->eval(eos, env, EVAL_QUERY);
                eos << "</" << tagmap->getTag(tag) << ">";
            }
            break;

        case EVAL_SIGNOFF:
            exp->eval(eos, env, EVAL_SIGNOFF);
            break;

        default:
            throw
                RuntimeException
                ("NodeConstructExpression: Illegal Evaluation Mode",
                 eid_runtime_illegalmode);
            break;
    }
}
