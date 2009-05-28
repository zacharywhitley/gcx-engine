input metauml;

beginfig(1);

% class declaration (in alphabetic order)
Class.Executor("Executor")
  ("-debug_ostream: OutputStream*",
   "-eval_ostream: OutputStream*",
   "-query: Expression*",
   "-buffer: Buffer*",
   "-fsamap: FSAMap*",
   "-parvarmap: ParVarMap*",
   "-penv: PathEnvironment*",
   "-vt: VariableTree*",
   "-depset: vector<DependencySet*>*",
   "-pt: ProjectionTree*",
   "-ppt: PassiveProjectionTree*",
   "-pdfa: ProjectionDFA*",
   "-cmd: CmdLine")
  ("+Executor(CmdLine* _cmd)",
   "+~Executor()",
   "+start(): void",
   "-initStreams(): void",
   "-parseQuery(): void",
   "-optimizeQuery(): void",
   "-collectQueryInformation(): void",
   "-buildVariableTree(): void",
   "-buildDependencies(): void",
   "-buildProjectionTree(): void",
   "-buildSignOffQuery(): void",
   "-insertSignOffs(vector<SignOffExpression*>& signoffs): void",
   "-initProjectionDFA(): void",
   "-initStreamParser(): void",
   "-preprocessStream(): void",
   "-evalQuery(): void",
   "-finalDebugPrint(bool sppmode): void");

Note.QUERY("Requires (X)QUERY-Lexer File", "<<query_lexer.l>>", "And (X)QUERY-Parser File", "<<query_parser.y>>");

Class.CmdLine("CmdLine")
  ("+query_istream_type: ISTREAM_TYPE",
   "+xml_istream_type: ISTREAM_TYPE",
   "+eout_ostream_type: OSTREAM_TYPE",
   "+dout_ostream_type: OSTREAM_TYPE",
   "+query_arg: const char*",
   "+xml_arg: const char*",
   "+eout_arg: const char*",
   "+dout_arg: const char*",
   "+debug: bool",
   "+preprocess_stream_debug: bool",
   "+preprocess_stream_no_debug: bool")
  ("+CmdLine(ISTREAM_TYPE _query_istream_type, ISTREAM_TYPE _xml_istream_type, OSTREAM_TYPE _eout_ostream_type, OSTREAM_TYPE _dout_ostream_type, const char* _query_arg, const char* _xml_arg, const char* _eout_arg, const char* _dout_arg, bool _debug, bool _preprocess_stream_debug, bool _preprocess_stream_no_debug)",
   "+~CmdLine()",
   "+checkIntegrity(): void");

Instance.Main("Main")
  ("printFragmentXQ(): void",
   "printVersionNumber(): void",
   "printAbout(): void",
   "invalidCall(const char* exec_name): void",
   "getInputStreamType(const char* arg): ISTREAM_TYPE",
   "getOutputStreamType(const char* arg): OSTREAM_TYPE",
   "executeOnExit(): void",
   "main(int argc, char** argv): int");

Class.PathEnvironment("PathEnvironment")
  ("-env: PEnv")
  ("+PathEnvironment()",
   "+~PathEnvironment()",
   "+insertPath(unsigned var, PathExpression* path): void",
   "+print(OutputStream& dos): void",
   "+getPath(unsigned var): PathExpression*",
   "+getPathBetween(unsigned end, unsigned start): PathExpression*");

leftToRight.bottom(40)(Executor, Main, PathEnvironment);
topToBottom.left(40)(Executor, CmdLine);
topToBottom.midx(40)(QUERY, Main);

drawObjects(Executor,
  QUERY,
  CmdLine,
  Main,
  PathEnvironment);

% link declarations
link(dependency)(pathHorizontal(Main.w, Executor.right));
item(iStereo)("<<use>>")(obj.w = .5[Main.w - (18, 0), Executor.e - (0, 108)]);
link(dependency)(pathVertical(Executor.s, CmdLine.top));
item(iStereo)("<<use>>")(obj.w = .5[Executor.s - (580, 0), CmdLine.n]);
link(dependency)(pathManhattanX(Executor.ne, PathEnvironment.n));
item(iStereo)("<<use>>")(obj.w = .5[Executor.ne, PathEnvironment.n + (0, 232)]);
clink(dashedLink)(QUERY, Executor);

endfig;
end