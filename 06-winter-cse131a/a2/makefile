JAVAFILES     := $(wildcard *.java)
CLASSDIR      := class
CLASSFILES    := $(patsubst %,$(CLASSDIR)/%,$(JAVAFILES:%.java=%.class))

LIBDIR        := lib
LIBFILES      := $(wildcard $(LIBDIR)/*.jar)
LIBS          := $(subst jar ,jar:,$(LIBFILES))

TESTDIR       := test
TESTDATADIR   := $(TESTDIR)/data
TESTSUFFIX    := onyx

TFILES        := stats98lefty strvar
TESTFILES     := $(patsubst %,$(TESTDIR)/%.$(TESTSUFFIX),$(TFILES))
#TESTFILES     := $(wildcard $(TESTDIR)/*.$(TESTSUFFIX))

OUTPUTSUFFIX  := xml

CHMOD         := chmod ugo+x
DIFF          := xmldiff -n
JAVAC         := javac -cp $(CLASSPATH):$(LIBS) -d $(CLASSDIR)
JAVA          := java -cp $(CLASSPATH):./$(CLASSDIR)/:$(LIBS)
RM            := rm -f

CUPFILE       := parser.cup
PARSER        := Parser.java

SYMFILE       := sym.java
JFLEXFILE     := scanner.jflex
SCANNER       := Scanner.java

SCANNERDRIVER := ScannerDriver.java
PARSERDRIVER  := ParserDriver.java

SCAN          := scan
PARSE         := parse


.PHONY: all clean new debug test
all: $(SCAN) test

clean:
	-$(RM) *~ .*.swp *.class $(CLASSDIR)/*.class $(TESTDIR)/*.xml
	-$(RM) $(SCANNER) $(SCAN) $(SYMFILE) $(PARSER) $(PARSE)

new: clean all

debug: $(PARSE)
	@echo "  [debug]"
	@for i in $(TESTFILES); do                                  \
	  echo "--- $$i ---";                                       \
	  ./$(PARSE) $$i debug;                                     \
	done

test: $(PARSE)
	@echo "  [test]"
	@for i in $(TESTFILES); do                                  \
	  echo "--- $$i ---";                                       \
	  ./$(PARSE) $$i;                                           \
	  $(DIFF) $$i.xml $${i/$(TESTDIR)/$(TESTDATADIR)}.xml;      \
	done


$(SCANNER): $(JFLEXFILE)
	@echo "  [jflex]"
	jflex $< &> /dev/null

$(PARSER) $(SYMFILE): $(CUPFILE)
	@echo "  [cup]"
	$(JAVA) java_cup.Main -parser $(PARSER:%.java=%) < $< &> /dev/null


$(CLASSFILES): $(SCANNER) $(PARSER) $(JAVAFILES)
	@echo "  [javac]"
	$(JAVAC) $^


$(SCAN): $(CLASSFILES)
	@echo "  [scan]"
	@echo '#!/bin/sh' > $(SCAN)
	@echo '$(JAVA) $(SCANNERDRIVER:%.java=%) $$*' >> $(SCAN)
	@$(CHMOD) $(SCAN)

$(PARSE): $(CLASSFILES)
	@echo "  [parse]"
	@echo '#!/bin/sh' > $(PARSE)
	@echo '$(JAVA) $(PARSERDRIVER:%.java=%) $$*' >> $(PARSE)
	@$(CHMOD) $(PARSE)
