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

// The main structure that holds the data for each "pixel"
// consisting of a unicode character and a color
typedef struct s_unicode
{
    char*       uni;
    char*       color;
    
}               Pixel;

/** @brief  Structure to store the terminal data
 *  @param  MAX_COL     Maximum number of columns in the terminal
 *  @param  MAX_ROW     Maximum number of rows in the terminal
 *  @param  size        Size of the buffer MAX_COL * MAX_ROW
 *  @param  buffer      Buffer to store the terminal data
 *  @param  buffer_copy Copy of the buffer
 *  @param  pixels      Array of pixels
 *  @param  frame       Frame number
 *  @param  clear       Clear the terminal
 *  @param  delay       Delay between frames
 *  @param  draw        Draw flag to draw keep the drawing loop running
 */

typedef struct      term_s
{
    int             MAX_COL;
    int             MAX_ROW;
    int             size;
    char*           buffer;
    int             buffer_size;
    char*           buffer_copy;
    Pixel*          pixels;
    unsigned int    frame;
    int             clear;
    unsigned int    delay;
    bool            draw;

}                   term_t;


void                ft_ascii();
void                init_term(term_t *t);
void                draw(term_t *t, void (*f)(term_t*));
void                assign_pix_buff(char* buffer, Pixel* pixels,int size);
void                fill_pixel(Pixel* pixels, char* color, char* uni, int i);
void                putpix(Pixel* pixels, char* color, char* uni);
void                map_pix(term_t *t, int x, int y, char *color, char *uni);

/*      hooks       */
void                handleKeyPress(char key, term_t *t);
void                ft_keyhook(term_t *t);


/*     utils       */
int                 compare_val_in_buffers(term_t *t, int i);
void                copy_last_buffer(term_t *t);
void                free_all(term_t *t);
void                systemExit(term_t *t);

extern char*        all_colors[10];

#endif
