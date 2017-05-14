#
# Makefile template for CSE 30 -- PA1
#

#
# You need to change these next lines for each assignment
#

HEADERS		= pa1.h

C_SRCS		= main.c strToLong.c

ASM_SRCS	= isEven.s printChar.s checkRange.s numOfDigits.s displayX.s

C_OBJS		= main.o strToLong.o

ASM_OBJS	= isEven.o printChar.o checkRange.o numOfDigits.o displayX.o

OBJS		= ${C_OBJS} ${ASM_OBJS}

EXE		= pa1

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

GCC_FLAGS	= -c -g -ansi -Wall -D__EXTENSIONS__
LINT_FLAGS1	= -c -err=warn
LINT_FLAGS2	= -u -err=warn
ASM_FLAGS	= -c -g
LD_FLAGS	= -g -ansi -Wall

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
	/usr/bin/rm -f *.o $(EXE) *.ln core a.out
	@echo ""
	@echo "Clean."

new:
	make clean
	make

public:
	strip $(EXE)
	chmod 755 $(EXE) 
	cp -p $(EXE) $(HOME)/../public/pa1test
	cat Makefile | head -87 > $(HOME)/../public/Makefile-PA1
	chmod 644 $(HOME)/../public/Makefile-PA1

