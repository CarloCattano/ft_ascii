#!/bin/bash

set -e 

if [ $# -eq 0 ]; then
    cc -Wall -Wextra -Werror -lm main.c utils.c init.c draw.c keyhooks.c player.c -o ft_ascii
    echo "Built ft_ascii"

elif [ $1 == "audio" ]; then
    cc draw.c fft.c ftascii.c init.c keyhooks.c player.c  utils.c  libportaudio.a \
            -lrt -lm -lasound -ljack -lfftw3 -pthread -o testfft -Wall -Wextra -ggdb3 -O3 -DFFT_SIZE=64;
    ./testfft

elif [ $1 == "run" ]; then
    cc -Wall -Wextra -Werror -lm main.c utils.c init.c draw.c keyhooks.c player.c -o ft_ascii
    ./ft_ascii

elif [ $1 == "clean" ]; then
    rm ft_ascii -f
    
elif [ $1 == "debug" ]; then
    cc -Wall -Wextra -Werror -g -lm main.c utils.c init.c draw.c keyhooks.c player.c -o ft_ascii
    if [ $2 == "run" ]; then
        gdb ./ft_ascii
    else
        valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./ft_ascii
    fi
    
else
    echo "Invalid argument"
fi

