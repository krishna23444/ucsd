# Authors: Erik Buchanan, Nitay Joffe
# Class: CSE 123B - Networking
# Assignment: 1 - Web Server
# Date: 1/21/07
CC = gcc
CFLAGS = -Wall -g

LD = gcc
LDFLAGS = -Wall -pthread -g

RM = rm -f
DEPENDENCIES = .depend

HEADERS = $(wildcard *.h)

EXECUTABLE_SOURCES_WITHOUT_MAIN = debug.c htaccess.c http.c server.c \
                                  string_set.c
EXECUTABLE_SOURCES = $(EXECUTABLE_SOURCES_WITHOUT_MAIN) main.c
EXECUTABLE_OBJECTS = $(EXECUTABLE_SOURCES:%.c=%.o)
EXECUTABLE = server

TEST_SOURCES = $(EXECUTABLE_SOURCES_WITHOUT_MAIN) CuTest.c all_unit_tests.c \
               htaccess_test.c string_set_test.c
TEST_OBJECTS = $(TEST_SOURCES:%.c=%.o)
TEST = all_unit_tests

ALL_C_FILES = $(wildcard *.c)

CLEAN_FILES = *~ *.o
CLOBBER_FILES = $(TEST) $(EXECUTABLE) $(DEPENDENCIES)

TAR_FILES = $(ALL_C_FILES) $(HEADERS) Makefile cutest_license.txt README


.PHONY: all clean clobber new tar test
all:  $(EXECUTABLE)

clean:
	$(RM) $(CLEAN_FILES)

clobber: clean
	$(RM) $(CLOBBER_FILES)

new: clobber all

tar:
	tar -cjf code-`date +%Y%m%d%H%M%S`.tar.bz2 $(TAR_FILES)

test: $(TEST)
	./$<

$(EXECUTABLE): $(DEPENDENCIES) $(EXECUTABLE_OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(EXECUTABLE_OBJECTS)

$(TEST): $(DEPENDENCIES) $(TEST_OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(TEST_OBJECTS)

$(DEPENDENCIES): $(ALL_C_FILES)
	makedepend -Y -- $(CFLAGS) -- $^ &> /dev/null
	touch $@

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<


# DO NOT DELETE

CuTest.o: CuTest.h
all_unit_tests.o: CuTest.h all_unit_tests.h
debug.o: debug.h
htaccess.o: debug.h htaccess.h string_set.h
htaccess_test.o: CuTest.h htaccess.h string_set.h
http.o: debug.h http.h
main.o: debug.h htaccess.h string_set.h server.h
server.o: debug.h htaccess.h string_set.h http.h server.h
string_set.o: debug.h string_set.h
string_set_test.o: CuTest.h string_set.h
