input metauml;

beginfig(1);

% class declaration (in alphabetic order)
Class.FunctAbsExpression("FunctAbsExpression")
  ()
  ("+FunctAbsExpression(AggregateFunctExpression * _aggfunct)",
   "+~FunctAbsExpression()",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "-calculateValue(OutputStream& eos, Environment* env, unsigned modus, bool& non_value): long double");

Class.FunctCeilingExpression("FunctCeilingExpression")
  ()
  ("+FunctCeilingExpression(AggregateFunctExpression * _aggfunct)",
   "+~FunctCeilingExpression()",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "-calculateValue(OutputStream& eos, Environment* env, unsigned modus, bool& non_value): long double");

Class.FunctCoverExpression("FunctCoverExpression")
  ()
  ("+FunctCoverExpression(AggregateFunctExpression * _aggfunct)",
   "+~FunctCoverExpression()",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "-calculateValue(OutputStream& eos, Environment* env, unsigned modus, bool& non_value): long double");

Class.FunctFloorExpression("FunctFloorExpression")
  ()
  ("+FunctFloorExpression(AggregateFunctExpression * _aggfunct)",
   "+~FunctFloorExpression()",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "-calculateValue(OutputStream& eos, Environment* env, unsigned modus, bool& non_value): long double");

Class.FunctRoundExpression("FunctRoundExpression")
  ()
  ("+FunctRoundExpression(AggregateFunctExpression * _aggfunct)",
   "+~FunctRoundExpression()",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "-calculateValue(OutputStream& eos, Environment* env, unsigned modus, bool& non_value): long double");

Class.FunctRoundHalfToEvenExpression("FunctRoundHalfToEvenExpression")
  ()
  ("+FunctRoundHalfToEvenExpression(AggregateFunctExpression * _aggfunct)",
   "+~FunctRoundHalfToEvenExpression()",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "-calculateValue(OutputStream& eos, Environment* env, unsigned modus, bool& non_value): long double");

Class.FunctTruncateExpression("FunctTruncateExpression")
  ()
  ("+FunctTruncateExpression(AggregateFunctExpression * _aggfunct)",
   "+~FunctTruncateExpression()",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "-calculateValue(OutputStream& eos, Environment* env, unsigned modus, bool& non_value): long double");

AbstractClass.RoundingExpression("RoundingExpression")
  ("#aggfunct: AggregateFunctExpression*")
  ("+RoundingExpression(EXP_TYPE _type, AggregateFunctExpression* _aggfunct)",
   "+~RoundingExpression()",
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
   "-calculateValue(OutputStream& eos, Environment* env, unsigned modus, bool& non_value)=0: long double");
   
RoundingExpression.info.foreColor := (0.800000,0.800000,0.800000);

leftToRight.bottom(40)(FunctAbsExpression, RoundingExpression, FunctTruncateExpression);
topToBottom.right(40)(FunctAbsExpression, FunctCeilingExpression, FunctCoverExpression, FunctFloorExpression);
topToBottom.left(40)(FunctTruncateExpression, FunctRoundHalfToEvenExpression, FunctRoundExpression);

drawObjects(FunctAbsExpression,
  FunctCeilingExpression,
  FunctCoverExpression,
  FunctFloorExpression,
  FunctRoundExpression,
  FunctRoundHalfToEvenExpression,
  FunctTruncateExpression,
  RoundingExpression);

% link declarations
link(inheritance)(pathHorizontal(FunctAbsExpression.e, RoundingExpression.left));
link(inheritance)(pathManhattanX(FunctCeilingExpression.e, RoundingExpression.s));
link(inheritance)(pathManhattanX(FunctCoverExpression.e, RoundingExpression.s));
link(inheritance)(pathManhattanX(FunctFloorExpression.e, RoundingExpression.s));
link(inheritance)(pathManhattanX(FunctRoundExpression.w, RoundingExpression.s));
link(inheritance)(pathManhattanX(FunctRoundHalfToEvenExpression.w, RoundingExpression.s));
link(inheritance)(pathHorizontal(FunctTruncateExpression.w, RoundingExpression.right));

endfig;
end