input metauml;

beginfig(1);

% class declaration (in alphabetic order)
Class.AggregateFunctAvgExpression("AggregateFunctAvgExpression")
  ()
  ("+AggregateFunctAvgExpression(VarExpression* _var)",
   "+AggregateFunctAvgExpression(VarStepExpression* _varstep)",
   "+~AggregateFunctAvgExpression()",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "-calculateValue(OutputStream& eos, Environment* env, unsigned modus, bool& empty_sequence): long double");

Class.AggregateFunctCountExpression("AggregateFunctCountExpression")
  ()
  ("+AggregateFunctCountExpression(VarExpression* _var)",
   "+AggregateFunctCountExpression(VarStepExpression* _varstep)",
   "+~AggregateFunctCountExpression()",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "-calculateValue(OutputStream& eos, Environment* env, unsigned modus, bool& empty_sequence): long double");
  
AbstractClass.AggregateFunctExpression("AggregateFunctExpression")
  ("#var: VarExpression*",
   "#varstep: VarStepExpression*",
   "#forexp: ForExpression*",
   "#bit: BufferIterator*")
  ("+AggregateFunctExpression(EXP_TYPE _type, VarExpression* _var)",
   "+AggregateFunctExpression(EXP_TYPE _type, VarStepExpression* _varstep)",
   "+~AggregateFunctExpression()",
   "+scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars, vector<unsigned>& violating_vars): void",
   "+replaceVarId(unsigned old_id, unsigned new_id): void",
   "+extractFSAMap(FSAMap* fsamap, unsigned parent_var): void",
   "+extractParVarMap(ParVarMap* parvarmap): void",
   "+extractDependencies(vector<DependencySet*>* depset): void",
   "+placeSignOffs(vector<SignOffExpression*>& signoffs): Expression*",
   "+rewriteAggregateFuncts(): void",
   "+print(OutputStream& dos) const: void",
   "+init(BufferNode* root): void",
   "+eval(OutputStream& eos, Environment* env, unsigned modus)=0: void",
   "+isSingleValued(): bool",
   "+prepareOperand(OutputStream& eos, Environment* env, unsigned modus): void",
   "+unprepareOperand(): void",
   "+getNextValue(): Value*",
   "-calculateValue(OutputStream& eos, Environment* env, unsigned modus, bool& empty_sequence)=0: long double");
   
AggregateFunctExpression.info.foreColor := (0.701961,0.701961,0.701961);

Class.AggregateFunctMaxExpression("AggregateFunctMaxExpression")
  ()
  ("+AggregateFunctMaxExpression(VarExpression* _var)",
   "+AggregateFunctMaxExpression(VarStepExpression* _varstep)",
   "+~AggregateFunctMaxExpression()",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "-calculateValue(OutputStream& eos, Environment* env, unsigned modus, bool& empty_sequence): long double");

Class.AggregateFunctMedianExpression("AggregateFunctMedianExpression")
  ()
  ("+AggregateFunctMedianExpression(VarExpression* _var)",
   "+AggregateFunctMedianExpression(VarStepExpression* _varstep)",
   "+~AggregateFunctMedianExpression()",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "-calculateValue(OutputStream& eos, Environment* env, unsigned modus, bool& empty_sequence): long double");
  
Class.AggregateFunctMinExpression("AggregateFunctMinExpression")
  ()
  ("+AggregateFunctMinExpression(VarExpression* _var)",
   "+AggregateFunctMinExpression(VarStepExpression* _varstep)",
   "+~AggregateFunctMinExpression()",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "-calculateValue(OutputStream& eos, Environment* env, unsigned modus, bool& empty_sequence): long double");
  
Class.AggregateFunctStdDevPopExpression("AggregateFunctStdDevPopExpression")
  ()
  ("+AggregateFunctStdDevPopExpression(VarExpression* _var)",
   "+AggregateFunctStdDevPopExpression(VarStepExpression* _varstep)",
   "+~AggregateFunctStdDevPopExpression()",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "-calculateValue(OutputStream& eos, Environment* env, unsigned modus, bool& empty_sequence): long double");
  
