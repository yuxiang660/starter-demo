CC = g++
CFLAGS  = -g
TARGET = main

all: $(TARGET)

.PHONY: clean

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp

compile: $(TARGET).cpp
	$(CC) -g -o $(TARGET) $(TARGET).cpp

clean:
	/bin/sh -c 'rm -rf $(TARGET)'

run:
	./$(TARGET)

debug:
	/usr/bin/gdbserver ${GDB_SERVER} ./$(TARGET)
