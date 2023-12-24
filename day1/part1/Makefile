# Makefile for C project

# Compiler and compiler options
CC = gcc
CFLAGS = -std=c11 -Wall

# Source files and target executable
SRC_DIR = src
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
TARGET = my_program

# Object files (compiled sources)
OBJ_DIR = obj
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Build rule for the executable
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

# Build rule for object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create the object directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Phony target to clean up object files and the executable
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: clean


