CC = g++
CFLAGS  = -g -fsanitize=address
TARGET = main
LD_PATH = /grid/common/pkgs/gcc/v6.3.0p1/lib:/grid/common/pkgs/gcc/v6.3.0p1/lib64

all: $(TARGET)

.PHONY: clean

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp

compile: $(TARGET).cpp
	$(CC) -g -o $(TARGET) $(TARGET).cpp

clean:
	/bin/sh -c 'rm -rf $(TARGET)'

run:
	@export LD_LIBRARY_PATH=$(LD_PATH) && ./$(TARGET)
