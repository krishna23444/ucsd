# Author: Kevin Clark <k1clark@ucsd.edu>
# Author: Nitay Joffe <njoffe@ucsd.edu>
# Date: 03/06/06
#
# $@ - target of rule
# $< - first dependency
# $^ - all dependencies
#

LIBDIR          := lib
LIBS            := $(subst jar ,jar:,$(wildcard $(LIBDIR)/*.jar))

CLASS_DIR       := bin
JAVA_FILES      := $(wildcard *.java)
CLASSES         := $(patsubst %,$(CLASS_DIR)/%,$(JAVA_FILES:%.java=%.class))

CLASSPATH       := .:$(CLASSPATH):$(LIBS):$(CLASS_DIR)

JFLEX_FILE      := scanner.jflex
SCANNER         := Scanner.java

SYMBOLS         := sym.java

CUP_FILE        := parser.cup
PARSER          := Parser.java

SCANNER_DRIVER  := ScannerDriver
PARSER_DRIVER   := ParserDriver
INTERPRETER     := Interpreter

SCAN            := scan
PARSE           := parse
ONYXI           := onyxi

CHMOD           := chmod ugo+x
DIFF            := xmldiff -n
ECHO            := echo -e
JAVAC           := javac -cp $(CLASSPATH) -d $(CLASS_DIR)
JAVA            := java -cp $(CLASSPATH)
JFLEX           := jflex
RM              := rm -f


.PHONY: all clean new dist vars
all: $(SCAN) $(PARSE) $(ONYXI)

clean:
	-$(RM) *~ .*.swp $(CLASS_DIR)/*.class sample_data/*/in/*.{err,xml}
	-$(RM) $(PARSER) $(SYMBOLS) $(SCANNER) $(SCAN) $(PARSE) $(ONYXI)
	@$(ECHO)

new: clean all

dist: clean
	tar --exclude=*.svn* -cjvf compilers_a3.tar.bz2 *

vars:
	@$(ECHO) "  [vars] ------------------------------------------------- [vars]"
	@$(ECHO)
	@$(ECHO) "LIBS = $(LIBS)"
	@$(ECHO)
	@$(ECHO) "JAVA FILES = $(JAVA_FILES)"
	@$(ECHO)
	@$(ECHO) "CLASSES = $(CLASSES)"
	@$(ECHO)
	@$(ECHO) "CLASSPATH = $(CLASSPATH)"
	@$(ECHO)
	@$(ECHO) "JAVAC = $(JAVAC)"
	@$(ECHO)
	@$(ECHO) "JAVA = $(JAVA)"
	@$(ECHO)


$(CLASSES): $(PARSER) $(SYMBOLS) $(SCANNER) $(JAVA_FILES)
	$(JAVAC) $^


$(SCANNER): $(JFLEX_FILE)
	@$(ECHO) "  [jflex]------------------------------------------------  [jflex]"
	$(JFLEX) $<
	@$(ECHO)

$(PARSER) $(SYMBOLS): $(CUP_FILE)
	@$(ECHO) "  [cup] -------------------------------------------------- [cup]"
	$(JAVA) java_cup.Main -parser $(PARSER:%.java=%) < $<
	@$(ECHO)


$(SCAN): $(CLASSES)
	@$(ECHO) "  [scan] ------------------------------------------------- [scan]"
	@$(ECHO) '#!/bin/sh' > $@
	@$(ECHO) '$(JAVA) $(SCANNER_DRIVER) $$*' >> $@
	@$(CHMOD) $@
	@$(ECHO)

$(PARSE): $(CLASSES)
	@$(ECHO) "  [parse] ------------------------------------------------ [parse]"
	@$(ECHO) '#!/bin/sh' > $@
	@$(ECHO) '$(JAVA) $(PARSER_DRIVER) $$*' >> $@
	@$(CHMOD) $@
	@$(ECHO)

$(ONYXI): $(CLASSES)
	@$(ECHO) "  [onyxi] ------------------------------------------------ [onyxi]"
	@$(ECHO) '#!/bin/sh' > $@
	@$(ECHO) '$(JAVA) $(INTERPRETER) $$*' >> $@
	@$(CHMOD) $@
	@$(ECHO)
