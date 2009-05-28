input metauml;

beginfig(1);

% class declaration (in alphabetic order)
Class.PassiveProjectionTree("PassiveProjectionTree")
  ("-root: PassiveProjectionTreeNode*",
   "-penv: PathEnvironment*")
  ("+PassiveProjectionTree(PathEnvironment* _penv)",
   "+~PassiveProjectionTree()",
   "+registerPath(PathExpression* path): void",
   "+print(OutputStream& dos): void",
   "+getRootNode(): PassiveProjectionTreeNode*",
   "+getPathEnv(): PathEnvironment*");

Class.PassiveProjectionTreeConfiguration("PassiveProjectionTreeConfiguration")
  ("-active_nodes: vector<PassiveProjectionTreeNode*>",
   "-passive_nodes: vector<PassiveProjectionTreeNode*>")
  ("+PassiveProjectionTreeConfiguration(PassiveProjectionTree* ppt)",
   "+PassiveProjectionTreeConfiguration(vector<PassiveProjectionTreeNode*> _passive_nodes)",
   "+~PassiveProjectionTreeConfiguration()",
   "+addActiveNode(PassiveProjectionTreeNode* n): void",
   "+addPassiveNode(PassiveProjectionTreeNode* n): void",
   "+applyTag(TAG t): PassiveProjectionTreeConfiguration*",
   "+applyText(): PassiveProjectionTreeConfiguration*",
   "+hasActiveNodes(): bool");

Class.PassiveProjectionTreeNode("PassiveProjectionTreeNode")
  ("-parent: PassiveProjectionTreeNode*",
   "-label: PathStepExpression*",
   "-children: vector<PassiveProjectionTreeNode*>")
  ("+PassiveProjectionTreeNode()",
   "+PassiveProjectionTreeNode(PassiveProjectionTreeNode* _parent, PathStepExpression* _label)",
   "+~PassiveProjectionTreeNode()",
   "+registerPath(PathExpression* path, unsigned pos): void",
   "+print(OutputStream& dos): void",
   "+getParent(): PassiveProjectionTreeNode*",
   "+getChildren(): vector<PassiveProjectionTreeNode*>*",
   "+getLabel(): PathStepExpression*",
   "-print(OutputStream& dos, unsigned indents): void");

Class.ProjectionDFA("ProjectionDFA")
  ("-initial: ProjectionDFAState*")
  ("+ProjectionDFA(ProjectionTree* _pt, PassiveProjectionTree* _ppt)",
   "+~ProjectionDFA()",
   "+print(OutputStream& dos): void",
   "+getInitialState(): ProjectionDFAState*");

Class.ProjectionDFAState("ProjectionDFAState")
  ("-parent: ProjectionDFAState*",
   "-config: TokenConfiguration*",
   "-pconfig: PassiveProjectionTreeConfiguration*",
   "-transitions: ProjectionDFATransitions",
   "-keep_node: bool",
   "-keep_subtree: bool",
   "-is_output: bool",
   "-skip_subtree: bool",
   "-roles: vector<Role*>",
   "-role_counts: vector<unsigned>",
   "-cumulative_roles: vector<unsigned>",
   "-non_cumulative_roles: vector<unsigned>")
  ("+ProjectionDFAState(ProjectionTree* _pt, PassiveProjectionTree* _ppt)",
   "+ProjectionDFAState(ProjectionDFAState* _parent, PassiveProjectionTreeConfiguration* _pconf)",
   "+ProjectionDFAState(ProjectionDFAState* _parent, TokenConfiguration* _config, PassiveProjectionTreeConfiguration* _pconfig)",
   "+~ProjectionDFAState()",
   "+update(): void",
   "+print(OutputStream& dos): void",
   "+getTokenConfiguration(): TokenConfiguration*",
   "+getPassiveProjectionTreeConfiguration(): PassiveProjectionTreeConfiguration*",
   "+getCumulativeRoles(): vector<unsigned>*",
   "+getNonCumulativeRoles(): vector<unsigned>*",
   "+isOutput(): bool",
   "+isRoot(): bool",
   "+traceBack(): ProjectionDFAState*",
   "+takeTransition(TAG t,short& t_type): ProjectionDFAState*",
   "+takeTextTransition(): ProjectionDFAState*",
   "+keepNode(): bool",
   "+keepSubtree(): bool",
   "+skipSubtree(): bool",
   "+print(OutputStream& dos, unsigned indents, bool is_text): void",
   "-computeTransition(TAG t): void");

