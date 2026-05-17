# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Iinclude

# Directories
SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include
EXAMPLE_DIR = examples
TEST_DIR = tests

SRC_FILES = draw.c init.c keyhooks.c pixel.c utils.c
SRCS = $(patsubst %, $(SRC_DIR)/%, $(SRC_FILES))
OBJS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Target library and programs
LIB = libftascii.a
EXAMPLE = example
EXAMPLE_SRC = main.c
EXAMPLE_OBJ = $(OBJ_DIR)/main.o
SNAKE = snake
SNAKE_SRC = $(EXAMPLE_DIR)/snake.c
SNAKE_OBJ = $(OBJ_DIR)/snake.o
TEST_CORE = test_core
TEST_CORE_SRC = $(TEST_DIR)/test_core.c
TEST_CORE_OBJ = $(OBJ_DIR)/test_core.o

$(shell mkdir -p $(OBJ_DIR))

.PHONY: all lib clean re run run-snake test

all: $(LIB) $(EXAMPLE) $(SNAKE)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/main.o: $(EXAMPLE_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/snake.o: $(SNAKE_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/test_core.o: $(TEST_CORE_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB): $(OBJS)
	ar rcs $(LIB) $(OBJS)

$(EXAMPLE): $(EXAMPLE_OBJ) $(LIB)
	$(CC) $(CFLAGS) $(EXAMPLE_OBJ) -L. -lftascii -lm -o $(EXAMPLE)

$(SNAKE): $(SNAKE_OBJ) $(LIB)
	$(CC) $(CFLAGS) $(SNAKE_OBJ) -L. -lftascii -lm -o $(SNAKE)

$(TEST_CORE): $(TEST_CORE_OBJ) $(LIB)
	$(CC) $(CFLAGS) $(TEST_CORE_OBJ) -L. -lftascii -lm -o $(TEST_CORE)

lib: clean
	$(MAKE) $(LIB)

test: $(TEST_CORE)
	./$(TEST_CORE)

run: all
	./$(EXAMPLE)

run-snake: $(SNAKE)
	./$(SNAKE)

clean:
	rm -f $(OBJ_DIR)/*.o $(LIB) $(EXAMPLE) $(SNAKE) $(TEST_CORE)

re: clean all
