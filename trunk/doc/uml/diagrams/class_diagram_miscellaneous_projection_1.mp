input metauml;

beginfig(1);

% class declaration (in alphabetic order)
Class.DependencySet("DependencySet")
  ("-var: unsigned",
   "-tuples: vector<DependencyTuple*>")
  ("+DependencySet(unsigned _var)",
   "+~DependencySet()",
   "+insertTuple(PathExpression* path, bool pos, bool dos, bool cond): void",
   "+print(OutputStream& dos): void",
   "+getVar(): unsigned",
   "+getTuples(): vector<DependencyTuple*>*",
   "+hasTuples(): bool",
   "+removeSyntacticallyEqualTuple(PassiveProjectionTree* ppt): void",
   "+removeSemanticallyContainedTuple(PassiveProjectionTree* ppt): void");

Class.DependencyTuple("DependencyTuple")
  ("-path: PathExpression*",
   "-cond: bool",
   "-role: Role*")
  ("+DependencyTuple(PathExpression* _path, bool _cond)",
   "+~DependencyTuple()",
   "+print(OutputStream& dos): void",
   "+getPath(): PathExpression*",
   "+setRole(Role* _role): void",
   "+isCondTuple(): bool",
   "+isPosTuple(): bool",
   "+isDosTuple(): bool",
   "+isSyntacticallyEqualTo(DependencyTuple* tuple): bool",
   "+isSemanticallyContainedIn(DependencyTuple* tuple): bool",
   "+registerToPassiveProjectionTree(unsigned var, PassiveProjectionTree* ppt): void");

Class.ProjectionTree("ProjectionTree")
  ("-root: ProjectionTreeNode*",
   "-roleinfo: RoleList*",
   "-labels: ProjectionTreeLabels*")
  ("+ProjectionTree(VariableTree* _vt, vector<DependencySet*>* _depset)",
   "+~ProjectionTree()",
   "+insertNode(int parent_var, int introduced_var, PathExpression* path, bool dep, Role* role, bool direct_output): void",
   "+print(OutputStream& dos): void",
   "+getRootNode(): ProjectionTreeNode*",
   "+getLabels(): ProjectionTreeLabels*",
   "+labelTree(): void",
   "+removeUnneededNodes(PassiveProjectionTree* ppt): void",
   "+removeRedundantRoles(PassiveProjectionTree* ppt): void",
   "-createProjectionTree(VariableTreeNode* varnode, vector<DependencySet*>* depset): void");

Class.ProjectionTreeLabel("ProjectionTreeLabel")
  ("-parent: ProjectionTreeLabel*",
   "-predecessor: ProjectionTreeLabel*",
   "-n: ProjectionTreeNode*",
   "-position: unsigned",
   "-id: unsigned",
   "-same_level_successor: ProjectionTreeLabel*",
   "-child_successors: vector<ProjectionTreeLabel*>",
   "-self_successors: vector<ProjectionTreeLabel*>")
  ("+ProjectionTreeLabel(ProjectionTreeNode* _n, unsigned _position, unsigned _id, ProjectionTreeLabel* _same_level_successor, vector<ProjectionTreeLabel*> _child_successors, vector<ProjectionTreeLabel*> _self_successors)",
   "+~ProjectionTreeLabel()",
   "+updateParentPredecessorInformation(ProjectionTreeLabel* _parent, ProjectionTreeLabel* _predecessor): void",
   "+print(OutputStream& dos): void",
   "+getParent(): ProjectionTreeLabel*",
   "+getPredecessor(): ProjectionTreeLabel*",
   "+getLeftmostSLPredecessor(): ProjectionTreeLabel*",
   "+getDependingVar(): unsigned",
   "+getPath(): PathExpression*",
   "+getPathStep(): PathStepExpression*",
   "+getTag(): TAG",
   "+getAxisFollowingFSA(): AXIS_TYPE",
   "+getAxisFollowingFSA(unsigned fsa): AXIS_TYPE",
   "+getFSALabel(): ProjectionTreeLabel*",
   "+getId(): unsigned",
   "+getPosition(): unsigned",
   "+getSameLevelSuccessor(): ProjectionTreeLabel*",
   "+getChildSuccessors(): vector<ProjectionTreeLabel*>*",
   "+getSelfSuccessors(): vector<ProjectionTreeLabel*>*",
   "+getProjectionTreeNode(): ProjectionTreeNode*",
   "+isChildOrDescendantLabel(): bool",
   "+isDescendantLabel(): bool",
   "+isChildLabel(): bool",
   "+isDosLabel(): bool",
   "+isDosNodeLabel(): bool",
   "+isDosOrDescendantLabel(): bool",
   "+isNodeLabel(): bool",
   "+isStarLabel(): bool",
   "+hasSameLevelOrChildSuccessors(): bool",
   "+atEndOfPath(): bool",
   "+locateVarAncestorLabel(unsigned var): ProjectionTreeLabel*",
   "+matchesTag(TAG t): bool",
   "+matchesText(): bool",
   "+descendantAxisBetw(unsigned basing_id, bool consider_final): bool");

