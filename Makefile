CC = cc
CFLAGS = -Wall -Wextra -Iinclude -g
SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include

SRC_FILES = draw.c init.c keyhooks.c pixel.c player.c utils.c numbers.c
SRCS = $(patsubst %, $(SRC_DIR)/%, $(SRC_FILES))

OBJS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

LIB = libftascii.a

PONG = pong_cli
PONG_SRC = main.c

PONG_OBJ= $(OBJ_DIR)/main.o

$(shell mkdir -p $(OBJ_DIR))

all: $(LIB) $(PONG)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/main.o: $(PONG_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB): $(OBJS)
	ar rcs $(LIB) $(OBJS)

$(PONG): $(PONG_OBJ) $(LIB)
	$(CC) $(CFLAGS) $(PONG_OBJ) -L. -lftascii -o $(PONG) -lpthread -lm -ldl

clean:
	rm -f $(OBJ_DIR)/*.o $(LIB) $(PONG)

re: clean all

run: all
	alacritty -e ./$(PONG) & python3 router.py
