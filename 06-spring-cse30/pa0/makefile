#
# Makefile template for CSE 30 -- PA0
#

#
# You need to change these next few lines for each assignment
#

HEADERS		= pa0.h

C_SRCS		= main.c printBirthDateInfo.c

ASM_SRCS	= printHello.s sum3.s

C_OBJS		= main.o printBirthDateInfo.o

ASM_OBJS	= printHello.o sum3.o

OBJS		= ${C_OBJS} ${ASM_OBJS}

EXE		= a.out

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
ASM		= ${GCC}
LINT		= /opt/SUNWspro/bin/lint

GCC_FLAGS	= -c -g -ansi -Wall
LINT_FLAGS1	= -c -err=warn
LINT_FLAGS2	= -u -err=warn
ASM_FLAGS	= -c -g
LD_FLAGS	= -g -ansi -Wall

#
# Standard rules
#

.s.o:
	@echo "Assembling each assembly source file separately ..."
	${ASM} ${ASM_FLAGS} $<
	@echo ""

.c.o:
	@echo "Linting each C source file separately ..."
	${LINT} ${LINT_FLAGS1} $<
	@echo ""
	@echo "Compiling each C source file separately ..."
	${GCC} ${GCC_FLAGS} $<
	@echo ""

#
# Have our project target be named whatever we specify about for EXE.
#

${EXE}:	${OBJS}
	@echo "2nd phase lint on all C source files ..."
	${LINT} ${LINT_FLAGS2} *.ln
	@echo ""
	@echo "Linking all object modules ..."
	${GCC} ${LD_FLAGS} -o ${EXE} ${OBJS}
	@echo ""
	@echo "Done."

${C_OBJS}:	${HEADERS}

clean:
	@echo "Cleaning up project directory ..."
	/usr/bin/rm -f *.o ${EXE} *.ln core
	@echo ""
	@echo "Clean."

