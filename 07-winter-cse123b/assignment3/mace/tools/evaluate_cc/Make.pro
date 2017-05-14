 

CC      = g++-2.95  -g 
CCLIB   = g++-2.95 -G
LD      = g++-2.95  -g

PIC     = -fPIC
#CFLAGS  = -O

LROOT = /usr/project/ari1/leda-4.3
 
so  = .so
a   = .a
o   = .o
e   = 
AR  = ar cr
s   = .s
ASM = $(CC) -c $(GFLAGS)

XLIB_PATH = -L/usr/X11R6/lib

XLIB    = 

VPATH = ".\"

include Make.lst

.cc$(o):  
	$(CC) $(CFLAGS) $(DFLAGS) $(IFLAGS) -I$(LROOT)/include -c $*.cc
.cpp$(o):  
	$(CC) $(CFLAGS) $(DFLAGS) $(IFLAGS) -I$(LROOT)/include -c $*.cpp
.C$(o):
	$(CC) $(CFLAGS) $(DFLAGS) $(IFLAGS) -I$(LROOT)/include -c $*.C

$(o)$(e): gml_scanner.o gml_parser.o gmlp.o
	$(LD) $(LFLAGS) -o $* gml_scanner.o gml_parser.o gmlp.o $*$(o) $(OBJECTS) -L$(LROOT) $(UNIXLIB)

.C$(e): gml_scanner.o gml_parser.o gmlp.o
	$(CC) $(CFLAGS) $(DFLAGS) $(IFLAGS) -I$(LROOT)/include -c $*.C
	$(LD) $(LFLAGS) -o $* gml_scanner.o gml_parser.o  gmlp.o $*$(o) $(OBJECTS)  -L$(LROOT) $(UNIXLIB)

.cc$(e): gml_scanner.o gml_parser.o gmlp.o  
	$(CC) $(CFLAGS) $(DFLAGS) $(IFLAGS) -I$(LROOT)/include -c $*.cc
	$(LD) $(LFLAGS) -o $* gml_scanner.o gml_parser.o gmlp.o $*$(o) $(OBJECTS)  -L$(LROOT) $(UNIXLIB)

all:	$(OBJECTS) $(PROGS)

clean:
	rm -f $(PROGS)
	rm -f *$(o) core

del:
	rm -f *$(o) $(PROGS) core


static_xlman:
	$(LD) -o xlman xlman.o ../../libW.a ../../libP.a ../../libG.a ../../libL.a $(XLIB_PATH) $(XLIB)
	strip xlman

