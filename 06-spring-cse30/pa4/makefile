#
# Makefile template for CSE 30 -- PA4
#

#
# You need to change these next three lines for each assignment
#

HEADERS		= mydir.h

C_SRCS		= mydir.c buildFileInfoTable.c displayFileInfo.c \
		  mtimeSortAscending.c nameSortDescending.c \
		  displayOwnerName.c displayPermissions.c

ASM_SRCS	= displayGroupName.s mtimeSortDescending.s \
		  nameSortAscending.s

C_OBJS		= mydir.o buildFileInfoTable.o displayFileInfo.o \
		  mtimeSortAscending.o displayOwnerName.o \
		  displayPermissions.o nameSortDescending.o

ASM_OBJS	= displayGroupName.o nameSortAscending.o \
		  mtimeSortDescending.o 

OBJS		= ${C_OBJS} ${ASM_OBJS}

EXE		= mydir

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
# Simply have our project target be a single default "a.out" executable.
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
	/usr/bin/rm -f *.o a.out *.ln core $(EXE)
	@echo ""
	@echo "Clean."

new:
	make clean
	make

public:
	strip $(EXE)
	chmod 755 $(EXE) 
	cp -p $(EXE) $(HOME)/../public/pa4test

