CC = /grid/common/pkgs/gcc/v6.3.0p1/bin/g++
CFLAGS  = -g
TARGET = main
LD_PATH = /grid/common/pkgs/gcc/v6.3.0p1/lib:/grid/common/pkgs/gcc/v6.3.0p1/lib64


all: clean $(TARGET)

.PHONY: clean run

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

%.o : %.C
	$(CC) $(CFLAGS) $< -o $@

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

clean:
	/bin/sh -c 'rm -rf $(TARGET) *.o'

run:
	@export LD_LIBRARY_PATH=$(LD_PATH) && ./$(TARGET) | c++filt -t
