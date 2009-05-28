input metauml;

beginfig(1);

% class declaration (in alphabetic order)
Class.FileInputStream("FileInputStream")
  ("-file: FILE*")
  ("+FileInputStream(const char* _path)",
   "+~FileInputStream()",
   "+open(): void",
   "+close(): void",
   "+getParserStream(): FILE*",
   "+isReadable(): bool");

AbstractClass.InputStream("InputStream")
  ("#type: ISTREAM_TYPE",
   "#arg: const char*")
  ("+InputStream(ISTREAM_TYPE _type, const char* _arg)",
   "+~InputStream()",
   "+open()=0: void",
   "+close()=0: void",
   "+getParserStream()=0: FILE*",
   "+isReadable()=0: bool",
   "+getType(): const ISTREAM_TYPE",
   "+getArg(): const char*",
   "+setArg(const char* _arg): void");

Class.NullInputStream("NullInputStream")
  ()
  ("+NullInputStream()",
   "+~NullInputStream()",
   "+open(): void",
   "+close(): void",
   "+getParserStream(): FILE*",
   "+isReadable(): bool");

Class.SocketInputStream("SocketInputStream")
  ()
  ("+SocketInputStream()",
   "+~SocketInputStream()",
   "+open(): void",
   "+close(): void",
   "+getParserStream(): FILE*",
   "+isReadable(): bool");

Note.SocketInputStreamNote("Remark:", "Currently Not Implemented");

Class.StdinInputStream("StdinInputStream")
  ()
  ("+StdinInputStream()",
   "+~StdinInputStream()",
   "+open(): void",
   "+close(): void",
   "+getParserStream(): FILE*",
   "+isReadable(): bool");

leftToRight.bottom(40)(FileInputStream, InputStream, StdinInputStream);
topToBottom.right(40)(FileInputStream, NullInputStream);
topToBottom.left(40)(StdinInputStream, SocketInputStream);
topToBottom.midx(40)(SocketInputStream, SocketInputStreamNote);

drawObjects(FileInputStream,
  InputStream,
  NullInputStream,
  SocketInputStream,
  SocketInputStreamNote,
  StdinInputStream);

% link declarations
link(inheritance)(pathHorizontal(FileInputStream.e, InputStream.left));
link(inheritance)(pathManhattanX(NullInputStream.e, InputStream.s));
link(inheritance)(pathManhattanX(SocketInputStream.w, InputStream.s));
link(inheritance)(pathHorizontal(StdinInputStream.w, InputStream.right));
clink(dashedLink)(SocketInputStreamNote, SocketInputStream);

endfig;
end