Class.AggregateFunctStdDevSampExpression("AggregateFunctStdDevSampExpression")
  ()
  ("+AggregateFunctStdDevSampExpression(VarExpression* _var)",
   "+AggregateFunctStdDevSampExpression(VarStepExpression* _varstep)",
   "+~AggregateFunctStdDevSampExpression()",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "-calculateValue(OutputStream& eos, Environment* env, unsigned modus, bool& empty_sequence): long double");

Class.AggregateFunctSumExpression("AggregateFunctSumExpression")
  ()
  ("+AggregateFunctSumExpression(VarExpression* _var)",
   "+AggregateFunctSumExpression(VarStepExpression* _varstep)",
   "+~AggregateFunctSumExpression()",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "-calculateValue(OutputStream& eos, Environment* env, unsigned modus, bool& empty_sequence): long double");
   
Class.AggregateFunctVarPopExpression("AggregateFunctVarPopExpression")
  ()
  ("+AggregateFunctVarPopExpression(VarExpression* _var)",
   "+AggregateFunctVarPopExpression(VarStepExpression* _varstep)",
   "+~AggregateFunctVarPopExpression()",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "-calculateValue(OutputStream& eos, Environment* env, unsigned modus, bool& empty_sequence): long double");

Class.AggregateFunctVarSampExpression("AggregateFunctVarSampExpression")
  ()
  ("+AggregateFunctVarSampExpression(VarExpression* _var)",
   "+AggregateFunctVarSampExpression(VarStepExpression* _varstep)",
   "+~AggregateFunctVarSampExpression()",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "-calculateValue(OutputStream& eos, Environment* env, unsigned modus, bool& empty_sequence): long double");

leftToRight.bottom(40)(AggregateFunctAvgExpression, AggregateFunctExpression, AggregateFunctVarSampExpression);
topToBottom.right(40)(AggregateFunctAvgExpression, AggregateFunctCountExpression, AggregateFunctMaxExpression, AggregateFunctMedianExpression, AggregateFunctMinExpression);
topToBottom.left(40)(AggregateFunctVarSampExpression, AggregateFunctVarPopExpression, AggregateFunctSumExpression, AggregateFunctStdDevSampExpression, AggregateFunctStdDevPopExpression);

drawObjects(AggregateFunctAvgExpression,
  AggregateFunctCountExpression,
  AggregateFunctExpression,
  AggregateFunctMaxExpression,
  AggregateFunctMedianExpression,
  AggregateFunctMinExpression,
  AggregateFunctStdDevPopExpression,
  AggregateFunctStdDevSampExpression,
  AggregateFunctSumExpression,
  AggregateFunctVarPopExpression,
  AggregateFunctVarSampExpression);

% link declarations
link(inheritance)(pathHorizontal(AggregateFunctAvgExpression.e, AggregateFunctExpression.left));
link(inheritance)(pathManhattanX(AggregateFunctCountExpression.e, AggregateFunctExpression.s));
link(inheritance)(pathManhattanX(AggregateFunctMedianExpression.e, AggregateFunctExpression.s));
link(inheritance)(pathManhattanX(AggregateFunctMaxExpression.e, AggregateFunctExpression.s));
link(inheritance)(pathManhattanX(AggregateFunctMinExpression.e, AggregateFunctExpression.s));
link(inheritance)(pathManhattanX(AggregateFunctStdDevPopExpression.w, AggregateFunctExpression.s));
link(inheritance)(pathManhattanX(AggregateFunctStdDevSampExpression.w, AggregateFunctExpression.s));
link(inheritance)(pathManhattanX(AggregateFunctSumExpression.w, AggregateFunctExpression.s));
link(inheritance)(pathManhattanX(AggregateFunctVarPopExpression.w, AggregateFunctExpression.s));
link(inheritance)(pathHorizontal(AggregateFunctVarSampExpression.w, AggregateFunctExpression.right));

endfig;
end