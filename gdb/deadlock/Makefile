CC = g++
CFLAGS  = -g
LFLAGS  = -lpthread
TARGET = main

all: clean $(TARGET)

.PHONY: clean run

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) $(TARGET).cpp $(LFLAGS) -o $(TARGET)

main2:
	gcc main.cpp -lpthread -lstdc++ -o main

clean:
	/bin/sh -c 'rm -rf $(TARGET)'

# For deadlock.py usage, please refer to https://github.com/facebook/folly/tree/main/folly/experimental/gdb
# > sudo gdb -p <PID> // have to use sudo
