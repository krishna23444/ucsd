
include $(MACEPATH)/extradefaults-gen.mk

GEN_EXTRADEFAULTS=$(MACEPATH)/perl5/gen-extradefaults.pl

$(APPS) : % : %.$(DEFAULT_VERSION)
	ln -sfv $< $@

$(MACEPATH)/extradefaults-gen.mk : $(MACEPATH)/Makefile.vars $(GEN_EXTRADEFAULTS)
	@$(GEN_EXTRADEFAULTS) $(COMPILE_VERSIONS) > $@
