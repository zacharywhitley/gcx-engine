############################################
### Makefile for the GCX project          
###                                        
### Author: Michael Schmidt;
###         Gunnar Jehl (multi-step paths/aggregate functions/optimizations)            
### - Saarland University Database Group - 
############################################
#                                          
# Hint: Change the CPP flags if you want disable all optimizations
#
# NOTE: DISABLING OPTIMIZATIONS may SIGNIFICANTLY
#       slow down query evaluation!          
############################################

# possible configuration flags are
# -DROLE_REFCOUNT: Use reference counting instead of int multisets;
#                  this implementation is faster, but not suited for debugging
#                  purposes, since role ids are 'invisible'. It is strongly recommended
#                  to turn this compile option on.
# -DNO_OPTIMIZATIONS: Turn off (most of the) optimizations;
#                     should be used only for debugging purposes and to get
#                     better insights into the engine's internal processing strategy.
# -DREWRITE_VARSTEPS: Rewrite varstep expressions into for-loops. On the hand hand,
#                     this option causes earlier updates; on the other hand, it might
#                     interfere with other optimizations and, as a consequence, can
#                     slow down query evaluation.
# -DVALIDATION: Turn on XML document validation; please note that only those parts
#               of the document are validated that are kept according to the projection
#               strategy. For the remaining part, only depth is kept track of (but
#               closing tags are not matched against opening tags). You should ignore
#               this option if you are sure that your XML documents are well-formed.

############################# The suggested configuration (performance optimized) is:
#FLAGS = -DROLE_REFCOUNT 

############################# Alternative configurations are:
#### Rewriting of var step expression. Alternative optimized configuration.
FLAGS = -DROLE_REFCOUNT -DREWRITE_VARSTEPS

#### No optimizations, gives insight into the internals of the engine.
#FLAGS = -DROLE_REFCOUNT -DNO_OPTIMIZATIONS

#### No optimizations, rewriting of var step expression. Gives insight into the internals. 
#FLAGS = -DROLE_REFCOUNT -DNO_OPTIMIZATIONS -DREWRITE_VARSTEPS

CPP =  g++ -Wall -O3 $(FLAGS)

############################### list of source files
SOURCES=miscfunctions.cpp main.cpp expression.cpp emptyexpression.cpp constexpression.cpp sequenceexpression.cpp forexpression.cpp varstepexpression.cpp varexpression.cpp nodeconstructexpression.cpp ifexpression.cpp signoffexpression.cpp operandexpression.cpp condexpression.cpp andcondexpression.cpp orcondexpression.cpp notcondexpression.cpp existscondexpression.cpp condoperandexpression.cpp truecondexpression.cpp falsecondexpression.cpp pathstepexpression.cpp aggregatefunctexpression.cpp aggregatefunctsumexpression.cpp aggregatefunctavgexpression.cpp aggregatefunctminexpression.cpp aggregatefunctmaxexpression.cpp aggregatefunctcountexpression.cpp aggregatefunctstddevsampexpression.cpp aggregatefunctstddevpopexpression.cpp aggregatefunctvarsampexpression.cpp aggregatefunctvarpopexpression.cpp aggregatefunctlistexpression.cpp typeenums.cpp prettyprint.cpp environment.cpp buffer.cpp buffernode.cpp tagnode.cpp charnode.cpp bufferiterator.cpp loader.cpp varname.cpp pathenvironment.cpp pathexpression.cpp pathexpressionadornment.cpp streampreprocessor.cpp streamtracker.cpp fsamap.cpp parvarmap.cpp executor.cpp variabletree.cpp variabletreenode.cpp dependencyset.cpp dependencytuple.cpp passiveprojectiontree.cpp passiveprojectiontreenode.cpp passiveprojectiontreeconfiguration.cpp projectiontree.cpp projectiontreelabel.cpp projectiontreelabels.cpp projectiontreenode.cpp projectiondfa.cpp projectiondfastate.cpp projectiondfatransitions.cpp cmdline.cpp tagmap.cpp intmultiset.cpp buffernodelist.cpp stringhash.cpp role.cpp rolelist.cpp pathsteptagexpression.cpp pathstepstarexpression.cpp pathstepnodeexpression.cpp pathsteptextexpression.cpp error.cpp tokenconfiguration.cpp pathstepattribute.cpp pathstepattributeposition.cpp

############################### list of generated object files
OBJECTS=$(SOURCES:.cpp=.o)

############################### all 
all: gcx

############################### binary
gcx:	$(OBJECTS) query_parser.o query_lexer.o sax.o 
		$(CPP) $(CFLAGS) $(OBJECTS) query_parser.o query_lexer.o sax.o -o gcx

############################### objects
%.o: %.cpp
	$(CPP) $(CFLAGS) -c $<

query_parser.h: query_parser.C

query_parser.C: query_parser.y
	bison -v -b query_parser -d query_parser.y
	mv query_parser.tab.c query_parser.C
	mv query_parser.tab.h query_parser.h

query_parser.o: query_parser.C
	$(CPP) -c query_parser.C

query_lexer.C: query_lexer.l
#	flex -d -T -oquery_lexer.C query_lexer.l # Flex debugging mode
	flex -oquery_lexer.C query_lexer.l

query_lexer.o: query_lexer.C query_parser.h
	$(CPP) -c query_lexer.C

sax.C: sax.l
	flex sax.l
	sed "s/yy/yyxml/g" < lex.yy.c > sax.C
	rm -f lex.yy.c

sax.o: sax.C loader.h
	$(CPP) -c sax.C

############################### clean 
clean:
	rm -f *.o *~ gcx
	rm -f query_parser.h query_parser.output query_parser.C query_parser.tab.h query_parser.tab.c
	rm -f query_lexer.C
	rm -f sax.C
