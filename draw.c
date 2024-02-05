#include "ftascii.h"

int random_minus_plus()
{
    int r = rand() % 2;
    if (r == 0)
        r = -1;
    return 1;
}

static void draw_player(term_t *t, int i, player_t *p)
{
    if (i > t->MAX_COL && i < (t->size - t->MAX_COL) && i % t->MAX_COL != 0 
            && i % t->MAX_COL != t->MAX_COL - 1) 
    {
        int offset = (p->posy * t->MAX_COL) + p->posx;

        if (compare_val_in_buffers(t, offset))
        {
            p->curr_brush = p->brushes[p->brush_index];
        }
        else {          /* replace the old brush with different one (swallow) */
            if (p->curr_brush == t->buffer[offset])
                p->brush_index = (p->brush_index + random_minus_plus()) % strlen(p->brushes);
            
            p->curr_brush = p->brushes[p->brush_index];
            p->toggle ? p->curr_brush = p->brushes[((i + random_minus_plus()) % 3)] : 0;

            t->buffer[offset] = p->curr_brush;
        }
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
            draw_player(t, i, t->players[0]);
            draw_player(t, i, t->players[1]);
            draw_player(t, i, t->players[2]);
            draw_player(t, i, t->players[3]);
        }
    }
    t->frame++;
    if (t->frame > 2048)
        t->frame = 1;

    write(1, t->buffer, t->size);       // draw the whole buffer in one call  
}

