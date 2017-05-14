LIBNAME = appcommon
MACEPATH = ../../

.PHONY: library svnignore .cvsignore
include ../../Makefile.vars

all: library

CXXFLAGS = $(GLOBAL_FLAGS) -I../.. -I../../lib -I../../services -I../../services/interfaces $(PTHREAD_FLAG) -D_REENTRANT

SRCS = $(wildcard *.cc)
BASE_OBJS = $(SRCS:.cc=.o)

%.d:	%.cc 
	$(DEPENDCCRULE)
ifeq "$(MAKECMDGOALS)" "clean"
else 
ifeq "$(MAKECMDGOALS)" "cleaner"
else 
ifeq "$(SRCS)" ""
else 
-include $(SRCS:.cc=.d)
endif
endif
endif

cleandepends:
	rm *.d

#$(LIBNAME).so: $(OBJS)
#	ld -G $(OBJS) -o $(LIBNAME).so

clean:
	@echo "Removing dependency files"
	@rm -f *.d *.d.*
	@echo "Removing object and library files of compile type $(TYPES)"
	@rm -f $(ALL_OBJS) $(ALL_LIBS)
cleaner:
	@echo "Removing all dependency files, cvsignore files, and object and library files of compile type $(COMPILE_VERSIONS)"
	@rm -f *.d *.d.* .cvsignore $(CLEAN_OBJS) $(CLEAN_LIBS)


svnignore: .cvsignore
	if [ -f .cvsignore ]; then svn propset svn:ignore -F .cvsignore .; fi
.cvsignore : 
	rm -f .cvsignore
	for i in .cvsignore $(CLEAN_LIBS); do echo $$i >> $@; done

include ../../extradefaults.mk

library: $(ALL_LIBS)

