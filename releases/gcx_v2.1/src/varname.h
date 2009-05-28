
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
 *  @brief Header file for varname.cpp.
 *  @details Header file specifying constructors, destructor and functions for varname.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef VARNAME_H
#define VARNAME_H

#include <vector>
#include <sstream>
#include "typeenums.h"

//! @namespace std
using namespace std;

// forward declarations
class OutputStream;

/*! @class VarName
 *  @brief Represents mapping of numeric values of variables to their string value.
 *  @details Represents mapping of numeric values of variables to their string value, which takes
 *    therefore a string, e.g. <tt>\$x</tt> or <tt>\$y</tt>, as arguments and returns the string value
 *    for a (numeric value of a) variable.
 *  @note Variables are internally represented by numbers!
 *  @note SINGLETON PATTERN!
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class VarName {

  public:
    /*! @fn static VarName* getInstance()
     *  @brief Returns instance of VarName.
     *  @details Returns instance of VarName, i.e. of this object.
     *  @retval VarName* Pointer to a VarName object.
     */
    static VarName *getInstance();

    /*! @brief Destructor.
     *  @details Destructor.
     */
     virtual ~ VarName();

    /*! @fn unsigned insertVarname(const char* varname, bool force)
     *  @brief Inserts a new string of a variable and returns the numeric value.
     *  @details Inserts a new string of a variable and returns the numeric value.
     *  @param[in] varname Pointer to a char object (string of a variable).
     *  @param[in] force <tt>true</tt> (default: <tt>false</tt>) if force insertion of variable,
     *    <tt>false</tt> otherwise.
     *  @retval unsigned Numeric value for string value of a variable.
     */
    unsigned insertVarname(const char *varname, bool force = false);

    /*! @fn void print(OutputStream& dos)
     *  @brief Prints mapping of (numeric values of) variables to their string values.
     *  @details Prints mapping of (numeric values of) variables to their string values.
     *  @param[in] dos Pointer to a OutputStream object.
     *  @retval void
     */
    void print(OutputStream & dos);

    /*! @fn inline const char* getVarname(unsigned id)
     *  @brief Returns string value for the entered (numeric value of a) variable.
     *  @details Returns string value for the entered (numeric value of a) variable.
     *  @param[in] id Numeric value of a variable.
     *  @retval char* Pointer to a char object.
     */
    inline const char *getVarname(unsigned id) {
        return varnames[id];
    }
    /*! @fn const char* getFreshVarname()
     *  @brief Returns a new unused string to be used for a variable.
     *  @details Returns a new unused string to be used for a variable.
     *  @retval char* Pointer to a char object.
     */ const char *getFreshVarname();

  private:
    /*! @var static VarName* instance
     *  @brief Instance of VarName.
     *  @details Instance of VarName to avoid duplicate existence of this object.
     */
    static VarName *instance;

    /*! @brief Constructor.
     *  @details Constructor - creating object.
     */
    VarName();

    /*! @var vector<const char*> varnames
     *  @brief The string values of all used variables.
     *  @details The string values of all used variables.
     */
    vector < const char *>varnames;

    /*! @var int index
     *  @brief The highest numeric value of the (last) fresh variable.
     *  @details The highest numeric value of the (last) fresh variable,
     *    which has been obtained from method getFreshVarname().
     */
    int index;
};

#endif // VARNAME_H
