MACEPATH=../../..

PARSERS=MaceHeaderRecDescent.pm MaceRecDescent.pm ClassParserRecDescent.pm

.PHONY: .cvsignore

all: $(PARSERS)

include $(MACEPATH)/Makefile.vars
ifneq (clean,$(MAKECMDGOALS))
ifneq (cleaner,$(MAKECMDGOALS))
include $(PERLMAKE)
endif
endif

MaceHeaderRecDescent.pm: $(Mace_Compiler_MHGrammar_dep)
MaceRecDescent.pm: $(Mace_Compiler_MaceGrammar_dep)
ClassParserRecDescent.pm: $(Mace_Compiler_ClassGrammar_dep)

$(PARSERS): $(compilergen_dep)
	$(compilergen) $@

cleaner: clean
clean:
	rm -f $(PARSERS)

svnignore: .cvsignore
	if [ -f .cvsignore ]; then svn propset svn:ignore -F .cvsignore .; fi
.cvsignore: 
	rm -f .cvsignore
	for i in .cvsignore $(PARSERS); do echo $$i >> $@; done

