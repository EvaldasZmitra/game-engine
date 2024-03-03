# Makefile to build all .c files in src directory into .o files

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -I./include 
LDFLAGS = $(shell pkg-config --cflags glib-2.0 sdl2)

# Directories
SRC_DIR = ./src
BUILD_DIR = ./build

# Get all .c files in the source directory
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# Generate a list of corresponding .o files in the build directory
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Default target to build all .o files
all: $(OBJ_FILES)

# Rule to build .o files from .c files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)



clean:
	rm -f $(BUILD_DIR)/*.o

.PHONY: all clean