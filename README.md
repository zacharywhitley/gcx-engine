# THE GCX XQUERY ENGINE
Version 2.1 Readme

The G(arbage) C(ollected) X(Query) engine is the first streaming
XQuery engine that implements active garbage collection, a novel
buffer management strategy in which both static and dynamic analysis
are exploited. This technique actively purges main memory buffers at
runtime based on the current status of query evaluation.
This approach aims at both keeping main memory consumption low at
runtime and speeding up query evaluation. 
It has originally been developed as part of a research project of 
the Saarland University Database Group, which is continued at
the Freiburg University Database Group. For detailed information on
active garbage collection in XQuery engines please visit the GCX
project homepage at

  -> http://dbis.informatik.uni-freiburg.de/index.php?project=GCX

------------------------------------------------------------
* See file INSTALL.txt for installation instructions
* See file LICENSE.txt for license information
------------------------------------------------------------

For feedback, such as questions, comments, bug reports, or 
feature requests please use one of the following GCX mailing lists.

  -> http://lists.sourceforge.net/mailman/listinfo/gcx-engine-general
     Mailing list for general discussion about GCX (general questions,
     comments ...).
  -> http://lists.sourceforge.net/mailman/listinfo/gcx-engine-support
     Mailing list to ask questions about using and building GCX.
  -> http://lists.sourceforge.net/mailman/listinfo/gcx-engine-bugs
     Mailing list for bug reports and discussion about bugs in GCX.
  -> http://lists.sourceforge.net/mailman/listinfo/gcx-engine-requests
     Mailing list to request new or desired features for future
     releases.
