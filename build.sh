#!/bin/bash

set -e 

NAME="equalizer"

if [ $# -eq 0 ]; then
    cc draw.c fft.c ftascii.c init.c keyhooks.c player.c  utils.c  libportaudio.a \
            -lrt -lm -lasound -ljack -lfftw3 -pthread -o $NAME -Wall -Wextra -DFFT_SIZE=128
 
    echo "Built ft_ascii"

elif [ $1 == "audio" ]; then
    cc draw.c fft.c ftascii.c init.c keyhooks.c player.c  utils.c  libportaudio.a \
            -lrt -lm -lasound -ljack -lfftw3 -pthread -o $NAME -Wall -Wextra -DFFT_SIZE=128
    ./"$NAME"

elif [ $1 == "run" ]; then
    cc draw.c fft.c ftascii.c init.c keyhooks.c player.c  utils.c  libportaudio.a \
            -lrt -lm -lasound -ljack -lfftw3 -pthread -o $NAME -Wall -Wextra -DFFT_SIZE=128
    ./"$NAME"

elif [ $1 == "clean" ]; then
    rm $NAME -f
    
elif [ $1 == "debug" ]; then
    cc draw.c fft.c ftascii.c init.c keyhooks.c player.c  utils.c  libportaudio.a \
            -lrt -lm -lasound -ljack -lfftw3 -pthread -o $NAME -Wall -Wextra -DFFT_SIZE=32 -ggdb3 -DDEBUG
    if [ $2 == "run" ]; then
        gdb ./"$NAME"
    else
        valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$NAME
    fi
    
else
    echo "Invalid argument"
fi

