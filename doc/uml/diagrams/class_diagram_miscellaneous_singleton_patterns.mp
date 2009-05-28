input metauml;

beginfig(1);

% class declaration (in alphabetic order)
Class.FSAMap("FSAMap")
  ("-instance: static FSAMap*",
   "-fsamap: VarVarMap")
  ("+getInstance(): static FSAMap*",
   "+~FSAMap()",
   "+insertFSA(unsigned var, unsigned fsa): void",
   "+print(OutputStream& dos): void",
   "+getFSA(unsigned var): unsigned",
   "-FSAMap()");

Instance.Loader("Loader")
  ("saxBeginElement(const char* _tag): void",
   "saxBeginEndElement(const char* _tag): void",
   "saxEndElement(const char* _tag): void",
   "saxEndElement(): void",
   "saxCharacters(const char* _text): void",
   "yyxmllex(): extern int",
   "yyxmllex_init(): extern int",
   "saxparse(): int",
   "yyxmllex_destroy(): extern int",
   "destroyLexer(): void");

Note.SAX("Requires SAX-Parser File", "<<sax.l>>");

Class.ParVarMap("ParVarMap")
  ("-instance: static ParVarMap*",
   "-parvarmap: VarVarPathMap",
   "-varsq: vector<unsigned>",
   "-direct_output: vector<bool>")
  ("+getInstance(): static ParVarMap*",
   "+~ParVarMap()",
   "+insertParVar(unsigned var, unsigned parvar, PathExpression* path, bool _direct_output): void",
   "+print(OutputStream& dos): void",
   "+getParVar(unsigned var): unsigned",
   "+getPath(unsigned var): PathExpression*",
   "+getVarsq(): vector<unsigned>*",
   "+getDirectOutputInfo(): vector<bool>*",
   "-ParVarMap()");

Class.Role("Role")
  ("-id: unsigned",
   "-type: ROLE_TYPE",
   "-basing_var: unsigned",
   "-basing_fsa: unsigned",
   "-rel_path: PathExpression*",
   "-var_path: PathExpression*",
   "-concat_path: PathExpression*")
  ("+Role(unsigned _id, ROLE_TYPE _type, unsigned _basing_var, unsigned _basing_fsa, PathExpression* _rel_path, PathExpression* _var_path)",
   "+~Role()",
   "+print(OutputStream& dos) const: void",
   "+printExtended(OutputStream& dos): void",
   "+getId(): unsigned",
   "+setId(unsigned _id): void",
   "+getType(): ROLE_TYPE",
   "+getBasingVar(): unsigned",
   "+getBasingFSA(): unsigned",
   "+getConcatenationPath(): PathExpression*",
   "+getSignOffExpression(): SignOffExpression*",
   "+isPosRole(): bool",
   "+isDosRole(): bool");

Class.RoleList("RoleList")
  ("-instance: static RoleList*",
   "-roles: vector<Role*>",
   "-penv: PathEnvironment*",
   "-max_role_id: unsigned",
   "-max_role_id_computed: bool")
  ("+initInstance(PathEnvironment* _penv): static void",
   "+getInstance(): static RoleList*",
   "+~RoleList()",
   "+getRoles(): vector<Role*>*",
   "+getFreshRole(unsigned basing_var, ROLE_TYPE type, PathExpression* from_var): Role*",
   "+getMaxRoleId(): unsigned",
   "+removeRole(Role* role): bool",
   "+sortRolesForSignOffs(): void",
   "-RoleList(PathEnvironment* _penv)",
   "-computeMaxRoleId(): void");

Class.StreamManager("StreamManager")
  ("-instance: static StreamManager*",
   "-query_istream: InputStream*",
   "-xml_istream: InputStream*",
   "-debug_ostream: OutputStream*",
   "-eval_ostream: OutputStream*")
  ("+initInstance(InputStream* _query_istream, InputStream* _xml_istream, OutputStream* _debug_ostream, OutputStream* _eval_ostream): static void",
   "+getInstance(): static StreamManager*",
   "+~StreamManager()",
   "+print(): void",
   "+getQueryInputStream(): InputStream*",
   "+getXMLInputStream(): InputStream*",
   "+getDebugOutputStream(): OutputStream*",
   "+getEvalOutputStream(): OutputStream*",
   "-StreamManager(InputStream* _query_istream, InputStream* _xml_istream, OutputStream* _debug_ostream, OutputStream* _eval_ostream)");

