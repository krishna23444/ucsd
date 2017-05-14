.PHONY: library .cvsignore

MACEPATH=../
include ../Makefile.vars

LIBNAME=mace

all: library

CXXFLAGS= $(GLOBAL_FLAGS) -I../ -D_REENTRANT -D_THREAD_SAFE -D_PTHREAD -D__USE_UNIX98 -D_GNU_SOURCE $(PTHREAD_FLAG)

SRCS = $(wildcard *.cc)
CSRCS = $(wildcard *.c)
BASE_OBJS = $(SRCS:.cc=.o) $(CSRCS:.c=.o)

%.d:	%.cc 
	$(DEPENDCCRULE)
%.d:	%.c 
	$(DEPENDCRULE)
ifeq "$(MAKECMDGOALS)" "clean"
else 
ifeq "$(MAKECMDGOALS)" "cleaner"
else
ifeq "$(SRCS)" ""
else 
-include $(SRCS:.cc=.d) $(CSRCS:.c=.d)
endif
endif
endif

include $(MACEPATH)/extradefaults.mk

#libmace.so: $(OBJS)
#	    ld -G $(OBJS) -o libmace.so

clean:
	@echo "Removing dependency files"
	@rm -f *.d *.d.*
	@echo "Removing object and library files of compile type $(TYPES)"
	@rm -f $(ALL_OBJS) $(ALL_LIBS)
cleaner:
	@echo "Removing all dependency files, and object and library files of compile type $(COMPILE_VERSIONS)"
	@rm -f *.d *.d.* $(CLEAN_OBJS) $(CLEAN_LIBS)

svnignore: .cvsignore
	if [ -f .cvsignore ]; then svn propset svn:ignore -F .cvsignore .; fi
.cvsignore:
	rm -f .cvsignore
	for i in .cvsignore $(CLEAN_LIBS); do echo $$i >> $@; done

library: $(ALL_LIBS)
