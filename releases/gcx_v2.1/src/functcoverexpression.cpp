
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
 *  @brief Implementations of header file functcoverexpression.h.
 *  @details Implementations of constructors, destructor and functions of the corresponding header file functcoverexpression.h.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#include "functcoverexpression.h"

FunctCoverExpression::FunctCoverExpression(AggregateFunctExpression * _aggfunct):
RoundingExpression(et_cover, _aggfunct) {
}

FunctCoverExpression::~FunctCoverExpression() {
}

void FunctCoverExpression::eval(OutputStream & eos, Environment * env,
                                unsigned modus) {

    switch (modus) {

        case EVAL_QUERY:
          {
              bool non_value = true;
              long double cover_val =
                  calculateValue(eos, env, modus, non_value);
              if (!non_value) {
                  const char *cover_str =
                      MiscFunctions::getStringFromNumerical(cover_val);
                  eos << cover_str;
                  delete[]cover_str;
              } else {
                  eos << OUTPUT_COVER_ON_NON_VALUE;
              }
              break;
          }

        case EVAL_QUERY_SILENT:
          {
              bool non_value = true;
              long double cover_val =
                  calculateValue(eos, env, modus, non_value);
              if (!non_value) {
                  cur_val.setNumVal(cover_val);
              } else {
                  const char *cover_non_val =
                      new char[strlen(OUTPUT_COVER_ON_NON_VALUE) + 1];
                  strcpy((char *) cover_non_val, OUTPUT_COVER_ON_NON_VALUE);
                  cur_val.setStrVal(cover_non_val);
              }
              break;
          }

        case EVAL_SIGNOFF:
            break;

        default:
            throw
                RuntimeException
                ("FunctCoverExpression: Illegal Evaluation Mode",
                 eid_runtime_illegalmode);
            break;
    }
}

long double FunctCoverExpression::calculateValue(OutputStream & eos,
                                                 Environment * env,
                                                 unsigned modus,
                                                 bool & non_value) {

    long double cover_ret = 0;

    aggfunct->prepareOperand(eos, env, modus);
    Value *agg_val = aggfunct->getNextValue();

    if (agg_val->getType() == xsd_numeric) {
        non_value = false;
        long double aggfunct_result = agg_val->getNumVal();

        cover_ret =
            aggfunct_result <
            0 ? floor(aggfunct_result) : ceil(aggfunct_result);
    }
    aggfunct->unprepareOperand();

    return cover_ret;
}
