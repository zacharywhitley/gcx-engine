
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
 *  @brief Implementations of header file aggregatefunctcountexpression.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file aggregatefunctcountexpression.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "aggregatefunctcountexpression.h"

AggregateFunctCountExpression::AggregateFunctCountExpression(VarExpression * _var):
AggregateFunctExpression(et_aggregatefunctcount, _var) {
}

AggregateFunctCountExpression::AggregateFunctCountExpression(VarStepExpression * _varstep):
AggregateFunctExpression(et_aggregatefunctcount, _varstep) {
}

AggregateFunctCountExpression::~AggregateFunctCountExpression() {
}

void AggregateFunctCountExpression::eval(OutputStream & eos,
                                         Environment * env, unsigned modus) {

    switch (modus) {

        case EVAL_QUERY:
          {
              bool empty_sequence = true;
              long double count_val =
                  calculateValue(eos, env, modus, empty_sequence);
              if (!empty_sequence) {
                  const char *count_str =
                      MiscFunctions::getStringFromNumerical(count_val);
                  eos << count_str;
                  delete[]count_str;
              } else {
                  const char *count_str =
                      MiscFunctions::
                      getStringFromNumerical(OUTPUT_COUNT_ON_EMPTY_SEQUENCE);
                  eos << count_str;
                  delete[]count_str;
              }
              break;
          }

        case EVAL_QUERY_SILENT:
          {
              bool empty_sequence = true;
              long double count_val =
                  calculateValue(eos, env, modus, empty_sequence);
              if (!empty_sequence) {
                  cur_val.setNumVal(count_val);
              } else {
                  cur_val.setNumVal(OUTPUT_COUNT_ON_EMPTY_SEQUENCE);
              }
              break;
          }

        case EVAL_SIGNOFF:
            forexp->eval(eos, env, EVAL_SIGNOFF);
            break;

        default:
            throw
                RuntimeException
                ("AggregateFunctCountExpression: Illegal Evaluation Mode",
                 eid_runtime_illegalmode);
            break;
    }
}

long double AggregateFunctCountExpression::calculateValue(OutputStream & eos,
                                                          Environment * env,
                                                          unsigned modus,
                                                          bool &
                                                          empty_sequence) {

    var ? bit->init(env->getNodeBinding(var->getId())) :
        bit->init(env->getNodeBinding(varstep->getId()));

    long double count_ret = 0;
    long double result = 0;

    while (BufferNode * cur =
           bit->getNext(READ_UP_TO_CLOSE_NONE, LOCK_CONTEXT_ALWAYS_CLEAR)) {

        empty_sequence = false;
        result++;

        forexp->evalSignOffForBinding(eos, env, cur);
    }
    bit->clear();

    if (!empty_sequence) {
        count_ret = MiscFunctions::getRoundFrom(result);
    }

    return count_ret;
}
