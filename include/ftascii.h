#ifndef FTASCII_H
#define FTASCII_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/select.h>
#include <stdbool.h>

#include "const.h"

typedef struct s_unicode
{
    char*       uni;
    char*       color;
    
}               Pixel;

typedef struct player_s
{
    int         posx;
    int         posy;
    int         dx;
    int         dy;
}               player_t;

typedef struct term_s
{
    int             MAX_COL;
    int             MAX_ROW;
    int             size;
    char*           buffer;
	char* 		    buffer_copy;
    Pixel*          pixels;
	unsigned int    frame;
    int 		    clear;
    unsigned int    delay;
	bool            draw;
    player_t* 	    players[4];

}                   term_t;


void                ft_ascii();
void                init_term(term_t *t);

/*      hooks       */
void                handleKeyPress(char key, term_t *t);
void                ft_keyhook(term_t *t);

void                draw(term_t *t);
void                move_player(term_t *t);

/*     utils       */
int                 compare_val_in_buffers(term_t *t, int i);
void                copy_last_buffer(term_t *t);

void                free_all(term_t *t);
void                systemExit(term_t *t);

void                assign_pix_buff(char* buffer, Pixel* pixels,int size);
void                fill_pixel(Pixel* pixels, char* color, char* uni, int i);

int                 str_to_hex(char *str);
void                putpix(Pixel* pixels, char* color, char* uni);

char*               hex_to_unicode(int hex);

#endif
