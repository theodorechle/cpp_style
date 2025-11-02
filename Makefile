CPP_C=g++
CPP_FLAGS=-std=c++17 -Wall -g -MMD -MP
BIN_DIR=bin
OBJ_DIR=obj/lib
OBJ_TEST_DIR=obj/test
SRC_DIR=src
TESTS_DIR=tests
LIB=bin/cpp_style_lib
TESTS_LIB=cpp_tests/bin/cpp_tests_lib
COMMONS_LIB=cpp_commons/bin/cpp_commons_lib

# Source files
SRC_STYLE=$(wildcard $(SRC_DIR)/style/*.cpp) $(wildcard $(SRC_DIR)/style_nodes/*.cpp)
SRC_TESTS=$(wildcard $(TESTS_DIR)/*/*.cpp) $(TESTS_DIR)/main.cpp

# Object files
OBJ_STYLE=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_STYLE))
OBJ_TESTS=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_TEST_DIR)/%.o, $(SRC_TESTS))

# Executable targets
LIB=$(BIN_DIR)/cpp_style_lib
TESTS=$(BIN_DIR)/tests

.PHONY: clean tests lib

ifeq ($(DEBUG),1)
CPP_FLAGS += -DDEBUG
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
$(TESTS): $(OBJ_TESTS) $(LIB).a $(COMMONS_LIB).a $(TESTS_LIB).a
	@mkdir -p $(BIN_DIR)
	$(CPP_C) $(CPP_FLAGS) -o $@ $^

# Rule for compiling all test files
$(OBJ_TEST_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CPP_C) $(CPP_FLAGS) -DDEBUG -c $< -o $@

$(TESTS_LIB).a:
	$(MAKE) -C cpp_tests -j lib BASH_COLORS=$(BASH_COLORS)

$(COMMONS_LIB).a:
	$(MAKE) -C cpp_commons -j lib

# Clean all generated files
clean:
	@find obj -mindepth 1 ! -name .gitkeep -delete
	@find bin -mindepth 1 ! -name .gitkeep -delete
	$(MAKE) -C cpp_tests clean