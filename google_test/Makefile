GOOGLE_TEST_INCLUDE_PATH = ./framework/include
GOOGLE_TEST_LIB_PATH = ./framework/lib

G++ = /grid/common/pkgs/gcc/v9.3.0/bin/g++
LD_PATH = /grid/common/pkgs/gccv9.3.0/lib:/grid/common/pkgs/gcc/v9.3.0/lib64

C_FLAGS = -g -c -Wall -I $(GOOGLE_TEST_INCLUDE_PATH)
LD_FLAGS = -L $(GOOGLE_TEST_LIB_PATH) -lgtest -lpthread

OBJECTS = main.o foo.o foo_test.o
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(G++) -o $(TARGET) $(OBJECTS) $(LD_FLAGS)

%.o : %.cpp
	$(G++) $(C_FLAGS) $<

clean:
	rm -f $(TARGET) $(OBJECTS)

run:
	@export LD_LIBRARY_PATH=$(LD_PATH) && ./$(TARGET)

.PHONY: all clean
