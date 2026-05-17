# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Iinclude

# Directories
SRC_DIR = src
OBJ_DIR = obj
EXAMPLE_DIR = examples
TEST_DIR = tests
SHOWCASE_DIR = showcase

SRC_FILES = draw.c init.c keyhooks.c pixel.c utils.c
OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

LIB = libftascii.a
EXAMPLE = example
SNAKE = snake
TEST_CORE = test_core
SHOWCASE_BINS = $(SHOWCASE_DIR)/flow2d $(SHOWCASE_DIR)/sacred2d $(SHOWCASE_DIR)/cube3d
SHOWCASE_COMMON_OBJ = $(OBJ_DIR)/showcase_common.o

$(shell mkdir -p $(OBJ_DIR))

.PHONY: all lib clean re run run-snake test showcase run-flow run-sacred run-cube

all: $(LIB) $(EXAMPLE) $(SNAKE) showcase

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/snake.o: $(EXAMPLE_DIR)/snake.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/test_core.o: $(TEST_DIR)/test_core.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/showcase_common.o: $(SHOWCASE_DIR)/common.c $(SHOWCASE_DIR)/common.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/flow2d.o: $(SHOWCASE_DIR)/flow2d.c $(SHOWCASE_DIR)/common.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/sacred2d.o: $(SHOWCASE_DIR)/sacred2d.c $(SHOWCASE_DIR)/common.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/cube3d.o: $(SHOWCASE_DIR)/cube3d.c $(SHOWCASE_DIR)/common.h
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB): $(OBJS)
	ar rcs $(LIB) $(OBJS)

$(EXAMPLE): $(OBJ_DIR)/main.o $(LIB)
	$(CC) $(CFLAGS) $(OBJ_DIR)/main.o -L. -lftascii -lm -o $(EXAMPLE)

$(SNAKE): $(OBJ_DIR)/snake.o $(LIB)
	$(CC) $(CFLAGS) $(OBJ_DIR)/snake.o -L. -lftascii -lm -o $(SNAKE)

$(TEST_CORE): $(OBJ_DIR)/test_core.o $(LIB)
	$(CC) $(CFLAGS) $(OBJ_DIR)/test_core.o -L. -lftascii -lm -o $(TEST_CORE)

$(SHOWCASE_DIR)/flow2d: $(OBJ_DIR)/flow2d.o $(SHOWCASE_COMMON_OBJ) $(LIB)
	$(CC) $(CFLAGS) $(OBJ_DIR)/flow2d.o $(SHOWCASE_COMMON_OBJ) -L. -lftascii -lm -o $@

$(SHOWCASE_DIR)/sacred2d: $(OBJ_DIR)/sacred2d.o $(SHOWCASE_COMMON_OBJ) $(LIB)
	$(CC) $(CFLAGS) $(OBJ_DIR)/sacred2d.o $(SHOWCASE_COMMON_OBJ) -L. -lftascii -lm -o $@

$(SHOWCASE_DIR)/cube3d: $(OBJ_DIR)/cube3d.o $(SHOWCASE_COMMON_OBJ) $(LIB)
	$(CC) $(CFLAGS) $(OBJ_DIR)/cube3d.o $(SHOWCASE_COMMON_OBJ) -L. -lftascii -lm -o $@

showcase: $(SHOWCASE_BINS)

lib: clean
	$(MAKE) $(LIB)

test: $(TEST_CORE)
	./$(TEST_CORE)

run: all
	./$(EXAMPLE)

run-snake: $(SNAKE)
	./$(SNAKE)

run-flow: $(SHOWCASE_DIR)/flow2d
	./$(SHOWCASE_DIR)/flow2d

run-sacred: $(SHOWCASE_DIR)/sacred2d
	./$(SHOWCASE_DIR)/sacred2d

run-cube: $(SHOWCASE_DIR)/cube3d
	./$(SHOWCASE_DIR)/cube3d

clean:
	rm -f $(OBJ_DIR)/*.o $(LIB) $(EXAMPLE) $(SNAKE) $(TEST_CORE) $(SHOWCASE_BINS)

re: clean all
