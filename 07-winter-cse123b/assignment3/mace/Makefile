#DIRS=lib compiler services application
DIRS=perl5 lib services application

# MACEPATH = .

include Makefile.vars
ifeq "$(MACE_EXTRAS_SHA1)" "1"
DIRS:=mace-extras/sha1 $(DIRS)
endif

.PHONY: all clean .cvsignore

include commondirs.mk

clean: cleandirs
cleaner: cleanerdirs
	rm extradefaults-gen.mk

.cvsignore: 
	rm -f .cvsignore
	for i in .cvsignore extradefaults-gen.mk; do echo $$i >> $@; done
