SHELL := /bin/csh

CC = g++
CFLAGS  = -g
TARGET = main

all: $(TARGET)

.PHONY: clean run debug

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp

clean:
	/bin/sh -c 'rm -rf $(TARGET)'

run:
	source setup.csh && ./$(TARGET)

debug:
	source setup.csh && /usr/bin/gdbserver sjcvl-yuxiangw:2000 ./$(TARGET)