CC=g++
CXX=g++
RANLIB=ranlib

LIBSRC=container.cpp sockets.cpp
LIBOBJ=$(LIBSRC:.cpp=.o)

INCS=-I.
CFLAGS = -Wall -std=c++11 -g $(INCS)
CXXFLAGS = -Wall -std=c++11 -g $(INCS)

VIRLIB = container sockets
TARGETS = $(VIRLIB)

TAR=tar
TARFLAGS=-cvf
TARNAME=ex5.tar
TARSRCS=$(LIBSRC) Makefile README

all: both

both: container.o sockets.o

container.o: container.cpp
	g++ -std=c++11 -Wall container.cpp -o container

sockets.o: sockets.cpp
	g++ -std=c++11 -Wall sockets.cpp -o sockets

clean:
	$(RM) $(TARGETS) $(VIRLIB) $(OBJ) $(LIBOBJ) *~ *core

depend:
	makedepend -- $(CFLAGS) -- $(SRC) $(LIBSRC)

tar:
	$(TAR) $(TARFLAGS) $(TARNAME) $(TARSRCS)
