# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude 

# Directories
SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include

# Source files
SRC_FILES = draw.c init.c keyhooks.c pixel.c player.c utils.c numbers.c
SRCS = $(patsubst %, $(SRC_DIR)/%, $(SRC_FILES))

# Object files
OBJS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Target library
LIB = libftascii.a


PONG = pong_cli
PONG_SRC = main.c
PONG_OBJ= $(OBJ_DIR)/main.o

# Create object directory if it doesn't exist
$(shell mkdir -p $(OBJ_DIR))

# Default target
all: $(LIB) $(PONG)

# Compile each source file into an object file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile the example program object file
$(OBJ_DIR)/main.o: $(PONG_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

# Archive the object files into a static library
$(LIB): $(OBJS)
	ar rcs $(LIB) $(OBJS)

# Link the example program
$(PONG): $(PONG_OBJ) $(LIB)
	$(CC) $(CFLAGS) $(PONG_OBJ) -L. -lftascii -o $(PONG)

# Clean up the build files
clean:
	rm -f $(OBJ_DIR)/*.o $(LIB) $(PONG)

