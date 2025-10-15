# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Iinclude

# Directories
SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include

SRC_FILES = draw.c init.c keyhooks.c pixel.c utils.c

SRCS = $(patsubst %, $(SRC_DIR)/%, $(SRC_FILES))
OBJS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Target library
LIB = libftascii.a

EXAMPLE = example
EXAMPLE_SRC = main.c

EXAMPLE_OBJ= $(OBJ_DIR)/main.o

# Create object directory if it doesn't exist
$(shell mkdir -p $(OBJ_DIR))

# Default target
all: $(LIB) $(EXAMPLE)

# Compile each source file into an object file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile the example program object file
$(OBJ_DIR)/main.o: $(EXAMPLE_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

# Archive the object files into a static library
$(LIB): $(OBJS)
	ar rcs $(LIB) $(OBJS)

# Link the example program
$(EXAMPLE): $(EXAMPLE_OBJ) $(LIB)
	$(CC) $(CFLAGS) $(EXAMPLE_OBJ) -L. -lftascii -lm -o $(EXAMPLE)

lib: clean
	$(MAKE) $(LIB)

# Clean up the build files
clean:
	rm -f $(OBJ_DIR)/*.o $(LIB) $(EXAMPLE)

re: clean all

run: all
	./$(EXAMPLE)
