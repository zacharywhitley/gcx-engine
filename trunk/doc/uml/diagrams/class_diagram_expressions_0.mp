input metauml;

beginfig(1);

% class declaration (in alphabetic order)
AbstractClass.Expression("Expression")
  ("#tagmap: TagMap*",
   "#type: EXP_TYPE")
  ("+Expression(EXP_TYPE _type)",
   "+~Expression()",
   "+scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars, vector<unsigned>& violating_vars): void",
   "+replaceVarId(unsigned old_id, unsigned new_id): void",
   "+mergeSequences(): void",
   "+extractFSAMap(FSAMap* fsamap, unsigned parent_var): void",
   "+extractParVarMap(ParVarMap* parvarmap): void",
   "+extractDependencies(vector<DependencySet*>* depset): void",
   "+placeSignOffs(vector<SignOffExpression*>& signoffs): Expression*",
   "+rewriteWhereExps(): void",
   "+rewriteEmptyFuncts(): void",
   "+rewriteVarstepCondExps(): void",
   "+rewriteAggregateFuncts(): void",
   "+rewriteVarsteps(): void",
   "+print(OutputStream& dos) const=0: void",
   "+init(BufferNode* root): void",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "+getType(): const EXP_TYPE",
   "+setType(EXP_TYPE _type): void",
   "+containsDirectOutput(): bool");

Class.ForExpression("ForExpression")
  ("-var: VarExpression*",
   "-varexp: VarExpression*",
   "-varstep: VarStepExpression*",
   "-exp: Expression*",
   "-bit: BufferIterator*")
  ("+ForExpression(VarExpression* _var, VarExpression* _varexp, Expression* _exp)",
   "+ForExpression(VarExpression* _var, VarStepExpression* _varstep, Expression* _exp)",
   "+~ForExpression()",
   "+scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars, vector<unsigned>& violating_vars): void",
   "+replaceVarId(unsigned old_id, unsigned new_id): void",
   "+mergeSequences(): void",
   "+extractFSAMap(FSAMap* fsamap, unsigned parent_var): void",
   "+extractParVarMap(ParVarMap* parvarmap): void",
   "+extractDependencies(vector<DependencySet*>* depset): void",
   "+placeSignOffs(vector<SignOffExpression*>& signoffs): Expression*",
   "+rewriteWhereExps(): void",
   "+rewriteEmptyFuncts(): void",
   "+rewriteVarstepCondExps(): void",
   "+rewriteAggregateFuncts(): void",
   "+rewriteVarsteps(): void",
   "+print(OutputStream& dos) const: void",
   "+init(BufferNode* root): void",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "+evalSignOffForBinding(OutputStream& eos, Environment* env, BufferNode* cur): void",
   "+containsDirectOutput(): bool");

Class.IfExpression("IfExpression")
  ("-cond: CondExpression*",
   "-thenexp: Expression*",
   "-elseexp: Expression*")
  ("+IfExpression(CondExpression* _cond, Expression* _thenexp, Expression* _elseexp)",
   "+~IfExpression()",
   "+scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars, vector<unsigned>& violating_vars): void",
   "+replaceVarId(unsigned old_id, unsigned new_id): void",
   "+mergeSequences(): void",
   "+extractFSAMap(FSAMap* fsamap, unsigned parent_var): void",
   "+extractParVarMap(ParVarMap* parvarmap): void",
   "+extractDependencies(vector<DependencySet*>* depset): void",
   "+placeSignOffs(vector<SignOffExpression*>& signoffs): Expression*",
   "+rewriteWhereExps(): void",
   "+rewriteEmptyFuncts(): void",
   "+rewriteVarstepCondExps(): void",
   "+rewriteAggregateFuncts(): void",
   "+rewriteVarsteps(): void",
   "+print(OutputStream& dos) const: void",
   "+init(BufferNode* root): void",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "+containsDirectOutput(): bool");

