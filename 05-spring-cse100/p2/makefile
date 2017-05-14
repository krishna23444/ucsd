## Filename: Makefile
## Author: Nitay Joffe
## Date: 04/21/05
##
## Description: Makefile for java projects that works well with eclipse

# $@ = full name of the target of the rule
# $< = name of the first prerequisite
# $? = all the prerequisites that have changed, with spaces between them
# $^ = all the prerequisites, with spaces between them
# $* = stem of an implicit rule (maching %), or target name minux suffix
# @ -- turns off echoing of command being performed


# Edit line below, put in pure filenames of all source files in folder src/
FILES = TreapSet TreapSetNode SkipListSet SkipListSetNode \
		CountingComparator TestSets \
		SomeP2Checks Test 

JAVAC = javac
JFLAGS = -g -Xlint:all

DOCDIR = doc
SRCS = $(FILES:%=%.java)
CLASSES = $(SRCS:%.java=%.class)

.PHONY: all debug doc clean new run
#all: debug
all: run

doc: $(SRCS)
	javadoc -d $(DOCDIR) *.java

clean:
	rm -f core *.class $(DOCDIR)/*.html

debug: $(CLASSES)
	java Test

new:
	@make clean
	@make

run: $(CLASSES)
	java TestSets

%.class: %.java
	$(JAVAC) $(JFLAGS) $<
