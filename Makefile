# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2

# Directories
SRC_DIR = .
OBJ_DIR = obj
BIN_DIR = bin

# Targets and files
LIB_NAME = libmy_malloc.a
LIB_TARGET = $(BIN_DIR)/$(LIB_NAME)
MY_MALLOC_SRC = $(SRC_DIR)/my_malloc.c
MY_MALLOC_OBJ = $(OBJ_DIR)/my_malloc.o

TEST_NAME = test
TEST_TARGET = $(BIN_DIR)/$(TEST_NAME)
MY_MALLOC_TEST_SRC = $(SRC_DIR)/my_malloc_test.c
MY_MALLOC_TEST_OBJ = $(OBJ_DIR)/my_malloc_test.o

# Default target
default: $(LIB_TARGET) $(TEST_TARGET)

# Build static library
$(LIB_TARGET): $(MY_MALLOC_OBJ)
	@mkdir -p $(BIN_DIR)
	ar rcs $@ $^

# Build object file for my_malloc
$(MY_MALLOC_OBJ): $(MY_MALLOC_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build test executable
$(TEST_TARGET): $(MY_MALLOC_TEST_OBJ) $(LIB_TARGET)
	$(CC) $(CFLAGS) -o $@ $< -L$(BIN_DIR) -lmy_malloc

# Build object file for test
$(MY_MALLOC_TEST_OBJ): $(MY_MALLOC_TEST_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Alias for the test target
test: $(TEST_TARGET)

# Clean up
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: default clean
