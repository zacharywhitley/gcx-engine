input metauml;

beginfig(1);

% class declaration (in alphabetic order)
Class.CommentExpression("CommentExpression")
  ("-comment: const char*")
  ("+CommentExpression(const char* _comment)",
   "+~CommentExpression()",
   "+print(OutputStream& dos) const: void");

AbstractClass.CondExpression("CondExpression")
  ()
  ();

CondExpression.info.iAttributeStack.top := 0;
CondExpression.info.iAttributeStack.bottom := 0;
CondExpression.info.iMethodStack.top := 0;
CondExpression.info.iMethodStack.bottom := 0;

CondExpression.info.foreColor := (0.400000,0.400000,0.400000);

Class.DocExpression("DocExpression")
  ("-path: const char*")
  ("+DocExpression()",
   "+~DocExpression()",
   "+print(OutputStream& dos) const: void",
   "+getPath(): const char*",
   "+setPath(const char* _path): void",
   "+isEmptyPath(): bool",
   "+isPathEqualTo(const char* _path): bool");

Class.EmptyExpression("EmptyExpression")
  ()
  ("+EmptyExpression()",
   "+~EmptyExpression()",
   "+print(OutputStream& dos) const: void");

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

AbstractClass.OperandExpression("OperandExpression")
  ()
  ();

OperandExpression.info.iAttributeStack.top := 0;
OperandExpression.info.iAttributeStack.bottom := 0;
OperandExpression.info.iMethodStack.top := 0;
OperandExpression.info.iMethodStack.bottom := 0;

OperandExpression.info.foreColor := (0.498039,0.498039,0.498039);

AbstractClass.PathStepExpression("PathStepExpression")
  ()
  ();

PathStepExpression.info.iAttributeStack.top := 0;
PathStepExpression.info.iAttributeStack.bottom := 0;
PathStepExpression.info.iMethodStack.top := 0;
PathStepExpression.info.iMethodStack.bottom := 0;

PathStepExpression.info.foreColor := (0.600000,0.600000,0.600000);

leftToRight.bottom(40)(CommentExpression, Expression, PathStepExpression);
topToBottom.right(40)(CommentExpression, DocExpression, EmptyExpression);
topToBottom.left(40)(PathStepExpression, OperandExpression, CondExpression);

drawObjects(CommentExpression,
  CondExpression,
  DocExpression,
  EmptyExpression,
  Expression,
  OperandExpression,
  PathStepExpression);

% link declarations
link(inheritance)(pathHorizontal(CommentExpression.e, Expression.left));
link(inheritance)(pathManhattanX(DocExpression.e, Expression.s));
link(inheritance)(pathManhattanX(EmptyExpression.e, Expression.s));
link(inheritance)(pathManhattanX(CondExpression.w, Expression.s));
link(inheritance)(pathManhattanX(OperandExpression.w, Expression.s));
link(inheritance)(pathHorizontal(PathStepExpression.w, Expression.right));

endfig;
end