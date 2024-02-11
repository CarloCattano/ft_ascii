#include "ftascii.h"

/* static int check_border(term_t *t, int i) */
/* { */
/*     return (i < t->MAX_COL || i > (t->size - t->MAX_COL) || i % t->MAX_COL == 0 */ 
/*             || i % t->MAX_COL == t->MAX_COL - 1); */
/* } */


void draw(term_t *t)
{
    /* for (int i = 0; i < t->size; i++) */
    /* { */

    /*     /1* if (i < t->MAX_COL || i > (t->size - t->MAX_COL) || i % t->MAX_COL == 0 *1/ */ 
    /*     /1*         || i % t->MAX_COL == t->MAX_COL - 1) *1/ */
    /*     /1* { *1/ */
    /*     /1*     change_pixel(&t->pixels[i], "▅", RED); *1/ */
    /*     /1* } *1/ */
    /*     /1* else *1/ */
    /*     /1* { *1/ */
    /*     /1*     change_pixel(&t->pixels[i], "█", GREEN); *1/ */
    /*     /1* } *1/ */
    /*     //change pixel information */
        
    /* } */
    
    if(t->pixels == NULL || t->buffer == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }

    assign_pix_buff(t->buffer, t->pixels, t->size);

    write(1, t->buffer, t->size * 8);
    t->frame > 2048 ? t->frame = 1 : t->frame++;
}

