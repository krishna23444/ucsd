MAKE     := make
SHELL    := bash
RM       := rm -f
ECHO     := echo

CPP      := g++
CPPFLAGS := -Wall

SRC      := $(shell ls *.cc 2>/dev/null)
EXE      := $(SRC:%.cc=%)~


.PHONY: all clean new run
all: run

clean:
	-$(RM) *.o *~ .*.swp core

new: clean all

run: $(EXE)
	-@$(ECHO)
	-@$(ECHO) "-------------------------------"
	-@./$(EXE)
	-@$(ECHO) "-------------------------------"
	-@$(ECHO)


%~: %.cc
	$(CPP) $(CPPFLAGS) -o $@ $<
