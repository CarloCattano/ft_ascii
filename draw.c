#include "ftascii.h"

/* static int check_border(term_t *t, int i) */
/* { */
/*     return (i < t->MAX_COL || i > (t->size - t->MAX_COL) || i % t->MAX_COL == 0 */ 
/*             || i % t->MAX_COL == t->MAX_COL - 1); */
/* } */

void draw(term_t *t)
{
    if(!t->pixels || !t->buffer) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    write(1, t->buffer, t->size * 8);
    t->frame > 2048 ? t->frame = 1 : t->frame++;
}

