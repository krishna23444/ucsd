APPS = TritonShare_gui
LIBNAME = TritonShare_gui
MACEPATH = ../../
PRE_SERVICE_LIBS = -lTritonShare `pkg-config gtk+-2.0 --libs`

include ../Makefile.common

INCLUDE_DIRS += `pkg-config gtk+-2.0 --cflags`
INCLUDE_DIRS += -I$(MACEPATH)services/Http
LIBS += $(LIBDIRS) -lHttp -lappcommon -lmace $(CLIBS)  
LIBDIRS += `pkg-config gtk+-2.0 --libs`
