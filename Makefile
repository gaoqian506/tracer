

INCLUDE_DIR = -Iinclude

SRCS=$(wildcard  src/tracer/*.cpp)
OBJS=$(SRCS:%.cpp=%.o)



all : src/test

src/test : $(OBJS) src/test.cpp
	g++ -g $(OBJS) src/test.cpp $(INCLUDE_DIR) -o $@
	
%.o : %.cpp
	g++ -c -g $< $(INCLUDE_DIR) -o $@
	
clean:
	rm -f $(OBJS) src/test
	
	
#all : trace
#
#trace : trace.o
#	g++ -g $< -o $@
#trace.o : trace.cpp
#	g++ -c -g $< -o $@
#
#	
#clean:
#	rm -f trace.o trace