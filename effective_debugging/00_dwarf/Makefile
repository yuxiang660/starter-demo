CC = g++
CFLAGS  = -g
TARGET = main


all: $(TARGET)

.PHONY: clean run

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -c $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o

clean:
	/bin/sh -c 'rm -rf $(TARGET) $(TARGET).o'

foo:
	g++ -g -S foo.cpp

debug_dump:
	g++ -g -c foo.cpp -o foo.o
	readelf --debug-dump foo.o > readelf.log
	objdump --dwarf --full-contents foo.o > objdump.log

