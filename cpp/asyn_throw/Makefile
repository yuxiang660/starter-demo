CC = /grid/common/pkgs/gcc/v9.3.0p4/bin/g++
CFLAGS  = -g
TARGET = main
LD_PATH = /grid/common/pkgs/gcc/v9.3.0p4/lib:/grid/common/pkgs/gcc/v9.3.0p4/lib64


all: $(TARGET)

.PHONY: clean run

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -c $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o -pthread

clean:
	/bin/sh -c 'rm -rf $(TARGET) $(TARGET).o'

run:
	@export LD_LIBRARY_PATH=$(LD_PATH) && ./$(TARGET) | c++filt -t
