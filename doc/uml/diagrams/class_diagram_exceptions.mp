input metauml;

beginfig(1);

% class declaration (in alphabetic order)
AbstractClass.Exception("Exception")
  ("#type: EXCEPTION_TYPE",
   "#error_msg: const char*",
   "#error_id: ERROR_ID",
   "#exception_level: EXCEPTION_LEVEL")
  ("+Exception(EXCEPTION_TYPE _type, const char* _error_msg, ERROR_ID _error_id, EXCEPTION_LEVEL _exception_level)",
   "+~Exception()",
   "+terminate(int exit_code): void",
   "+getDebugMsg(): const char*",
   "+getType(): const EXCEPTION_TYPE",
   "+getErrorMsg(): const char*",
   "+getErrorId(): ERROR_ID",
   "+getExceptionLevel(): EXCEPTION_LEVEL");

Class.InvalidArgumentException("InvalidArgumentException")
  ()
  ("+InvalidArgumentException(const char* _error_msg, ERROR_ID _error_id, EXCEPTION_LEVEL _exception_level)",
   "+~InvalidArgumentException()");

Class.InvalidCastException("InvalidCastException")
  ()
  ("+InvalidCastException(const char* _error_msg, ERROR_ID _error_id, EXCEPTION_LEVEL _exception_level)",
   "+~InvalidCastException()");

Class.IOStreamException("IOStreamException")
  ()
  ("+IOStreamException(const char* _error_msg, ERROR_ID _error_id, EXCEPTION_LEVEL _exception_level)",
   "+~IOStreamException()");

Class.NotImplementedException("NotImplementedException")
  ()
  ("+NotImplementedException(const char* _error_msg, ERROR_ID _error_id, EXCEPTION_LEVEL _exception_level)",
   "+~NotImplementedException()");

Class.ParseException("ParseException")
  ()
  ("+ParseException(const char* _error_msg, ERROR_ID _error_id, EXCEPTION_LEVEL _exception_level)",
   "+~ParseException()");

Class.RuntimeException("RuntimeException")
  ()
  ("+RuntimeException(const char* _error_msg, ERROR_ID _error_id, EXCEPTION_LEVEL _exception_level)",
   "+~RuntimeException()");

leftToRight.bottom(40)(InvalidArgumentException, Exception, RuntimeException);
topToBottom.right(40)(InvalidArgumentException, InvalidCastException, IOStreamException);
topToBottom.left(40)(RuntimeException, ParseException, NotImplementedException);

drawObjects(Exception,
  InvalidArgumentException,
  InvalidCastException,
  IOStreamException,
  NotImplementedException,
  ParseException,
  RuntimeException);

% link declarations  
link(inheritance)(pathManhattanY(InvalidArgumentException.n, Exception.w));
link(inheritance)(pathManhattanX(InvalidCastException.e, Exception.s));
link(inheritance)(pathManhattanX(IOStreamException.e, Exception.s));
link(inheritance)(pathManhattanX(NotImplementedException.w, Exception.s));
link(inheritance)(pathManhattanX(ParseException.w, Exception.s));
link(inheritance)(pathManhattanY(RuntimeException.n, Exception.e));

endfig;
end