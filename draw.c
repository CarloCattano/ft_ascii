#include "ftascii.h"

static int check_border(term_t *t, int i)
{
    return (i < t->MAX_COL || i > (t->size - t->MAX_COL) || i % t->MAX_COL == 0 
            || i % t->MAX_COL == t->MAX_COL - 1);
}

static void draw_player(term_t *t, int i, player_t *p)
{
        (void)i ;
        int offset = (p->posy * t->MAX_COL) + p->posx;

        if (compare_val_in_buffers(t, offset)) 
        {
            if (!p->toggle)
                p->curr_brush = p->brushes[p->brush_index + 1 % strlen(p->brushes)];
            else
               p->curr_brush = p->brushes[p->brush_index];
        } else {          /* replace the old brush with different one (swallow) */
            if (p->curr_brush == t->buffer[offset] && p->toggle)
            {
                p->brush_index = (p->brush_index + 1) % strlen(p->brushes);
            }
            p->curr_brush = p->brushes[p->brush_index];
            t->buffer[offset] = p->curr_brush; 
        }
}

void draw(term_t *t, float *fft_values, float* sens)
{
    (void)sens;
    for (int i = 0; i < t->size; i++)
    {
        if (check_border(t, i)){
            t->buffer[i] !=  '|' ? t->buffer[i] = '|' : 0;
        }
        else {
            for (int j = 0; j < FFT_SIZE / 2 + 1; j++) {
                draw_player(t, i, t->players[j]);
            }
        }
    }     

    t->frame++;
    if (t->frame > 2048)
        t->frame = 1;
    
    for (int i = 0; i < FFT_SIZE / 2 + 1; i++)
       move_player_to(t->players[i], t->MAX_COL, t->MAX_ROW, i, fft_values[i]);

    t->sens > 4.0f ? t->sens = 4.0f : 0;
    t->sens < 0.25f ? t->sens = 0.25f : 0;

    t->delay < 1 ? t->delay = 1 : 0;
    t->delay > 3e5 ? t->delay = 3e5 : 0;

    write(1, t->buffer, t->size);       // draw the whole buffer in one call  
}
