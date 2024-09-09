#ifndef CONST_H
#define CONST_H


#include <stdbool.h>

#define   CLEAR     "\033[H"
#define   NOMOUSE   "\033[?25l"
#define   CURSOR    "\033[1;1H"

#define   RED        "\033[31m"
#define   ORANGE     "\033[38;5;208m"
#define   YELLOW     "\033[33m"
#define   GREEN      "\033[32m"
#define   BLUE       "\033[34m"
#define   INDIGO     "\033[38;5;75m"
#define   VIOLET     "\033[38;5;93m"
#define   MAGENTA    "\033[35m"
#define   CYAN       "\033[36m"
#define   WHITE      "\033[37m"

#define   RST       "\033[0m"

#define IMG_SIZE 12

extern char zero[6][8];
extern char one[6][8];
extern char two[6][8];
extern char three[6][8];
extern char four[6][8];
extern char five[6][8];
extern char six[6][8];
extern char seven[6][8];
extern char eight[6][8];
extern char nine[6][8];

extern char p1wins[7][48];

#endif
