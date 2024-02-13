#!/bin/bash

set -xe 

if [ $# -eq 0 ]; then
    cc draw.c main.c ftascii.c init.c keyhooks.c player.c  pixel.c utils.c  libportaudio.a \
        -lrt -lm -lasound -ljack -lfftw3 -pthread -o debug -Wall -Wextra
    ./debug

elif [ $1 == "clean" ]; then
    rm debug -f
    
elif [ $1 == "debug" ]; then
    cc draw.c main.c ftascii.c init.c keyhooks.c player.c  pixel.c utils.c  libportaudio.a \
        -lrt -lm -lasound -ljack -lfftw3 -pthread -o debug -Wall -Wextra -ggdb3 
    ./debug
    if [ $2 == "run" ]; then
        gdb ./debug
    else
        valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./debug
    fi
    
else
    echo "Invalid argument"
fi

