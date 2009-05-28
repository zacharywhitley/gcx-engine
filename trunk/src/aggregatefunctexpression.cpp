
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
 *  @brief Implementations of header file aggregatefunctexpression.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file aggregatefunctexpression.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "aggregatefunctexpression.h"

AggregateFunctExpression::AggregateFunctExpression(EXP_TYPE _type,
                                                   VarExpression *
                                                   _var):OperandExpression
    (_type, et_operandaggregatefunct),
var(_var),
varstep(NULL),
forexp(NULL),
bit(NULL) {
}

AggregateFunctExpression::AggregateFunctExpression(EXP_TYPE _type,
                                                   VarStepExpression *
                                                   _varstep):OperandExpression
    (_type, et_operandaggregatefunct),
var(NULL),
varstep(_varstep),
forexp(NULL),
bit(NULL) {
}

AggregateFunctExpression::~AggregateFunctExpression() {
    delete var;
    delete varstep;
    delete forexp;
    delete bit;
}

void AggregateFunctExpression::scopeCheck(vector < unsigned >&def_vars,
                                          vector < unsigned >&introduced_vars,
                                          vector < unsigned >&violating_vars) {
    if (var) {
        return var->scopeCheck(def_vars, introduced_vars, violating_vars);
    } else {
        return varstep->scopeCheck(def_vars, introduced_vars, violating_vars);
    }
}

void AggregateFunctExpression::replaceVarId(unsigned old_id, unsigned new_id) {
    if (var) {
        var->replaceVarId(old_id, new_id);
    } else {
        varstep->replaceVarId(old_id, new_id);
    }
}

void AggregateFunctExpression::extractFSAMap(FSAMap * fsamap,
                                             unsigned parent_var) {
    forexp->extractFSAMap(fsamap, parent_var);
}

void AggregateFunctExpression::extractParVarMap(ParVarMap * parvarmap) {
    forexp->extractParVarMap(parvarmap);
}

void AggregateFunctExpression::extractDependencies(vector <
                                                   DependencySet * >*depset) {
    // there is no need to add a /dos::node()-dependency tuple for aggregate function fn:count()
    // because all for counting required nodes are covered by the variable node!
    if (type != et_aggregatefunctcount) {
        forexp->extractDependencies(depset);
    }
}

Expression *AggregateFunctExpression::placeSignOffs(vector <
                                                    SignOffExpression *
                                                    >&signoffs) {
    forexp = (ForExpression *) forexp->placeSignOffs(signoffs);

    return this;
}

void AggregateFunctExpression::rewriteAggregateFuncts() {
    if (var) {
        VarExpression *tmpvar =
            new VarExpression(VarName::getInstance()->getFreshVarname(),
                              true);
        ForExpression *fexp =
            new ForExpression(tmpvar, var->clone(), tmpvar->clone());
        forexp = fexp;
    } else {
        VarExpression *tmpvar =
            new VarExpression(VarName::getInstance()->getFreshVarname(),
                              true);
        ForExpression *fexp =
            new ForExpression(tmpvar, varstep->clone(), tmpvar->clone());
        forexp = fexp;
    }
}

void AggregateFunctExpression::print(OutputStream & dos) const {
    switch (type) {
        case et_aggregatefunctsum:
            if (forexp) {
                dos << "fn:sum(" << (*forexp) << ")";
            } else if (varstep) {
                dos << "fn:sum(" << (*varstep) << ")";
            } else {
                dos << "fn:sum(" << (*var) << ")";
            }
            break;
        case et_aggregatefunctavg:
            if (forexp) {
                dos << "fn:avg(" << (*forexp) << ")";
            } else if (varstep) {
                dos << "fn:avg(" << (*varstep) << ")";
            } else {
                dos << "fn:avg(" << (*var) << ")";
            }
            break;
        case et_aggregatefunctmin:
            if (forexp) {
                dos << "fn:min(" << (*forexp) << ")";
            } else if (varstep) {
                dos << "fn:min(" << (*varstep) << ")";
            } else {
                dos << "fn:min(" << (*var) << ")";
            }
            break;
        case et_aggregatefunctmax:
            if (forexp) {
                dos << "fn:max(" << (*forexp) << ")";
            } else if (varstep) {
                dos << "fn:max(" << (*varstep) << ")";
            } else {
                dos << "fn:max(" << (*var) << ")";
            }
            break;
        case et_aggregatefunctcount:
            if (forexp) {
                dos << "fn:count(" << (*forexp) << ")";
            } else if (varstep) {
                dos << "fn:count(" << (*varstep) << ")";
            } else {
                dos << "fn:count(" << (*var) << ")";
            }
            break;
        case et_aggregatefunctstddevsamp:
            if (forexp) {
                dos << "fn:stddev_samp(" << (*forexp) << ")";
            } else if (varstep) {
                dos << "fn:stddev_samp(" << (*varstep) << ")";
            } else {
                dos << "fn:stddev_samp(" << (*var) << ")";
            }
            break;
        case et_aggregatefunctstddevpop:
            if (forexp) {
                dos << "fn:stddev_pop(" << (*forexp) << ")";
            } else if (varstep) {
                dos << "fn:stddev_pop(" << (*varstep) << ")";
            } else {
                dos << "fn:stddev_pop(" << (*var) << ")";
            }
            break;
        case et_aggregatefunctvarsamp:
            if (forexp) {
                dos << "fn:var_samp(" << (*forexp) << ")";
            } else if (varstep) {
                dos << "fn:var_samp(" << (*varstep) << ")";
            } else {
                dos << "fn:var_samp(" << (*var) << ")";
            }
            break;
        case et_aggregatefunctvarpop:
            if (forexp) {
                dos << "fn:var_pop(" << (*forexp) << ")";
            } else if (varstep) {
                dos << "fn:var_pop(" << (*varstep) << ")";
            } else {
                dos << "fn:var_pop(" << (*var) << ")";
            }
            break;
        case et_aggregatefunctmedian:
            if (forexp) {
                dos << "fn:median(" << (*forexp) << ")";
            } else if (varstep) {
                dos << "fn:median(" << (*varstep) << ")";
            } else {
                dos << "fn:median(" << (*var) << ")";
            }
            break;
        default:               // should never happen
            break;
    }
}

void AggregateFunctExpression::init(BufferNode * root) {
    delete bit;

    bit =
        var ? new BufferIterator(root, NULL) : new BufferIterator(root,
                                                                  varstep->
                                                                  getPath());
    forexp->init(root);
}

void AggregateFunctExpression::prepareOperand(OutputStream & eos,
                                              Environment * env,
                                              unsigned modus) {
    eval(eos, env, EVAL_QUERY_SILENT);
}

Value *AggregateFunctExpression::getNextValue() {
    if (initial) {
        initial = false;
        return &cur_val;
    } else {
        initial = true;
        return NULL;
    }
}