Class.ProjectionTreeLabels("ProjectionTreeLabels")
  ("-labels: vector<ProjectionTreeLabel*>")
  ("+ProjectionTreeLabels(ProjectionTreeNode* root)",
   "+~ProjectionTreeLabels()",
   "+updateParentPredecessorInformation(): void",
   "+print(OutputStream& dos): void",
   "+getAllRecursiveDosNodeSuccessors(unsigned id, vector<unsigned>&): void",
   "+getLabelById(unsigned id): ProjectionTreeLabel*",
   "+nrOfLabels(): unsigned",
   "-computeProjectionTreeLabels(ProjectionTreeNode* n, unsigned& id, unsigned cur_position): ProjectionTreeLabel*");

Class.ProjectionTreeNode("ProjectionTreeNode")
  ("-parent: ProjectionTreeNode*",
   "-var: int",
   "-path: PathExpression*",
   "-dep: bool",
   "-role: Role*",
   "-children: vector<ProjectionTreeNode*>",
   "-direct_output: bool")
  ("+ProjectionTreeNode(ProjectionTreeNode* _parent, int _var, PathExpression* _path, bool _dep, Role* _role, bool _direct_output)",
   "+~ProjectionTreeNode()",
   "+insertNode(int parent_var, int introduced_var, PathExpression * path, bool dep, Role* role, bool direct_output): bool",
   "+print(OutputStream& dos): void",
   "+getParent(): ProjectionTreeNode*",
   "+getVar(): int",
   "+getPath(): PathExpression*",
   "+getRole(): Role*",
   "+getChildren(): vector<ProjectionTreeNode*>*",
   "+getRootPath(): PathExpression*",
   "+getPathsFromRoot(vector<PathExpression*>& paths): void",
   "+isRoot(): bool",
   "+isDepNode(): bool",
   "+isPosDepNode(): bool",
   "+isDosDepNode(): bool",
   "+clearChildren(): void",
   "+removeUnneededNodes(PassiveProjectionTree* ppt): void",
   "+removeRedundantRoles(PassiveProjectionTree* ppt): void",
   "+registerToPassiveProjectionTree(PassiveProjectionTree* ppt): void",
   "-print(OutputStream& dos, unsigned indents): void",
   "-removeSubtreeInclSelf(PassiveProjectionTree* ppt): void",
   "-removeSubtreeExclSelf(PassiveProjectionTree* ppt): void",
   "-setRedundantRoleSelf(): void");

Class.VariableTree("VariableTree")
  ("-root: VariableTreeNode*")
  ("+VariableTree()",
   "+~VariableTree()",
   "+insertNode(unsigned parent_var, unsigned introduced_var, bool direct_output): void",
   "+print(OutputStream& dos): void",
   "+getRootNode(): VariableTreeNode*");

Class.VariableTreeNode("VariableTreeNode")
  ("-parent: VariableTreeNode*",
   "-var: unsigned",
   "-children: vector<VariableTreeNode*>",
   "-direct_output: bool")
  ("+VariableTreeNode()",
   "+VariableTreeNode(VariableTreeNode* _parent, unsigned _var, bool _direct_output)",
   "+~VariableTreeNode()",
   "+insertNode(unsigned parent_var, unsigned introduced_var, bool direct_output): bool",
   "+print(OutputStream& dos): void",
   "+getVar(): unsigned",
   "+getChildren(): vector<VariableTreeNode*>*",
   "+isDirectOutput(): bool",
   "-print(OutputStream& dos, unsigned indents): void");

leftToRight.bottom(40)(ProjectionTree, ProjectionTreeLabel);
topToBottom.right(40)(VariableTree, VariableTreeNode, ProjectionTree, ProjectionTreeNode);
topToBottom.left(40)(ProjectionTreeLabels, ProjectionTreeLabel, DependencySet, DependencyTuple);

drawObjects(DependencySet,
  DependencyTuple,
  ProjectionTree,
  ProjectionTreeLabel,
  ProjectionTreeLabels,
  ProjectionTreeNode,
  VariableTree,
  VariableTreeNode);

% link declarations
link(composition)(pathManhattanX(ProjectionTreeLabel.n, ProjectionTreeLabels.s));
link(composition)(pathVertical(DependencyTuple.n, DependencySet.bottom));
link(composition)(pathVertical(VariableTreeNode.n, VariableTree.bottom));
link(composition)(pathVertical(ProjectionTreeNode.n, ProjectionTree.bottom));
link(dependency)(pathStepX(ProjectionTree.e, ProjectionTreeLabels.w, 20));
item(iStereo)("<<use>>")(obj.w = .5[ProjectionTree.e, ProjectionTreeLabels.w + (0, 330)]);
link(dependency)(pathStepX(ProjectionTree.e, DependencySet.w, 20));
item(iStereo)("<<use>>")(obj.w = .5[ProjectionTree.e, DependencySet.w]);
link(dependency)(pathStepX(ProjectionTree.nw, VariableTree.w, 20));
item(iStereo)("<<use>>")(obj.w = .5[ProjectionTree.nw - (20, 0), VariableTree.w + (0, 290)]);

endfig;
end