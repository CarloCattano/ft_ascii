#!/bin/bash

set -e 

if [ $# -eq 0 ]; then
    cc -Wall -Wextra -Werror -I include/  -lm src/main.c src/ftascii.c src/utils.c src/init.c src/draw.c src/keyhooks.c src/player.c -o ft_ascii
    echo "Built ft_ascii"

elif [ $1 == "run" ]; then
    cc -Wall -Wextra -Werror -I include/ -lm src/main.c src/ftascii.c src/utils.c src/init.c src/draw.c src/keyhooks.c src/player.c -o ft_ascii
    ./ft_ascii

elif [ $1 == "clean" ]; then
    rm ft_ascii -f
    
elif [ $1 == "debug" ]; then
    cc -Wall -Wextra -Werror -I include/ -g -lm src/main.c src/ftascii.c src/utils.c src/init.c src/draw.c src/keyhooks.c src/player.c -o ft_ascii
    if [ $2 == "run" ]; then
        gdb ./ft_ascii
    else
        valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./ft_ascii
    fi
    
else
    echo "Invalid argument"
fi

