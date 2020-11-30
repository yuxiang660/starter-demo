# Makefile for gtest examples

GOOGLE_TEST_INCLUDE_PATH = ./framework/include
GOOGLE_TEST_LIB_PATH = ./framework/lib

G++ = g++
G++_FLAGS = -g -c -Wall -I $(GOOGLE_TEST_INCLUDE_PATH)
LD_FLAGS = -L $(GOOGLE_TEST_LIB_PATH) -lgtest -lpthread

OBJECTS = main.o foo.o foo_test.o
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(G++) -o $(TARGET) $(OBJECTS) $(LD_FLAGS)

%.o : %.cpp
	$(G++) $(G++_FLAGS) $<

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean
