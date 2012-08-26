#CC=g++
CC=clang++
WARNINGS=-Wall -Wextra -Woverloaded-virtual
#CFLAGS=-c -std=c++0x -g -ggdb -stdlib=libc++ -fPIC -D_DEBUG
CFLAGS=-c -std=c++0x -O2 -stdlib=libc++ -fPIC -g
LIBS=
LIBNAME=liboodb.so
MAJOR_VER=1
MINOR_VER=0
LIBFNAME=$(LIBNAME)
SOURCES=oodb.cc file_driver.cc
OBJECTS=$(SOURCES:.cc=.o)

all: static shared

shared: $(OBJECTS)
	$(CC) $(WARNINGS) $(LIBS) -shared -Wl,-soname,$(LIBNAME) -o $(LIBFNAME) *.o

static: $(OBJECTS)
	ar crf liboodb.a *.o

.cc.o:
	$(CC) $(WARNINGS) $(CFLAGS) $< -o $@

rebuild: clean all

clean:
	rm *.o liboodb*
