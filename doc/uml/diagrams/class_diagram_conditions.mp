input metauml;

beginfig(1);

% class declaration (in alphabetic order)
Class.AndCondExpression("AndCondExpression")
  ("-left: CondExpression*",
   "-right: CondExpression*")
  ("+AndCondExpression(CondExpression* _left, CondExpression* _right)",
   "+~AndCondExpression()",
   "+scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_var, vector<unsigned>& violating_vars): void",
   "+replaceVarId(unsigned old_id, unsigned new_id): void",
   "+extractFSAMap(FSAMap* fsamap, unsigned parent_var): void",
   "+extractParVarMap(ParVarMap* parvarmap): void",
   "+extractDependencies(vector<DependencySet*>* depset): void",
   "+placeSignOffs(vector<SignOffExpression*>& signoffs): Expression*",
   "+rewriteEmptyFuncts(): void",
   "+rewriteVarstepCondExps(): void",
   "+rewriteAggregateFuncts(): void",
   "+print(OutputStream& dos) const: void",
   "+init(BufferNode* root): void",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "+evalCond(OutputStream& eos, Environment* env, unsigned modus): bool");
   
AbstractClass.CondExpression("CondExpression")
  ()
  ("+CondExpression(EXP_TYPE _type)",
   "+~CondExpression()",
   "+evalCond(OutputStream& eos, Environment* env, unsigned modus)=0: bool");

CondExpression.info.foreColor := (0.400000,0.400000,0.400000);

Class.CondOperandExpression("CondOperandExpression")
  ("-left: OperandExpression*",
   "-op: COMP_TYPE",
   "-right: OperandExpression*")
  ("+CondOperandExpression(OperandExpression* _left, COMP_TYPE _op, OperandExpression* _right)",
   "+~CondOperandExpression()",
   "+scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars, vector<unsigned>& violating_vars): void",
   "+replaceVarId(unsigned old_id, unsigned new_id): void",
   "+extractFSAMap(FSAMap* fsamap, unsigned parent_var): void",
   "+extractParVarMap(ParVarMap* parvarmap): void",
   "+extractDependencies(vector<DependencySet*>* depset): void",
   "+placeSignOffs(vector<SignOffExpression*>& signoffs): Expression*",
   "+rewriteEmptyFuncts(): void",
   "+rewriteVarstepCondExps(): void",
   "+rewriteAggregateFuncts(): void",
   "+print(OutputStream& dos) const: void",
   "+init(BufferNode* root): void",
   "+evalCond(OutputStream& eos, Environment* env, unsigned modus): bool",
   "-evalRelOp(Value* left, COMP_TYPE op, Value* right): bool",
   "-invertOperator(COMP_TYPE ot): COMP_TYPE");

Class.EmptyCondExpression("EmptyCondExpression")
  ("-var: VarExpression*",
   "-varstep: VarStepExpression*",
   "-ncond: NotCondExpression*")
  ("+EmptyCondExpression(VarExpression* _var)",
   "+EmptyCondExpression(VarStepExpression* _varstep)",
   "+~EmptyCondExpression()",
   "+scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars, vector<unsigned>& violating_vars): void",
   "+replaceVarId(unsigned old_id, unsigned new_id): void",
   "+extractFSAMap(FSAMap* fsamap, unsigned parent_var): void",
   "+extractParVarMap(ParVarMap* parvarmap): void",   
   "+extractDependencies(vector<DependencySet*>* depset): void",
   "+placeSignOffs(vector<SignOffExpression*>& signoffs): Expression*",
   "+rewriteEmptyFuncts(): void",
   "+rewriteAggregateFuncts(): void",
   "+print(OutputStream& dos) const: void",
   "+init(BufferNode* root): void",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "+evalCond(OutputStream& eos, Environment* env, unsigned modus): bool");

Class.ExistsCondExpression("ExistsCondExpression")
  ("-var: VarExpression*",
   "-varstep: VarStepExpression*",
   "-bit: BufferIterator*")
  ("+ExistsCondExpression(VarExpression* _var)",
   "+ExistsCondExpression(VarStepExpression* _varstep)",
   "+~ExistsCondExpression()",
   "+scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars, vector<unsigned>& violating_vars): void",
   "+replaceVarId(unsigned old_id, unsigned new_id): void",
   "+extractDependencies(vector<DependencySet*>* depset): void",
   "+print(OutputStream& dos) const: void",
   "+init(BufferNode* root): void",
   "+evalCond(OutputStream& eos, Environment* env, unsigned modus): bool");
   
