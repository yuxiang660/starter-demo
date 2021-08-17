CC = g++
CFLAGS  = -g
TARGET = main
LD_PATH = /grid/common/pkgs/gcc/v6.3.0p1/lib:/grid/common/pkgs/gcc/v6.3.0p1/lib64


all: $(TARGET)

.PHONY: clean run

$(TARGET): bar.cpp foo.cpp main.cpp
	$(CC) $(CFLAGS) -c bar.cpp foo.cpp main.cpp
	$(CC) $(CFLAGS) -o $(TARGET) bar.o foo.o main.o

clean:
	/bin/sh -c 'rm -rf $(TARGET) $(TARGET).o'

run:
	@export LD_LIBRARY_PATH=$(LD_PATH) && ./$(TARGET) | c++filt -t
