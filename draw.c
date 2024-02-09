#include "ftascii.h"

/* static void draw_player(term_t *t, int i, player_t *p) */
/* { */
/*         int offset = (p->posy * t->MAX_COL) + p->posx; */

/*         if (compare_val_in_buffers(t, offset)) */
/*         { */
/*             p->curr_brush = p->brushes[p->brush_index]; */
/*         } */
/*         else {          /1* replace the old brush with different one (swallow) *1/ */
/*             if (p->curr_brush == t->buffer[offset]) */
/*                 p->brush_index = (p->brush_index + 1) % strlen(p->brushes); */
            
/*             p->curr_brush = p->brushes[p->brush_index]; */
/*             p->toggle ? p->curr_brush = p->brushes[(p->brush_index % 2)] : 0; */
/*             t->buffer[offset] = p->curr_brush; */
/*         } */
/* } */

static int check_border(term_t *t, int i)
{
    return (i < t->MAX_COL || i > (t->size - t->MAX_COL) || i % t->MAX_COL == 0 
            || i % t->MAX_COL == t->MAX_COL - 1);
}

// unicode character array for drawing
char unis[] = "◝◞◟◠◡";
char ascis[] = " .:-=+*#%@";

// draw the terminal

void draw(term_t *t)
{
    for (int i = 0; i < t->size; i++)
    {
        if (check_border(t, i)){
            change_pixel(&t->pixels[i], "•", GREEN);
        } else {
            
            t->frame > 1024 ? change_pixel(&t->pixels[i], "◡", RED) : change_pixel(&t->pixels[i], ";", GREEN);
        }
    }
    assign_pix_buff(t->buffer, t->pixels, t->size);
    write(1, t->buffer, strlen(t->buffer));
    
    if (t->frame > 2048) t->frame = 1; else t->frame += 4;
}

