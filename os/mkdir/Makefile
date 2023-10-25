CC = g++
CFLAGS  = -g
TARGET = main

all: $(TARGET)

.PHONY: clean run

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp

clean:
	/bin/sh -c 'rm -rf $(TARGET)'

run:
	@./$(TARGET)