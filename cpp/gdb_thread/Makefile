CC = g++
CFLAGS  = -g
LDFLAGS = -lpthread
TARGET = main

all: $(TARGET)

.PHONY: clean run

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp $(LDFLAGS)

clean:
	/bin/sh -c 'rm -rf $(TARGET)'

run:
	@./$(TARGET)

debug:
	@gdb ./$(TARGET)
