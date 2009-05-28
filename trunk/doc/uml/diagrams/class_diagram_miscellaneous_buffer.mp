input metauml;

beginfig(1);

% class declaration (in alphabetic order)
Class.Buffer("Buffer")
  ("-root: BufferNode*",
   "-cur: BufferNode*")
  ("+Buffer(ProjectionDFA* _pdfa)",
   "+~Buffer()",
   "+appendTag(TAG tag): void",
   "+appendTag(TAG tag, vector<unsigned>* _cumulative_roles, vector<unsigned>* _non_cumulative_roles): void",
   "+appendPCData(const char* data): void",
   "+appendPCData(const char* data, vector<unsigned>* _cumulative_roles, vector<unsigned>* _non_cumulative_roles): void",
   "+appendRoles(vector<unsigned>* _cumulative_roles, vector<unsigned>* _non_cumulative_roles): void",
   "+print(OutputStream& dos): void",
   "+printCurrent(OutputStream& dos): void",
   "+debugPrint(OutputStream& dos): void",
   "+getRoot(): BufferNode*",
   "+getCurrent(): BufferNode*",
   "+closeTag(): void");

Class.BufferIterator("BufferIterator")
  ("-base: BufferNode*",
   "-cur: BufferNode*",
   "-cur_old: BufferNode*",
   "-cur_locked: bool",
   "-path: PathExpression*",
   "-spp: StreamPreProcessor*",
   "-ps_context: vector<BufferNode*>",
   "-ps_context_position: vector<unsigned>",
   "-ps_skip_subtree: vector<bool>",
   "-ps_attribute: vector<bool>",
   "-ps_attribute_position: vector<unsigned>",
   "-ps_back_track: vector<bool>",
   "-ps_index: unsigned")
  ("+BufferIterator(BufferNode* _base, PathExpression* _path)",
   "+~BufferIterator()",
   "+init(BufferNode* _base): void",
   "+debugPrint(OutputStream& dos, unsigned debug_mode): void",
   "+getNext(unsigned read_up_to_closing, unsigned lock_node): BufferNode*",
   "+reset(): void",
   "+clear(): void",
   "-isSatisfyingPath(BufferNode* nnode): BufferNode*",
   "-isMatchingNodeTestAndPredicates(BufferNode* nnode, unsigned ps_idx): bool",
   "-getPrevPSNode(): BufferNode*",
   "-unsignPSNode(BufferNode* nnode): void");

Class.BufferNode("BufferNode")
  ("+type: bool",
   "+parent: BufferNode*",
   "+r_sibling: BufferNode*",
   "+cumulative_roles: unsigned",
   "+cumulative_roles: IntMultiSet*",
   "+non_cumulative_roles: unsigned",
   "+non_cumulative_roles: IntMultiSet*",
   "+node: void*",
   "+is_locked: bool")
  ("+BufferNode(const char* _data, BufferNode* _parent)",
   "+BufferNode(TAG _tag, BufferNode* _parent)",
   "+BufferNode(const char* _data, BufferNode* _parent, vector<unsigned>* _cumulative_roles, vector<unsigned>* _non_cumulative_roles)",
   "+BufferNode(TAG _tag, BufferNode* _parent, vector<unsigned>* _cumulative_roles, vector<unsigned>* _non_cumulative_roles)",
   "+~BufferNode()",
   "+addChild(BufferNode* child): void",
   "+appendRoles(vector<unsigned>* _cumulative_roles, vector<unsigned>* _non_cumulative_roles): void",
   "+print(OutputStream& eos): void",
   "+printNoSubnodes(OutputStream& dos): void",
   "+debugPrint(OutputStream& dos): void",
   "+debugPrintCompleteBuffer(OutputStream& dos): void",
   "+getPCDataRepresentation(): const char*",
   "+getPCDataRepresentation(std::ostringstream& s): void",
   "+close(): void",
   "+isClosed(): bool",
   "+lock(): void",
   "+unlock(): void",
   "+isLocked(): bool",
   "+isRoot(): bool",
   "+isMarked(): bool",
   "+isCumulativeMarked(): bool",
   "+hasCumulativeMarkedAncestor(): bool",
   "+hasNoMarkedAndNoLockedDos(): bool",
   "+removeRoleFromNode(Role* role): void",
   "+clear(): void",
   "+clearSubtreeIfPossible(): void");

Class.BufferNodeList("BufferNodeList")
  ("-front: BufferNode*",
   "-back: BufferNode*")
  ("+BufferNodeList()",
   "+~BufferNodeList()",
   "+push_back(BufferNode* node): void",
   "+operator[](unsigned index): BufferNode*",
   "+print(OutputStream& dos): void",
   "+getFront(): BufferNode*",
   "+isEmpty(): bool",
   "+removeElem(BufferNode* to_remove, BufferNode* prev): void");

Class.CharNode("CharNode")
  ("+data: const char*")
  ("+CharNode(const char* _data)",
   "+~CharNode()");

Class.Environment("Environment")
  ("-env: Env")
  ("+Environment()",
   "+Environment(BufferNode* _root)",
   "+~Environment()",
   "+insertNodeBinding(unsigned var, BufferNode* node): void",
   "+print(OutputStream& dos): void",
   "+getNodeBinding(unsigned var): BufferNode*",
   "clone(): Environment*");

Class.IntMultiSet("IntMultiSet")
  ("-max: int",
   "-elems: unsigned*")
  ("+IntMultiSet()",
   "IntMultiSet(vector<unsigned>* _elems)",
   "+~IntMultiSet()",
   "+addElems(IntMultiSet& set): void",
   "+addElems(vector<unsigned>* _elems): void",
   "+addElem(unsigned i): void",
   "+print(OutputStream& dos): void",
   "+getElemVec(): vector<unsigned>",
   "+isEmpty(): bool",
   "+containsElem(unsigned i): bool",
   "+removeElem(unsigned i): bool");

Class.TagNode("TagNode")
  ("+tag: TAG",
   "+is_closed: bool",
   "+children: BufferNodeList")
  ("+TagNode(TAG _tag)",
   "+~TagNode()");

leftToRight.top(40)(Buffer, BufferNode);
topToBottom.midx(40)(Buffer, BufferIterator, IntMultiSet, Environment);
topToBottom.midx(40)(BufferNode, BufferNodeList);
leftToRight.bottom(40)(CharNode, BufferNodeList, TagNode);

drawObjects(Buffer,
  BufferIterator,
  BufferNode,
  BufferNodeList,
  CharNode,
  Environment,
  IntMultiSet,
  TagNode);

% link declarations
link(dependency)(pathHorizontal(Buffer.e, BufferNode.left));
item(iStereo)("<<use>>")(obj.w = .5[Buffer.e - (40, 0), BufferNode.w + (0, 115)]);
link(dependency)(rpathStepX(IntMultiSet.e, BufferNode.sw, 90));
item(iStereo)("<<use>>")(obj.w = .5[IntMultiSet.e + (70, 0), BufferNode.sw + (0, 232)]);
link(composition)(pathVertical(BufferNode.s, BufferNodeList.top));
link(composition)(pathVertical(CharNode.n, BufferNode.bottom));
link(composition)(pathVertical(TagNode.n, BufferNode.bottom));
link(dependency)(pathVertical(BufferIterator.n, Buffer.bottom));
item(iStereo)("<<use>>")(obj.w = .5[BufferIterator.n, Buffer.s]);
link(dependency)(pathStepX(Environment.e, BufferNode.w, 82));
item(iStereo)("<<bind>>")(obj.w = .5[Environment.e, BufferNode.w]);

endfig;
end