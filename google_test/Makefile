GOOGLE_TEST_INCLUDE_PATH := ./include
GOOGLE_TEST_LIB_PATH := ./lib

INCLUDE  := -I $(GOOGLE_TEST_INCLUDE_PATH)

CXX      := /grid/common/pkgs/gcc/v9.3.0/bin/g++
CXXFLAGS := -pedantic-errors -Wall -Wextra -Werror
LDFLAGS  := -L $(GOOGLE_TEST_LIB_PATH) -lgtest -lpthread
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
RUNTIME_LD_PATH := /grid/common/pkgs/gccv9.3.0/lib:/grid/common/pkgs/gcc/v9.3.0/lib64

TARGET   := unit_test
SRC      := $(wildcard src/*.C)
OBJECTS  := $(SRC:%.C=$(OBJ_DIR)/%.o)

all: $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.C
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TARGET) $^ $(LDFLAGS)

.PHONY: all clean debug run help

debug: CXXFLAGS += -DDEBUG -g
debug: all

clean:
	@rm -rvf $(BUILD)

run:
	@export LD_LIBRARY_PATH=$(RUNTIME_LD_PATH) && $(APP_DIR)/$(TARGET)

help:
	@echo "	make - build unit test excutable file: $(APP_DIR)/$(TARGET)"
	@echo "	make debug - build debug version unit tests"
	@echo "	make run - run all unit tests"
	@echo "	make clean - remove all build files"


