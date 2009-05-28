
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
 *  @brief Implementations of header file aggregatefunctsumexpression.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file aggregatefunctsumexpression.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "aggregatefunctsumexpression.h"

AggregateFunctSumExpression::AggregateFunctSumExpression(VarExpression * _var):
AggregateFunctExpression(et_aggregatefunctsum, _var) {
}

AggregateFunctSumExpression::AggregateFunctSumExpression(VarStepExpression * _varstep):
AggregateFunctExpression(et_aggregatefunctsum, _varstep) {
}

AggregateFunctSumExpression::~AggregateFunctSumExpression() {
}

void AggregateFunctSumExpression::eval(OutputStream & eos, Environment * env,
                                       unsigned modus) {

    switch (modus) {

        case EVAL_QUERY:
          {
              bool empty_sequence = true;
              long double sum_val =
                  calculateValue(eos, env, modus, empty_sequence);
              if (!empty_sequence) {
                  const char *sum_str =
                      MiscFunctions::getStringFromNumerical(sum_val);
                  eos << sum_str;
                  delete[]sum_str;
              } else {
                  const char *sum_str =
                      MiscFunctions::
                      getStringFromNumerical(OUTPUT_SUM_ON_EMPTY_SEQUENCE);
                  eos << sum_str;
                  delete[]sum_str;
              }
              break;
          }

        case EVAL_QUERY_SILENT:
          {
              bool empty_sequence = true;
              long double sum_val =
                  calculateValue(eos, env, modus, empty_sequence);
              if (!empty_sequence) {
                  cur_val.setNumVal(sum_val);
              } else {
                  cur_val.setNumVal(OUTPUT_SUM_ON_EMPTY_SEQUENCE);
              }
              break;
          }

        case EVAL_SIGNOFF:
            forexp->eval(eos, env, EVAL_SIGNOFF);
            break;

        default:
            throw
                RuntimeException
                ("AggregateFunctSumExpression: Illegal Evaluation Mode",
                 eid_runtime_illegalmode);
            break;
    }
}

long double AggregateFunctSumExpression::calculateValue(OutputStream & eos,
                                                        Environment * env,
                                                        unsigned modus,
                                                        bool & empty_sequence) {

    var ? bit->init(env->getNodeBinding(var->getId())) :
        bit->init(env->getNodeBinding(varstep->getId()));

    long double sum_ret = 0;
    long double result = 0;
    const char *pcd = NULL;

    while (BufferNode * cur =
           bit->getNext(READ_UP_TO_CLOSE_CONTEXT, LOCK_CONTEXT_ALWAYS_CLEAR)) {

        empty_sequence = false;

        pcd = cur->getPCDataRepresentation();
        result =
            MiscFunctions::getSummationFrom(result,
                                            MiscFunctions::
                                            getNumericFromString(pcd));

        free((char *) pcd);

        forexp->evalSignOffForBinding(eos, env, cur);
    }
    bit->clear();

    if (!empty_sequence) {
        sum_ret = MiscFunctions::getRoundFrom(result);
    }

    return sum_ret;
}
