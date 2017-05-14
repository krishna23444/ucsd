LIBDIR   := lib
LIBS     := $(LIBDIR)/onyx_xml.jar

FLEXFILE := onyx_scanner.flex
SCANNER  := Scanner.java
DRIVER   := ScannerDriver.java
PROGRAM  := scan

.PHONY: all clean new flex
all: $(PROGRAM)

clean:
	rm -f *~ *.class Tests/*.xml $(SCANNER) $(PROGRAM)

new: clean all

$(SCANNER): $(FLEXFILE)
	jflex $<

$(PROGRAM): $(SCANNER) $(DRIVER)
	javac -cp $(CLASSPATH):$(LIBS) $(DRIVER)
	echo '#!/bin/sh' > $(PROGRAM)
	echo 'java ScannerDriver $$1' >> $(PROGRAM)
	chmod ugo+x $(PROGRAM)
