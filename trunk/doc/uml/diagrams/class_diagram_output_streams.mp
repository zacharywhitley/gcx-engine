input metauml;

beginfig(1);

% class declaration (in alphabetic order)
Class.FileOutputStream("FileOutputStream")
  ("-file: FILE*")
  ("+FileOutputStream(const char* _path)",
   "+~FileOutputStream()",
   "+open(): void",
   "+close(): void",
   "+write(const char* str): void",
   "+writeln(const char* str): void",
   "+isWriteable(): bool",
   "+flush(): void");

Class.NullOutputStream("NullOutputStream")
  ()
  ("+NullOutputStream()",
   "+~NullOutputStream()",
   "+open(): void",
   "+close(): void",
   "+write(const char* str): void",
   "+writeln(const char* str): void",
   "+isWriteable(): bool",
   "+flush(): void");

AbstractClass.OutputStream("OutputStream")
  ("-indents: int",
   "#type: OSTREAM_TYPE",
   "#arg: const char*")
  ("+OutputStream(OSTREAM_TYPE _type, const char* _arg)",
   "+~OutputStream()",
   "+open()=0: void",
   "+close()=0: void",
   "+decrementIndents(int n): void",
   "+incrementIndents(int n): void",
   "+resetIndents(): void",
   "+writeIndents(): void",
   "+write(const char* str)=0: void",
   "+writeln(const char* str)=0: void",
   "+isWriteable()=0: bool",
   "+flush()=0: void",
   "+getType(): const OSTREAM_TYPE",
   "+getArg(): const char*");

iGroup.left := 20;
iGroup.right := 20;
iGroup.top := 8;
iGroup.bottom := 8;
iGroup.boxed := 1;
iGroup.borderColor := (0, 0, 0);
iPict.ignoreNegativeBase := 1;

Picture.Text("OutputStreamManip");

Class.DecIndents("DecIndents")
  ("-n: int")
  ("+DecIndents(int _n)",
   "+operator( )(OutputStream& out) const: void");

Class.IncIndents("IncIndents")
  ("-n: int")
  ("+IncIndents(int _n)",
   "+operator( )(OutputStream& out) const: void");

Class.ResIndents("ResIndents")
  ()
  ("+ResIndents()",
   "+operator( )(OutputStream& out) const: void");

Class.WriIndents("WriIndents")
  ()
  ("+WriIndents()",
   "+operator( )(OutputStream& out) const: void");

topToBottom.midx(8)(Text, DecIndents, IncIndents, ResIndents, WriIndents);

Group.OutputStreamManip(Text, DecIndents, IncIndents, ResIndents, WriIndents);

Class.SocketOutputStream("SocketOutputStream")
  ()
  ("+SocketOutputStream()",
   "+~SocketOutputStream()",
   "+open(): void",
   "+close(): void",
   "+write(const char* str): void",
   "+writeln(const char* str): void",
   "+isWriteable(): bool",
   "+flush(): void");

Note.SocketOutputStreamNote("Remark:", "Currently Not Implemented");

Class.StdoutOutputStream("StdoutOutputStream")
  ()
  ("+StdoutOutputStream()",
   "+~StdoutOutputStream()",
   "+open(): void",
   "+close(): void",
   "+write(const char* str): void",
   "+writeln(const char* str): void",
   "+isWriteable(): bool",
   "+flush(): void");

leftToRight.bottom(40)(FileOutputStream, OutputStream, StdoutOutputStream);
topToBottom.right(40)(FileOutputStream, NullOutputStream);
topToBottom.left(40)(StdoutOutputStream, SocketOutputStream);
topToBottom.midx(40)(SocketOutputStream, SocketOutputStreamNote);
topToBottom.midx(25)(OutputStream, OutputStreamManip);

drawObjects(FileOutputStream,
  NullOutputStream,
  OutputStream,
  OutputStreamManip,
  SocketOutputStream,
  SocketOutputStreamNote,
  StdoutOutputStream);

% link declarations
link(inheritance)(pathHorizontal(FileOutputStream.e, OutputStream.left));
link(dependency)(pathVertical(OutputStream.s, OutputStreamManip.top));
item(iStereo)("<<use>>")(obj.w = .5[OutputStream.s + (0, 10), OutputStreamManip.n]);
link(inheritance)(pathStepX(NullOutputStream.e, OutputStream.w, 20));
link(inheritance)(rpathStepX(OutputStream.e, SocketOutputStream.w, 20));
link(inheritance)(pathHorizontal(StdoutOutputStream.w, OutputStream.right));
clink(dashedLink)(SocketOutputStreamNote, SocketOutputStream);

endfig;
end