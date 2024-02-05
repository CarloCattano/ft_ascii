#include "ftascii.h"

static void draw_player(term_t *t, int i)
{
    if (i > t->MAX_COL && i < (t->size - t->MAX_COL) && i % t->MAX_COL != 0 
            && i % t->MAX_COL != t->MAX_COL - 1) 
    {
        int offset = (t->player.posy * t->MAX_COL) + t->player.posx;

        if (compare_val_in_buffers(t, offset))
        {
            t->player.curr_brush = t->player.brushes[t->player.brush_index];
        }
        else {          /* replace the old brush with different one (swallow) */
            if (t->player.curr_brush == t->buffer[offset])
                t->player.brush_index = (t->player.brush_index + 1) % strlen(t->player.brushes);

            t->player.curr_brush = t->player.brushes[t->player.brush_index];
            t->buffer[offset] = t->player.curr_brush;
        }
        t->player.toggle ? t->buffer[i] = t->player.brushes[rand() % 4] : 0;
        t->player.curr_brush = t->buffer[offset];
    }
}

void draw(term_t *t)
{
    for (int i = 0; i < t->size; i++)
    {
        if (i < t->MAX_COL || i > (t->size - t->MAX_COL) || i % t->MAX_COL == 0 
                || i % t->MAX_COL == t->MAX_COL - 1) {
            t->buffer[i] !=  '|' ? t->buffer[i] = '|' : 0;
        } else {
            draw_player(t, i);
        }
    }
    t->frame++;
    if (t->frame > 2048)
        t->frame = 1;

    write(1, t->buffer, t->size);       // draw the whole buffer in one call  
}

