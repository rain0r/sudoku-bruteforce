CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g 
LDFLAGS=-g -lboost_timer -lboost_system -lboost_thread 
LDLIBS=

SRCS=$(wildcard *.cpp)
OBJS=$(subst .cpp,.o,$(SRCS))

all: sudoku

sudoku: $(OBJS)
	g++ $(LDFLAGS) -o $@ $(OBJS) 

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ .dependtool

include .depend