Class.FalseCondExpression("FalseCondExpression")
  ()
  ("+FalseCondExpression()",
   "+~FalseCondExpression()",
   "+print(OutputStream& dos) const: void",
   "+evalCond(OutputStream& eos, Environment* env, unsigned modus): bool");
   
Class.NotCondExpression("NotCondExpression")
  ("-exp: CondExpression*")
  ("+NotCondExpression(CondExpression* _exp)",
   "+~NotCondExpression()",
   "+scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars, vector<unsigned>& violating_vars): void",
   "+replaceVarId(unsigned old_id, unsigned new_id): void",
   "+extractFSAMap(FSAMap* fsamap, unsigned parent_var): void",
   "+extractParVarMap(ParVarMap* parvarmap): void",
   "+extractDependencies(vector<DependencySet*>* depset): void",
   "+placeSignOffs(vector<SignOffExpression*>& signoffs): Expression*",
   "+rewriteEmptyFuncts(): void",
   "+rewriteVarstepCondExps(): void",
   "+rewriteAggregateFuncts(): void",
   "+print(OutputStream& dos) const: void",
   "+init(BufferNode* root): void",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "+evalCond(OutputStream& eos, Environment* env, unsigned modus): bool");    

Class.OrCondExpression("OrCondExpression")
  ("-left: CondExpression*",
   "-right: CondExpression*")
  ("+OrCondExpression(CondExpression* _left, CondExpression* _right)",
   "+~OrCondExpression()",
   "+scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars, vector<unsigned>& violating_vars): void",
   "+replaceVarId(unsigned old_id, unsigned new_id): void",
   "+extractFSAMap(FSAMap* fsamap, unsigned parent_var): void",
   "+extractParVarMap(ParVarMap* parvarmap): void",
   "+extractDependencies(vector<DependencySet*>* depset): void",
   "+placeSignOffs(vector<SignOffExpression*>& signoffs): Expression*",
   "+rewriteEmptyFuncts(): void",
   "+rewriteVarstepCondExps(): void",
   "+rewriteAggregateFuncts(): void",
   "+print(OutputStream& dos) const: void",
   "+init(BufferNode* root): void",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "+evalCond(OutputStream& eos, Environment* env, unsigned modus): bool");

Class.TrueCondExpression("TrueCondExpression")
  ()
  ("+TrueCondExpression()",
   "+~TrueCondExpression()",
   "+print(OutputStream& dos) const: void",
   "+evalCond(OutputStream& eos, Environment* env, unsigned modus): bool");

Class.VarstepCondExpression("VarstepCondExpression")
  ("-var: VarExpression*",
   "-varstep: VarStepExpression*",
   "-econd: ExistsCondExpression")
  ("+VarstepCondExpression(VarExpression* _var)",
   "+VarstepCondExpression(VarStepExpression* _varstep)",
   "+~VarstepCondExpression()",
   "+scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars, vector<unsigned>& violating_vars): void",
   "+replaceVarId(unsigned old_id, unsigned new_id): void",
   "+extractDependencies(vector<DependencySet*>* depset): void",
   "+rewriteVarstepCondExps(): void",
   "+print(OutputStream& dos) const: void",
   "+init(BufferNode* root): void",
   "+evalCond(OutputStream& eos, Environment* env, unsigned modus): bool");

leftToRight.top(40)(AndCondExpression, CondExpression, VarstepCondExpression);
topToBottom.right(40)(AndCondExpression, CondOperandExpression, EmptyCondExpression, ExistsCondExpression);
topToBottom.left(40)(VarstepCondExpression, TrueCondExpression, OrCondExpression, NotCondExpression, FalseCondExpression);

drawObjects(AndCondExpression,
  CondExpression,
  CondOperandExpression,
  EmptyCondExpression,
  ExistsCondExpression,
  FalseCondExpression,
  NotCondExpression,
  OrCondExpression,
  TrueCondExpression,
  VarstepCondExpression);

% link declarations
link(inheritance)(pathManhattanX(AndCondExpression.e, CondExpression.s));
link(inheritance)(pathManhattanX(CondOperandExpression.e, CondExpression.s));
link(inheritance)(pathManhattanX(EmptyCondExpression.e, CondExpression.s));
link(inheritance)(pathManhattanX(ExistsCondExpression.e, CondExpression.s));
link(inheritance)(pathManhattanX(FalseCondExpression.w, CondExpression.s));
link(inheritance)(pathManhattanX(NotCondExpression.w, CondExpression.s));
link(inheritance)(pathManhattanX(OrCondExpression.w, CondExpression.s));
link(inheritance)(pathManhattanX(TrueCondExpression.w, CondExpression.s));
link(inheritance)(pathManhattanX(VarstepCondExpression.w, CondExpression.s));

endfig;
end