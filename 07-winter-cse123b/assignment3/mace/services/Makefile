MACEPATH=../
include $(MACEPATH)Makefile.vars
include Makefile.services

DIRS = $(SERVICES)

.PHONY: all null clean .cvsignore

all: dirs

DIR_MAKEFILES = $(addsuffix /Makefile,$(DIRS))

include $(MACEPATH)commondirs.mk

#MAKEFILES=$(MACEPATH)Makefile.lib

SUFFIX_DIRS = $(foreach s,$(COMPILE_VERSIONS),$(addsuffix $(s),$(addsuffix .,$(DIRS))))

clean: cleandirs
	rm -f core core.*

cleaner: cleanerdirs
	rm -f core core.*

dirs : null 
$(DIRS) : null
$(DIRS) : % : %/Makefile

$(DIR_MAKEFILES) : %/Makefile : 
	ln -sv ../Makefile.template $@

.cvsignore:
	rm -f .cvsignore
	for i in .cvsignore; do echo $$i >> $@; done

null: interfaces/Makefile
	@$(MAKE) -C interfaces nullfiles

Makefile: ;
Makefile.services: ;
$(MACEPATH)Makefile.vars: ;
$(MACEPATH)commondirs.mk: ;
