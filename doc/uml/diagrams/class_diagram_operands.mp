input metauml;

beginfig(1);

% class declaration (in alphabetic order)
AbstractClass.AggregateFunctExpression("AggregateFunctExpression")
  ()
  ();

AggregateFunctExpression.info.iAttributeStack.top := 0;
AggregateFunctExpression.info.iAttributeStack.bottom := 0;
AggregateFunctExpression.info.iMethodStack.top := 0;
AggregateFunctExpression.info.iMethodStack.bottom := 0;

AggregateFunctExpression.info.foreColor := (0.701961,0.701961,0.701961);

AbstractClass.ConstExpression("ConstExpression")
  ()
  ("+ConstExpression(EXP_TYPE _type)",
   "+~ConstExpression()",
   "+eval(OutputStream& eos, Environment* env, unsigned modus)=0: void",
   "+isSingleValued(): bool",
   "+prepareOperand(OutputStream& eos, Environment* env, unsigned modus): void",
   "+unprepareOperand(): void",
   "+getNextValue()=0: Value*");

Class.MiscFunctions("MiscFunctions")
  ()
  ("+getNumericFromString(const char* str): static long double",
   "+getStringFromNumerical(long double num): static const char*",
   "+getSummationFrom(long double left, long double right): static long double",
   "+getSubtractionFrom(long double left, long double right): static long double",
   "+getPowerFrom(long double base, long double exponent): static long double",
   "+getRadicalFrom(long double base, long double radix): static long double",
   "+getDivisionFrom(long double numerator, long double denominator): static long double",
   "+getRoundFrom(long double num, unsigned places): static long double",
   "+needsTrim(const char*): static bool",
   "+trim(const char* s): static const char*");

Class.NumericConstExpression("NumericConstExpression")
  ("-num: long double")
  ("+NumericConstExpression(const char* _num)",
   "+~NumericConstExpression()",
   "+print(OutputStream& dos) const: void",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "+getNextValue(): Value*");

AbstractClass.OperandExpression("OperandExpression")
  ("#op_type: EXP_TYPE",
   "#initial: bool",
   "#cur_val: Value")
  ("+OperandExpression(EXP_TYPE _type, EXP_TYPE _op_type)",
   "+~OperandExpression()",
   "+isSingleValued()=0: bool",
   "+prepareOperand(OutputStream& eos, Environment* env, unsigned modus)=0: void",
   "+unprepareOperand()=0: void",
   "+getNextValue()=0: Value*",
   "+getOperandType(): const EXP_TYPE");

OperandExpression.info.foreColor := (0.498039,0.498039,0.498039);

AbstractClass.RoundingExpression("RoundingExpression")
  ()
  ();

RoundingExpression.info.iAttributeStack.top := 0;
RoundingExpression.info.iAttributeStack.bottom := 0;
RoundingExpression.info.iMethodStack.top := 0;
RoundingExpression.info.iMethodStack.bottom := 0;

RoundingExpression.info.foreColor := (0.800000,0.800000,0.800000);

Class.StringConstExpression("StringConstExpression")
  ("-str: const char*")
  ("+StringConstExpression(const char* _str)",
   "+~StringConstExpression()",
   "+print(OutputStream& dos) const: void",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "+getNextValue(): Value*");

Class.Value("Value")
  ("+str_val: const char*",
   "+num_val: long double",
   "+type: VALUE_TYPE")
  ("+Value()",
   "+Value(const char* _str_val)",
   "+Value(long double _num_val)",
   "+~Value()",
   "+getStrVal(): const char*",
   "+getNumVal(): long double",
   "+getType(): VALUE_TYPE",
   "+setStrVal(const char* _str_val): void",
   "+setNumVal(long double _num_val): void",
   "+reset(): void");

AbstractClass.VarExpression("VarExpression")
  ("#id: unsigned",
   "-bit: BufferIterator*")
  ("+VarExpression()",
   "+VarExpression(const char* _name)",
   "+VarExpression(const char* _name, bool _force)",
   "+VarExpression(EXP_TYPE _type, unsigned _id)",
   "+~VarExpression()",
   "+scopeCheck(vector<unsigned>& def_vars, vector<unsigned>& introduced_vars, vector<unsigned>& violating_vars): void",
   "+replaceVarId(unsigned old_id, unsigned new_id): void",
   "+extractDependencies(vector<DependencySet*>* depset): void",
   "+print(OutputStream& dos) const: void",
   "+init(BufferNode* root): void",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "+getId(): unsigned",
   "+setId(unsigned _id): void",
   "+clone(): VarExpression*",
   "+isSingleValued(): bool",
   "+prepareOperand(OutputStream& eos, Environment* env, unsigned modus): void",
   "+unprepareOperand(): void",
   "+getNextValue(): Value*");

Class.VarStepExpression("VarStepExpression")
  ("-path: PathExpression*",
   "-bit: BufferIterator*")
  ("+VarStepExpression(unsigned _id, PathExpression* _path)",
   "+~VarStepExpression()",
   "+extractDependencies(vector<DependencySet*>* depset): void",
   "+print(OutputStream& dos) const: void",
   "+init(BufferNode* root): void",
   "+eval(OutputStream& eos, Environment* env, unsigned modus): void",
   "+getPath(): PathExpression*",
   "+clone(): VarStepExpression*",
   "+cloneWithoutFinalDosNodeAndAttributes(): VarStepExpression*",
   "+isSingleValued(): bool",
   "+prepareOperand(OutputStream& eos, Environment* env, unsigned modus): void",
   "+unprepareOperand(): void",
   "+getNextValue(): Value*");

leftToRight.bottom(40)(MiscFunctions, Value);
leftToRight.bottom(40)(AggregateFunctExpression, OperandExpression, RoundingExpression);
topToBottom.right(40)(AggregateFunctExpression, ConstExpression);
topToBottom.right(120)(ConstExpression, NumericConstExpression);
topToBottom.midx(180)(MiscFunctions, OperandExpression, StringConstExpression);
topToBottom.left(40)(RoundingExpression, VarExpression, VarStepExpression);

drawObjects(AggregateFunctExpression,
  ConstExpression,
  MiscFunctions,
  NumericConstExpression,
  OperandExpression,
  RoundingExpression,
  StringConstExpression,
  Value,
  VarExpression,
  VarStepExpression);

% link declarations
link(dependency)(pathVertical(OperandExpression.n, MiscFunctions.bottom));
item(iStereo)("<<use>>")(obj.w = .5[OperandExpression.n, MiscFunctions.s]);
link(dependency)(pathManhattanX(OperandExpression.e, Value.s));
item(iStereo)("<<use>>")(obj.w = .5[OperandExpression.e + (80, 0), Value.s]);
link(dependency)(pathHorizontal(Value.w, MiscFunctions.right));
item(iStereo)("<<use>>")(obj.w = .5[Value.nw - (18, 50), MiscFunctions.e]);
link(inheritance)(pathHorizontal(AggregateFunctExpression.e, OperandExpression.left));
link(inheritance)(pathManhattanX(ConstExpression.e, OperandExpression.s));
link(inheritance)(pathManhattanX(NumericConstExpression.n, ConstExpression.s));
link(inheritance)(pathManhattanX(StringConstExpression.w, ConstExpression.s));
link(inheritance)(pathVertical(VarStepExpression.n, VarExpression.bottom));
link(inheritance)(pathManhattanX(VarExpression.w, OperandExpression.s));
link(inheritance)(pathHorizontal(RoundingExpression.w, OperandExpression.right));

endfig;
end