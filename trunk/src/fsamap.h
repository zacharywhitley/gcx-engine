
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
 *  @brief Header file for fsamap.cpp.
 *  @details Header file specifying constructors, destructor and functions for fsamap.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef FSAMAP_H
#define FSAMAP_H

#include <map>
#include "typeenums.h"
#include "varname.h"

//! @namespace std
using namespace std;

/*! @typedef map<unsigned, unsigned> VarVarMap
 *  @brief The underlying map of this object.
 *  @details The underlying map of this object, which maps variables to their first straight ancestor (FSA) variable.
 */
typedef map < unsigned,
unsigned >VarVarMap;

/*! @class FSAMap
 *  @brief Represents mapping of variables to their first straight ancestor (FSA) variable.
 *  @details Represents mapping of variables to their first straight ancestor (FSA) variable,
 *    which takes therefore two variables, e.g. <tt>FSA(\$x) = \$y</tt>, as arguments. 
 *  @note Variables are internally represented by numbers!
 *  @note SINGLETON PATTERN!
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class FSAMap {

  public:
    /*! @fn static FSAMap* getInstance()
     *  @brief Returns instance of FSAMap.
     *  @details Returns instance of FSAMap, i.e. of this object.
     *  @retval FSAMap* Pointer to a FSAMap object.
     */
    static FSAMap *getInstance();

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ FSAMap();

    /*! @fn void insertFSA(unsigned var, unsigned fsa)
     *  @brief Inserts a new variable with its first straight ancestor (FSA) variable.
     *  @details Inserts a new variable with its first straight ancestor (FSA) variable.
     *  @param[in] var Variable for which first straight ancestor (FSA) variable holds.
     *  @param[in] fsa First straight ancestor (FSA) variable.
     *  @retval void
     */
    void insertFSA(unsigned var, unsigned fsa);

    /*! @fn void print(OutputStream& dos)
     *  @brief Prints all variables with their first straight ancestor (FSA) variable.
     *  @details Prints all variables with their first straight ancestor (FSA) variable.
     *  @param[in] dos Pointer to a OutputStream object.
     *  @retval void
     */
    void print(OutputStream & dos);

    /*! @fn unsigned getFSA(unsigned var)
     *  @brief Returns the first straight ancestor (FSA) variable for the entered variable.
     *  @details Returns the first straight ancestor (FSA) variable for the entered variable.
     *  @param[in] var Variable for which first straight ancestor (FSA) variable should be obtained.
     *  @retval unsigned First straight ancestor (FSA) variable for the entered variable.
     */
    unsigned getFSA(unsigned var);

  private:
    /*! @var static FSAMap* instance
     *  @brief Instance of FSAMap.
     *  @details Instance of FSAMap to avoid duplicate existence of this object.
     */
    static FSAMap *instance;

    /*! @brief Constructor.
     *  @details Constructor - creating object.
     */
     FSAMap();

    /*! @var VarVarMap fsamap
     *  @brief The underlying map of this object.
     *  @details The underlying map of this object, which maps variables to their first straight ancestor (FSA) variable.
     */
    VarVarMap fsamap;
};

#endif // FSAMAP_H
