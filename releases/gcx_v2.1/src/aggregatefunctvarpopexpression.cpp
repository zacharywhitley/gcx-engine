
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
 *  @brief Implementations of header file aggregatefunctvarpopexpression.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file aggregatefunctvarpopexpression.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "aggregatefunctvarpopexpression.h"

AggregateFunctVarPopExpression::AggregateFunctVarPopExpression(VarExpression * _var):
AggregateFunctExpression(et_aggregatefunctvarpop, _var) {
}

AggregateFunctVarPopExpression::AggregateFunctVarPopExpression(VarStepExpression * _varstep):
AggregateFunctExpression(et_aggregatefunctvarpop, _varstep) {
}

AggregateFunctVarPopExpression::~AggregateFunctVarPopExpression() {
}

void AggregateFunctVarPopExpression::eval(OutputStream & eos,
                                          Environment * env, unsigned modus) {

    switch (modus) {

        case EVAL_QUERY:
          {
              bool empty_sequence = true;
              long double varpop_val =
                  calculateValue(eos, env, modus, empty_sequence);
              if (!empty_sequence) {
                  const char *varpop_str =
                      MiscFunctions::getStringFromNumerical(varpop_val);
                  eos << varpop_str;
                  delete[]varpop_str;
              } else {
                  eos << OUTPUT_VARPOP_ON_EMPTY_SEQUENCE;
              }
              break;
          }

        case EVAL_QUERY_SILENT:
          {
              bool empty_sequence = true;
              long double varpop_val =
                  calculateValue(eos, env, modus, empty_sequence);
              if (!empty_sequence) {
                  cur_val.setNumVal(varpop_val);
              } else {
                  const char *varpop_empty_val =
                      new char[strlen(OUTPUT_VARPOP_ON_EMPTY_SEQUENCE) + 1];
                  strcpy((char *) varpop_empty_val,
                         OUTPUT_VARPOP_ON_EMPTY_SEQUENCE);
                  cur_val.setStrVal(varpop_empty_val);
              }
              break;
          }

        case EVAL_SIGNOFF:
            forexp->eval(eos, env, EVAL_SIGNOFF);
            break;

        default:
            throw
                RuntimeException
                ("AggregateFunctVarPopExpression: Illegal Evaluation Mode",
                 eid_runtime_illegalmode);
            break;
    }
}

long double AggregateFunctVarPopExpression::calculateValue(OutputStream & eos,
                                                           Environment * env,
                                                           unsigned modus,
                                                           bool &
                                                           empty_sequence) {

    var ? bit->init(env->getNodeBinding(var->getId())) :
        bit->init(env->getNodeBinding(varstep->getId()));

    long double varpop_ret = 0;
    long double result = 0;
    vector < long double >*pcds = new vector < long double >;
    const char *pcd = NULL;

    while (BufferNode * cur =
           bit->getNext(READ_UP_TO_CLOSE_CONTEXT, LOCK_CONTEXT_ALWAYS_CLEAR)) {

        empty_sequence = false;

        pcd = cur->getPCDataRepresentation();
        long double tmp = MiscFunctions::getNumericFromString(pcd);

        result = MiscFunctions::getSummationFrom(result, tmp);
        pcds->push_back(tmp);

        free((char *) pcd);

        forexp->evalSignOffForBinding(eos, env, cur);
    }
    bit->clear();

    if (!empty_sequence) {
        long double avg_val =
            MiscFunctions::getDivisionFrom(result, pcds->size());
        result = 0;
        for (unsigned i = 0; i < pcds->size(); i++) {
            result +=
                MiscFunctions::getPowerFrom(MiscFunctions::
                                            getSubtractionFrom((*pcds)[i],
                                                               avg_val), 2);
        }
        varpop_ret =
            MiscFunctions::getRoundFrom(MiscFunctions::
                                        getDivisionFrom(result, pcds->size()));
    }
    delete pcds;

    return varpop_ret;
}