Class.StreamPreProcessor("StreamPreProcessor")
  ("-instance: static StreamPreProcessor*",
   "-tagmap: TagMap*",
   "-tag: TAG",
   "-st: StreamTracker")
  ("+initInstance(Buffer* _buffer, ProjectionDFA* _pdfa): static void",
   "+getInstance(): static StreamPreProcessor*",
   "+~StreamPreProcessor()",
   "+processOpeningTag(const char* tag): void",
   "+processBachelorTag(const char* tag): void",
   "+processClosingTag(const char* tag): void",
   "+processClosingTag(): void",
   "+processPCData(const char* data): void",
   "+readNext(): void",
   "+readAll(): void",
   "-StreamPreProcessor(Buffer* _buffer, ProjectionDFA* _pdfa)");

Class.StreamTracker("StreamTracker")
  ("-cur: ProjectionDFAState*",
   "-buffer: Buffer*",
   "-tagmap: TagMap*",
   "-skip_subtree_depth: unsigned",
   "-keep_subtree_depth: unsigned")
  ("+StreamTracker(ProjectionDFA* _pdfa, Buffer* _buffer)",
   "+~StreamTracker()",
   "+processOpeningTag(const char* tag): void",
   "+processBachelorTag(const char* tag): void",
   "+processClosingTag(TAG tag): void",
   "+processClosingTag(const char* _tag): void",
   "+processClosingTag(): void",
   "+processPCData(const char* data): void");

Class.StringHash("StringHash")
  ("-base: int",
   "-matches: vector<TagInfo*>*",
   "-free_index: unsigned")
  ("+StringHash()",
   "+~StringHash()",
   "+hash(const char* s): unsigned",
   "+hash(const char* s, unsigned start_offset, unsigned end_offset): unsigned",
   "-h(const char* s): unsigned",
   "-h(const char* s, unsigned start_offset, unsigned end_offset): unsigned");

Class.TagInfo("TagInfo")
  ("+tag: const char*",
   "+index: unsigned")
  ("+TagInfo (const char* _tag, unsigned _index)",
   "+~TagInfo()");

Class.TagMap("TagMap")
  ("-instance: static TagMap*",
   "-tags: vector<const char*>",
   "-sh: StringHash")
  ("+getInstance(): static TagMap*",
   "+~TagMap()",
   "+insertTag(const char* tag): unsigned",
   "+insertTagWithOffset(const char* tag, unsigned start_offset, unsigned end_offset): unsigned",
   "+print(OutputStream& dos): void",
   "+getTag(unsigned id): const char*",
   "-TagMap()");

Class.VarName("VarName")
  ("-instance: static VarName*",
   "-varnames: vector<const char*>",
   "-index: int")
  ("+getInstance(): static VarName*",
   "+~VarName()",
   "+insertVarname(const char* varname, bool force): unsigned",
   "+print(OutputStream& dos): void",
   "+getVarname(unsigned id): const char*",
   "+getFreshVarname(): const char*",
   "-VarName()");

leftToRight.bottom(40)(FSAMap, RoleList, TagMap);
topToBottom.right(20)(TagMap, StringHash);
topToBottom.midx(20)(StringHash, TagInfo);
topToBottom.midx(20)(RoleList, Role, StreamPreProcessor, Loader, StreamManager);
leftToRight.midy(40)(Loader, SAX);
leftToRight.bottom(40)(ParVarMap, StreamPreProcessor, StreamTracker, VarName);

drawObjects(Loader,
  SAX,
  FSAMap,
  ParVarMap,
  Role,
  RoleList,
  StreamManager,
  StreamPreProcessor,
  StreamTracker,
  StringHash,
  TagInfo,
  TagMap,
  VarName);

% link declarations
link(composition)(pathVertical(Role.n, RoleList.bottom));
link(dependency)(pathVertical(TagMap.s, StringHash.top));
item(iStereo)("<<use>>")(obj.w = .5[TagMap.s - (35, 0), StringHash.n + (0, 10)]);
link(dependency)(pathVertical(StringHash.s, TagInfo.top));
item(iStereo)("<<use>>")(obj.w = .5[StringHash.s, TagInfo.n + (0, 10)]);
link(dependency)(pathHorizontal(StreamPreProcessor.e, StreamTracker.left));
item(iStereo)("<<use>>")(obj.w = .5[StreamPreProcessor.e - (40, 0), StreamTracker.w + (0, 20)]);
link(dependency)(pathVertical(StreamPreProcessor.s, Loader.top));
item(iStereo)("<<use>>")(obj.w = .5[StreamPreProcessor.s, Loader.n + (0, 10)]);
clink(dashedLink)(SAX, Loader);

endfig;
end