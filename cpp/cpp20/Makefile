CXX = /grid/common/pkgs/gcc/v12.3.0p1/bin/g++
LD_PATH = /grid/common/pkgs/gcc/v12.3.0p1/lib:/grid/common/pkgs/gcc/v12.3.0p1/lib64

C20FLAGS=-std=c++23 -fmodules-ts
CFLAGS=-g3 -O0 -fno-inline
LDFLAGS=

SRCS=main.cpp
OBJS=$(SRCS:.cpp=.o)

.PHONY: clean main

all: clean main

%.o:%.cpp
	$(CXX) $(C20FLAGS) $(CFLAGS) -c $<

module:
	$(CXX) $(C20FLAGS) -xc++-system-header iostream

main: $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@

clean:
	@rm -rf main *.o