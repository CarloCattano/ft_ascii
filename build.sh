#!/bin/bash

set -xe 

if [ $# -eq 0 ]; then
    cc -Wall -Wextra -Werror -lm main.c ftascii.c utils.c init.c draw.c keyhooks.c player.c pixel.c -o ft_ascii
    echo "Built ft_ascii"

elif [ $1 == "run" ]; then
    cc -Wall -Wextra -Werror -lm main.c utils.c ftascii.c init.c draw.c keyhooks.c player.c pixel.c -o ft_ascii
    ./ft_ascii

elif [ $1 == "clean" ]; then
    rm ft_ascii -f
    
elif [ $1 == "debug" ]; then
    cc -Wall -Wextra -Werror -g -lm main.c ftascii.c utils.c init.c draw.c keyhooks.c player.c pixel.c -o ft_ascii
    if [ $2 == "run" ]; then
        gdb ./ft_ascii
    else
        valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./ft_ascii
    fi
    
else
    echo "Invalid argument"
fi

