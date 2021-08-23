CC = gcc
CFLAGS  = -g
TARGET = main


all: $(TARGET)

.PHONY: clean run

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -c $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o

clean:
	/bin/sh -c 'rm -rf $(TARGET) $(TARGET).o'

# run:
# 	@export LD_LIBRARY_PATH=$(LD_PATH) && ./$(TARGET) | c++filt -t
