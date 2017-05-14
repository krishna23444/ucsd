
MACEPATH=../
include $(MACEPATH)Makefile.vars

DIRS = Mace/Compiler

.PHONY: $(PERLMAKE) .cvsignore perlmake

all : $(PERLMAKE) dirs

$(DIRS) : $(PERLMAKE)

include $(MACEPATH)commondirs.mk

$(PERLMAKE) perlvars.mk :
	./makedep.pl > $@

clean: cleandirs
	rm -f $(PERLMAKE)
cleaner: cleanerdirs
	rm -f $(PERLMAKE)

testcompile: all
	$(MAKE) -f testcompile.mk testcompile

.cvsignore : 
	rm -f .cvsignore
	for i in .cvsignore $(notdir $(PERLMAKE)); do echo $$i >> $@; done

perlmake : $(PERLMAKE)

Mace/Compiler/% :
	$(MAKE) DIR=Mace/Compiler -C Mace/Compiler $*
