#
# Makefile template for CSE 30 -- PA2
#

#
# You need to change these next lines for each assignment
#

HEADERS		= mycrypt.h

C_SRCS		= main.c getPassPhrase.c getKeys.c getRotateValue.c

ASM_SRCS	= strToULong.s rotate.s checkRange.s maskPassPhrase.s \
		  mycrypt.s

C_OBJS		= main.o getPassPhrase.o getKeys.o getRotateValue.o

ASM_OBJS	= strToULong.o rotate.o checkRange.o maskPassPhrase.o \
		  mycrypt.o

OBJS		= ${C_OBJS} ${ASM_OBJS}

EXE		= mycrypt

#
# You should not need to change anything below this line
#

#
# Relevant man pages:
#
# man -M /software/common/gnu/man gcc
# man -M /software/common/gnu/man as
# man -M /opt/SUNWspro/man lint
#

GCC		= /software/common/gnu/bin/gcc
ASM		= $(GCC)
LINT		= /opt/SUNWspro/bin/lint

GCC_FLAGS	= -c -g -ansi -W -Wall -D__EXTENSIONS__
LINT_FLAGS1	= -c -err=warn
LINT_FLAGS2	= -u -err=warn
ASM_FLAGS	= -c -g
LD_FLAGS	= -g -ansi -W -Wall

#
# Standard rules
#

.s.o:
	@echo "Assembling each assembly source file separately ..."
	$(ASM) $(ASM_FLAGS) $<
	@echo ""

.c.o:
	@echo "Linting each C source file separately ..."
	$(LINT) $(LINT_FLAGS1) $<
	@echo ""
	@echo "Compiling each C source file separately ..."
	$(GCC) $(GCC_FLAGS) $<
	@echo ""

#
# Simply have our project target be a single default $(EXE) executable.
#

$(EXE):	$(OBJS)
	@echo "2nd phase lint on all C source files ..."
	$(LINT) $(LINT_FLAGS2) *.ln
	@echo ""
	@echo "Linking all object modules ..."
	$(GCC) -o $(EXE) $(LD_FLAGS) $(OBJS)
	@echo ""
	@echo "Done."

${C_OBJS}:      ${HEADERS}


clean:
	@echo "Cleaning up project directory ..."
	rm -f *.o $(EXE) *.ln core a.out
	@echo ""
	@echo "Clean."

new:
	make clean
	make

public:
	strip $(EXE)
	chmod 755 $(EXE) 
	cp -p $(EXE) $(HOME)/../public/pa2test
	cat Makefile | head -89 > $(HOME)/../public/Makefile-PA2
	chmod 644 $(HOME)/../public/Makefile-PA2
