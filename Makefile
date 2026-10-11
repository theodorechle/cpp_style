SHARED_LIBS_BASE_PATH?=/usr/local
SHARED_LIBS_PATH:=$(SHARED_LIBS_BASE_PATH)/lib
SHARED_LIBS_INCLUDES_PATH:=$(SHARED_LIBS_BASE_PATH)/include
CPP_C:=g++
CPP_FLAGS:=-std=c++23 -Wall -g -MMD -MP -I$(SHARED_LIBS_INCLUDES_PATH)/cpptests
BIN_DIR:=bin
OBJ_DIR:=obj/lib
OBJ_TEST_DIR:=obj/test
SRC_DIR:=src
TESTS_DIR:=tests
COMMONS_LIB:=cpp_commons/bin/cpp_commons_lib.a
TESTS_LIB:=$(SHARED_LIBS_PATH)/libcpptests.so

# Source files
SRC_STYLE:=$(wildcard $(SRC_DIR)/*.cpp)
SRC_TESTS:=$(wildcard $(TESTS_DIR)/*.cpp) $(wildcard $(TESTS_DIR)/*/*.cpp)

# Object files
OBJ_STYLE:=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_STYLE))
OBJ_TESTS:=$(patsubst $(TESTS_DIR)/%.cpp, $(OBJ_TEST_DIR)/%.o, $(SRC_TESTS))

# Executable targets
LIB:=$(BIN_DIR)/cpp_style_lib
TESTS:=$(BIN_DIR)/tests

.PHONY: clean tests lib

ifeq ($(DEBUG),1)
CPP_FLAGS += -DDEBUG
endif

ifeq ($(DEBUG),2)
CPP_FLAGS += -DDEBUG -DDEBUG_DEBUG
endif

lib: $(LIB).a

tests: $(TESTS)

## LIB

$(LIB).a: $(OBJ_STYLE)
	@mkdir -p $(BIN_DIR)
	ar -r $@ $^

# Rule for compiling all object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CPP_C) $(CPP_FLAGS) -c $< -o $@

## TESTS
# Build the tests executable (tests + all style sources)
$(TESTS): $(OBJ_TESTS) $(LIB).a $(COMMONS_LIB) $(TESTS_LIB)
	@mkdir -p $(BIN_DIR)
	$(CPP_C) -o $@ $^ -lcpptests -Wl,-rpath,'$(SHARED_LIBS_PATH)'

# Rule for compiling all test files
$(OBJ_TEST_DIR)/%.o: $(TESTS_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CPP_C) $(CPP_FLAGS) -c $< -o $@

$(COMMONS_LIB):
	$(MAKE) -C cpp_commons lib DEBUG=$(DEBUG)

# Clean all generated files
clean:
	rm -rf bin obj
	$(MAKE) -C cpp_commons clean