Class.ProjectionDFATransitions("ProjectionDFATransitions")
  ("-parent: ProjectionDFAState*",
   "-transitions: hash_map<TAG, ProjectionDFAState*>",
   "-text_transition_computed: bool",
   "-text_transition: ProjectionDFAState*")
  ("+ProjectionDFATransitions(ProjectionDFAState* _parent)",
   "+~ProjectionDFATransitions()",
   "+print(OutputStream& dos, unsigned indents): void",
   "+takeTransition(TAG t, short& t_type): ProjectionDFAState*",
   "+takeTextTransition(bool is_root): ProjectionDFAState*",
   "+computeTransition(ProjectionDFAState* parent, TAG t): void",
   "-computeTextTransition(bool is_root): void");

Class.TokenConfiguration("TokenConfiguration")
  ("-parent: TokenConfiguration*",
   "-labels: ProjectionTreeLabels*",
   "-active_tokens: vector<unsigned>",
   "-passive_tokens: vector<unsigned>")
  ("+TokenConfiguration(ProjectionTree* pt)",
   "+TokenConfiguration(TokenConfiguration* parent)",
   "+~TokenConfiguration()",
   "+createRoleList(vector<Role*>& roles, vector<unsigned>& role_counts): void",
   "+applyTag(TAG t): TokenConfiguration*",
   "+applyText(): TokenConfiguration*",
   "+addPassiveTokens(unsigned i, unsigned n): void",
   "+print(OutputStream& dos, bool is_text): void",
   "+getActiveTokens(): vector <unsigned>",
   "+setActiveTokens(unsigned i, unsigned n): void",
   "+getPassiveTokens(): vector<unsigned>",
   "+setPassiveTokens(unsigned i, unsigned n): void",
   "+getActiveTokensById(unsigned i): unsigned",
   "+getPassiveTokensById(unsigned i): unsigned",
   "+getLastActiveTokenCountFor(unsigned token_id): unsigned",
   "+getLabels(): ProjectionTreeLabels*",
   "+isEmpty(): bool",
   "+isOutput(): bool",
   "+hasActiveToken(): bool",
   "+sumUpActiveTokenCountFor(unsigned token_id): unsigned",
   "+forceChildKeep(): bool",
   "+keepSubtree(): bool",
   "+skipSubtree(): bool");

leftToRight.bottom(40)(ProjectionDFA, PassiveProjectionTree);
topToBottom.right(40)(ProjectionDFA, ProjectionDFAState, ProjectionDFATransitions);
topToBottom.left(15)(PassiveProjectionTree, PassiveProjectionTreeConfiguration, PassiveProjectionTreeNode, TokenConfiguration);

drawObjects(PassiveProjectionTree,
  PassiveProjectionTreeConfiguration,
  PassiveProjectionTreeNode,
  ProjectionDFA,
  ProjectionDFAState,
  ProjectionDFATransitions,
  TokenConfiguration);

% link declarations
link(composition)(pathStepX(PassiveProjectionTreeNode.e, PassiveProjectionTree.e, 20));
link(dependency)(pathVertical(ProjectionDFA.s, ProjectionDFAState.top));
item(iStereo)("<<use>>")(obj.w = .5[ProjectionDFA.s + (128, 0), ProjectionDFAState.n]);
link(dependency)(pathManhattanX(ProjectionDFAState.s, ProjectionDFATransitions.n));
item(iStereo)("<<use>>")(obj.w = .5[ProjectionDFAState.s + (138, 0), ProjectionDFATransitions.n]);
link(dependency)(pathStepX(ProjectionDFAState.e, PassiveProjectionTreeConfiguration.w, 20));
item(iStereo)("<<use>>")(obj.w = .5[ProjectionDFAState.e, PassiveProjectionTreeConfiguration.w]);
link(dependency)(pathStepX(ProjectionDFAState.e, TokenConfiguration.w, 20));
item(iStereo)("<<use>>")(obj.w = .5[ProjectionDFAState.e, TokenConfiguration.w + (0, 63)]);

endfig;
end