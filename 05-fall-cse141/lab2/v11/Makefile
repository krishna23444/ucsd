MAKE       := make -j
SHELL      := bash
RM         := rm -f
TOUCH      := touch

CTAGS      := exuberant-ctags
TAGS       := tags

MAKEDEPEND := makedepend -Y
DEPEND     := .depend

CPP        := g++
CPPFLAGS   := -O2 -Wall -g
LD         := g++
LDFLAGS    := -Wall -g

SRC        := $(wildcard *.cc)
OBJ        := $(SRC:%.cc=%.o)
HEADERS    := $(wildcard *.h)
EXE        := iss


.PHONY: all clean new depend
all:
	$(MAKE) $(TAGS)
	$(MAKE) $(DEPEND)
	$(MAKE) $(EXE)

clean:
	$(RM) *.o .*.swp *~ core $(DEPEND) $(EXE)

new:
	$(MAKE) clean
	$(MAKE) all

depend: $(DEPEND)
$(DEPEND): $(SRC)
	-$(MAKEDEPEND) -- $(CPPFLAGS) -- $^ 2>/dev/null
	-@$(TOUCH) $@

$(TAGS): $(SRC) $(HEADERS)
	-$(CTAGS) $^


%.o: %.cc
	$(CPP) $(CPPFLAGS) -c $<


$(EXE): $(OBJ)
	$(CPP) $(LDFLAGS) -o $(EXE) $^



# DO NOT DELETE

main.o: proc.h
proc.o: proc.h
