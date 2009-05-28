/*
 | Author: Michael Schmidt;
 |         Gunnar Jehl (multi-step paths/aggregate functions/optimizations) 
 ===========================================================================
 ===========================================================================
 | Software License Agreement (BSD License)
 | 
 | Copyright (c) 2006-2007, Saarland University Database Group 
 | All rights reserved.
 |
 | Redistribution and use of this software in source and binary forms,
 | with or without modification, are permitted provided that the following
 | conditions are met:
 | 
 | * Redistributions of source code must retain the above
 |   copyright notice, this list of conditions and the
 |   following disclaimer.
 |
 | * Redistributions in binary form must reproduce the above
 |   copyright notice, this list of conditions and the
 |   following disclaimer in the documentation and/or other
 |   materials provided with the distribution.
 |
 | * Neither the name the of Saarland University Database Group nor the names 
 |   of its contributors may be used to endorse or promote products derived 
 |   from this software without specific prior written permission of the
 |   Saarland University Database Group.
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
*/
/*! @file
 *     @brief Header file for projectiontreenode.cpp.
 *     @details Header file specifying constructors, destructor and functions for projectiontreenode.cpp.
 *     @author Michael Schmidt
 *     @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *     @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
#ifndef PROJECTIONTREENODE_H
#define PROJECTIONTREENODE_H

#include "pathexpression.h"
#include "passiveprojectiontree.h"

/*! @class ProjectionTreeNode
 *     @brief
 *     @details
 *     @author Michael Schmidt
 *  @author Gunnar Jehl (multi-step paths/aggregate functions/optimizations)
 *  @version 1.01b
 *     @license Software License Agreement (BSD License)
 */
class ProjectionTreeNode {

public:
    //! constructor for arbitrarily parametrized projection tree node
    //! @param _parent parent projection tree node,
    //! @param _var the variable the node referes to (-1 if none)
    //! @param _path the path carried by this node
    //! @param _dep true if node is a dependency node, false otherwise
    //! @param _role role associated with this projection tree node
    //! @param _direct_output true if the node described a direct output for-expression 
    ProjectionTreeNode(ProjectionTreeNode* _parent, int _var,
                        PathExpression* _path, bool _dep, Role* _role,
                        bool _direct_output);

    virtual ~ProjectionTreeNode();

    inline ProjectionTreeNode* getParent() { return parent; } 
    inline int getVar() { return var; }
    inline PathExpression* getPath() { return path; }
    inline bool isDepNode() { return dep; }
    inline Role* getRole() { return role; }
    inline bool isRoot() { return parent==NULL; }
    inline bool isPosDepNode() { return role && role->isPosRole(); }
    inline bool isDosDepNode() { return role && role->isDosRole(); }

    inline vector<ProjectionTreeNode*>* getChildren() { return &children; }
    inline void clearChildren() { children.clear(); }

    bool insertNode(int parent_var, int introduced_var,
                       PathExpression * path, bool dep, Role* role,
                        bool direct_output);
    
    // Here is the description of the additional parameter:
    // @param[in] PassiveProjectionTree* ppt The projection tree to register dropped nodes.
    void removeUnneededNodes(PassiveProjectionTree* ppt);
    
    // Here is the description of the additional parameter:
    // @param[in] PassiveProjectionTree* ppt The projection tree to register dropped nodes.
    void removeRedundantRoles(FSAMap* fsamap,PassiveProjectionTree* ppt);

    void extendProjectionTreeNode();

    void print();

    /*! @brief Register a projection tree node to the passive projection tree.
     *  @details This method is called whenever a projection tree node is deleted.
     *  @param[in] PassiveProjectionTree* ppt The passive projection tree where the
     *             the node is registered.
     *  @retval void
     */
    void registerToPassiveProjectionTree(PassiveProjectionTree* ppt);

    /*! @brief Returns the path from the root to the current node, including its path.
     *  @details NULL paths are ignore. The returned path is a fresh copy (with all path
     *           steps cloned) and must be deleted externally.
     *  @retval PathExpression* The resulting path.
     */
    PathExpression* getRootPath();
    
    /*! @brief Collects the paths from the root down to this node.
     *  @details The passed vector is filled with pointers to the original paths, nothing must be deleted. 
     *  @param[in] vector<PathExpression*>& paths The path vector to be filled.
     *  @retval void
     */
    void getPathsFromRoot(vector<PathExpression*>& paths);
    

private:
    void print(unsigned tabs);

    // Here is the description of the additional parameter:
    // @param[in] PassiveProjectionTree* ppt The projection tree to register dropped nodes.
    void removeSubtreeInclSelf(PassiveProjectionTree* ppt);

    // Here is the description of the additional parameter:
    // @param[in] PassiveProjectionTree* ppt The projection tree to register dropped nodes.
    void removeSubtreeExclSelf(PassiveProjectionTree* ppt);

    void setRedundantRoleSelf();
    bool hasSubtreeMatchingAncestorFSA(FSAMap* fsamap, vector<ProjectionTreeNode*> ancestors);
    
    ProjectionTreeNode* parent;

    int var; // the variable the node represents (-1 if dependency node)

    PathExpression* path;

    bool dep;    // is the node a dependency node?

    Role* role;

    vector<ProjectionTreeNode*> children;

    /*! @var bool direct_output
     *  @brief True if the projection tree node describes a for-expression variable binding
     *         that produces direct output, e.g. in the form of a node construct or constant
     *         string output.
     */
    bool direct_output;
};

#endif // PROJECTIONTREENODE_H
