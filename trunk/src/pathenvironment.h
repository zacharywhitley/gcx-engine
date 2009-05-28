
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
 *  @brief Header file for pathenvironment.cpp.
 *  @details Header file specifying constructors, destructor and functions for pathenvironment.cpp.
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
#ifndef PATHENVIRONMENT_H
#define PATHENVIRONMENT_H

#include <map>
#include "pathexpression.h"
#include "parvarmap.h"
#include "varname.h"

/*! @typedef map<unsigned, PathExpression*> PEnv
 *  @brief The underlying map of this object.
 *  @details The underlying map of this object, which maps variables to their path.
 */
typedef map < unsigned,
 PathExpression * >PEnv;

/*! @class PathEnvironment
 *  @brief Represents mapping of variables to their path.
 *  @details Represents mapping of variables to their path,
 *    which takes therefore a variable, e.g. <tt>\$x</tt> or <tt>\$root</tt>, and a
 *    PathExpression, e.g. <tt>/bib//book</tt> or <tt>/bib/book/title</tt>, as arguments.
 *  @note Variables are internally represented by numbers!
 *  @author Michael Schmidt
 *  @author Gunnar Jehl
 *  @version 2.1
 *  @license Software License Agreement (BSD License)
 */
class PathEnvironment {

  public:
    /*! @brief Constructor.
     *  @details Constructor - creating object.
     */
    PathEnvironment();

    /*! @brief Destructor.
     *  @details Destructor.
     */
    virtual ~ PathEnvironment();

    /*! @fn void insertPath(unsigned var, PathExpression* path)
     *  @brief Inserts a new variable with its path.
     *  @details Inserts a new variable with its path.
     *  @param[in] var Variable for which path holds.
     *  @param[in] path Pointer to a PathExpression object.
     *  @retval void
     */
    void insertPath(unsigned var, PathExpression * path);

    /*! @fn void print(OutputStream& dos)
     *  @brief Prints all variables with their path.
     *  @details Prints all variables with their path.
     *  @param[in] dos Pointer to a OutputStream object.
     *  @retval void
     */
    void print(OutputStream & dos);

    /*! @fn PathExpression* getPath(unsigned var)
     *  @brief Returns the path for the entered variable.
     *  @details Returns the path for the entered variable.
     *  @param[in] var Variable for which path should be obtained.
     *  @retval PathExpression* Pointer to a PathExpression object.
     */
    PathExpression *getPath(unsigned var);

    /*! @fn PathExpression* getPathBetween(unsigned end, unsigned start)
     *  @brief Returns the path spanned between the two entered variables.
     *  @details Returns the path spanned between the two entered variables.
     *  @param[in] end Variable for which path should be obtained.
     *  @param[in] start Variable for which path should be obtained.
     *  @retval PathExpression* Pointer to a PathExpression object.
     */
    PathExpression *getPathBetween(unsigned end, unsigned start);

  private:
    /*! @var PEnv env
     *  @brief The underlying map of this object.
     *  @details The underlying map of this object, which maps variables to their path.
     */
    PEnv env;
};

#endif // PATHENVIRONMENT_H
