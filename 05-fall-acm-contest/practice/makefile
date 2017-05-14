MAKE        := make -j
SHELL       := bash
RM          := rm -f
MV          := mv -f
CP          := cp -f
ECHO        := echo
TOUCH       := touch
CTAGS       := ctags
TAGS        := tags

TAR         := tar -j
TARNAME     := acm
TARFILE     := $(TARNAME)-src.tar.bz2
TAREXCLUDES := .svn $(TARFILE)
TAREXCL     := $(patsubst %,--exclude %, $(TAREXCLUDES))


DIRS        := carpet interval jumble sync tax worm


.PHONY: all clean new srm tco tccc tar 
all clean:
	@for i in $(DIRS); do $(MAKE) -C $$i $@; done

new: clean all

tar: clean
	-$(TAR) $(TAREXCL) -cf $(TARFILE) .
