CXX=g++
BISON=bison
FLEX=flex
TARGET=irco
OBJS=Parser.o Scanner.o ParseDriver.o main.o ASTnodes.o


CFLAGS=-g -std=c++11

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(CFLAGS) -o $@

Parser.cpp: Parser.y
	$(BISON) -o $@ $<

Scanner.cpp: Scanner.l
	$(FLEX) -o $@ $<

.SUFFIXES:.cpp.o
.cpp.o:
	$(CXX) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) location.hh position.hh stack.hh Parser.cpp Parser.hpp Scanner.cpp