Class.NodeConstructExpression("NodeConstructExpression")
  ("-tag: TAG",
   "-exp: Expression*")
  ("+NodeConstructExpression(const char* _tag, Expression* _exp)",
   "+~NodeConstructExpression()",
   "+scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars, vector<unsigned>& violating_vars): void",
   "+replaceVarId(unsigned old_id, unsigned new_id): void",
   "+mergeSequences(): void",
   "+extractFSAMap(FSAMap* fsamap, unsigned parent_var): void",
   "+extractParVarMap(ParVarMap* parvarmap): void",
   "+extractDependencies(vector<DependencySet*>* depset): void",
   "+placeSignOffs(vector<SignOffExpression*>& signoffs): Expression*",
   "+rewriteWhereExps(): void",
   "+rewriteEmptyFuncts(): void",
   "+rewriteVarstepCondExps(): void",
   "+rewriteAggregateFuncts(): void",
   "+rewriteVarsteps(): void",
   "+print(OutputStream& dos) const: void",
   "+init(BufferNode* root): void",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void");

Class.PathExpression("PathExpression")
  ("-pathsteps: vector<PathStepExpression*>",
   "-adornment: PathExpressionAdornment*")
  ("+PathExpression()",
   "+~PathExpression()",
   "+addPathStep(PathStepExpression* ps): void",
   "+print(OutputStream& dos) const: void",
   "+getPathSteps(): vector<PathStepExpression*>*",
   "+getPathSize(): unsigned",
   "+getPathStepAt(unsigned i): PathStepExpression*",
   "+getTailPathStep(): PathStepExpression*",
   "+getPathStepAfterTextNodeTest(): PathStepExpression*",
   "+getWeight(): unsigned",
   "+getAdornment(): PathExpressionAdornment*",
   "+isEmptyPath(): bool",
   "+isDosNodePath(): bool",
   "+isSyntacticallyEqualTo(PathExpression* path): bool",
   "+isSemanticallyContainedIn(PathExpression* path): bool",
   "+hasInnerTextNodeTest(): bool",
   "+hasTerminatingTextNodeTest(): bool",
   "+hasFollowingDescendantOrDosFrom(unsigned ps_idx): bool",
   "+hasPreviousDescendantOrDosUpTo(unsigned ps_idx): bool",
   "+selectsNoNode(): bool",
   "+containsStarDescendantSequence(unsigned pos): bool",
   "+mightHasChildDescendantConflict(PathExpression* path): bool",
   "+replacePathStepAt(unsigned idx, PathStepExpression* ps): void",
   "+clone(): PathExpression*",
   "+cloneWithoutFinalDosNodeAndAttributes(): PathExpression*");

Class.PathExpressionAdornment("PathExpressionAdornment")
  ("-adorned_path: PathExpression*",
   "-rewritten_path: PathExpression*",
   "-path_adornments: vector<unsigned>",
   "-is_child_node_path: bool",
   "-is_descendant_node_path: bool",
   "-is_dos_node_path: bool",
   "-is_child_text_path: bool",
   "-is_descendant_text_path: bool")
  ("+PathExpressionAdornment(PathExpression* _path)",
   "+~PathExpressionAdornment()",
   "+print(OutputStream& dos) const: void",
   "+getAdornedPath(): PathExpression*",
   "+getRewrittenPath(): PathExpression*",
   "+getPathAdornments(): vector<unsigned>*",
   "+isChildNodePath(): bool",
   "+isDescendantNodePath(): bool",
   "+isDosNodePath(): bool",
   "+isChildTextPath(): bool",
   "+isDescendantTextPath(): bool");

Class.SequenceExpression("SequenceExpression")
  ("-exps: vector<Expression*>",
   "-delete_recursively: bool")
  ("+SequenceExpression(Expression* _left, Expression* _right)",
   "+~SequenceExpression()",
   "+scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars, vector<unsigned>& violating_vars): void",
   "+replaceVarId(unsigned old_id, unsigned new_id): void",
   "+mergeSequences(): void",
   "+extractFSAMap(FSAMap* fsamap, unsigned parent_var): void",
   "+extractParVarMap(ParVarMap* parvarmap): void",
   "+extractDependencies(vector<DependencySet*>* depset): void",
   "+placeSignOffs(vector<SignOffExpression*>& signoffs): Expression*",
   "+rewriteWhereExps(): void",
   "+rewriteEmptyFuncts(): void",
   "+rewriteVarstepCondExps(): void",
   "+rewriteAggregateFuncts(): void",
   "+rewriteVarsteps(): void",
   "+print(OutputStream& dos) const: void",
   "+init(BufferNode* root): void",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "+insertExp(Expression* exp): void",
   "+unsetRecursiveDelete(): void",
   "+containsDirectOutput(): bool",
   "-getExps(): vector<Expression*>*");

Class.SignOffExpression("SignOffExpression")
  ("-var: VarExpression*",
   "-varstep: VarStepExpression*",
   "-exec_var: VarExpression*",
   "-exec_varstep: VarStepExpression*",
   "-role: Role*",
   "-bit: BufferIterator*")
  ("+SignOffExpression(VarExpression* _var, Role* _role)",
   "+SignOffExpression(VarStepExpression* _varstep, Role* _role)",
   "+~SignOffExpression()",
   "+scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars, vector<unsigned>& violating_vars): void",
   "+replaceVarId(unsigned old_id, unsigned new_id): void",
   "+print(OutputStream& dos) const: void",
   "+init(BufferNode* root): void",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "+getVar(): unsigned");

Class.WhereExpression("WhereExpression")
  ("-cond: CondExpression*",
   "-exp: Expression*",
   "-ifexp: IfExpression*")
  ("+WhereExpression(CondExpression* _cond, Expression* _exp)",
   "+~WhereExpression()",
   "+scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars, vector<unsigned>& violating_vars): void",
   "+replaceVarId(unsigned old_id, unsigned new_id): void",
   "+mergeSequences(): void",
   "+extractFSAMap(FSAMap* fsamap, unsigned parent_var): void",
   "+extractParVarMap(ParVarMap* parvarmap): void",
   "+extractDependencies(vector<DependencySet*>* depset): void",
   "+placeSignOffs(vector<SignOffExpression*>& signoffs): Expression*",
   "+rewriteWhereExps(): void",
   "+rewriteEmptyFuncts(): void",
   "+rewriteVarstepCondExps(): void",
   "+rewriteAggregateFuncts(): void",
   "+rewriteVarsteps(): void",
   "+print(OutputStream& dos) const: void",
   "+init(BufferNode* root): void",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "+containsDirectOutput(): bool");

leftToRight.bottom(40)(ForExpression, Expression, WhereExpression);
topToBottom.right(40)(ForExpression, IfExpression, NodeConstructExpression, SignOffExpression);
topToBottom.left(40)(WhereExpression, PathExpression, PathExpressionAdornment, SequenceExpression);

drawObjects(Expression,
  ForExpression,
  IfExpression,
  NodeConstructExpression,
  PathExpression,
  PathExpressionAdornment,
  SequenceExpression,
  SignOffExpression,
  WhereExpression);

% link declarations
link(dependency)(pathVertical(PathExpression.s, PathExpressionAdornment.top));
item(iStereo)("<<use>>")(obj.w = .5[PathExpression.s + (25, 0), PathExpressionAdornment.n]);
link(inheritance)(pathHorizontal(ForExpression.e, Expression.left));
link(inheritance)(pathManhattanX(IfExpression.e, Expression.s));
link(inheritance)(pathManhattanX(NodeConstructExpression.e, Expression.s));
link(inheritance)(pathManhattanX(SignOffExpression.e, Expression.s));
link(inheritance)(pathManhattanX(PathExpression.w, Expression.s));
link(inheritance)(pathManhattanX(SequenceExpression.w, Expression.s));
link(inheritance)(pathHorizontal(WhereExpression.w, Expression.right));

endfig;
end