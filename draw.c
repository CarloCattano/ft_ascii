#include "ftascii.h"

static int check_border(term_t *t, int i)
{
    return (i < t->MAX_COL || i > (t->size - t->MAX_COL) || i % t->MAX_COL == 0 
            || i % t->MAX_COL == t->MAX_COL - 1);
}

static void delete_column_if_down(term_t *t, int posy, int posx)
{
    // Iterate over each row in the column at posx
    for (int i = posy - 1; i >= 1; i--)
    {
        t->buffer[i * t->MAX_COL + posx] = ' ';
    }
}

static void draw_player(term_t *t, player_t *p)
{
    int offset = (p->posy * t->MAX_COL) + p->posx;

    static int prev_posy = 0;

    if (p->posy > prev_posy) {
        t->buffer[offset] = p->curr_brush;
        delete_column_if_down(t, p->posy, p->posx);
    } else if (p->posy < prev_posy) {

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
    prev_posy = p->posy;
}

void draw(term_t *t, float *fft_values, float sens)
{
    for (int i = 0; i < t->size; i++)
    {
        if (check_border(t, i)) { // draw the border
            t->buffer[i] !=  '|' ? t->buffer[i] = '|' : 0;
        }
        else { // draw the inside
            for (int j = 0; j < FFT_SIZE / 2 + 1; j++) {
                draw_player(t, t->players[j]); 
            }
        }
    }

    compute_eq_bars(t, fft_values, sens);

    t->frame++;
    if (t->frame > 2048)
        t->frame = 1;
    
    
    t->sens > 8.0f ? t->sens = 8.0f : 0;
    t->sens < 0.125f ? t->sens = 0.125f : 0;

    t->delay < 1 ? t->delay = 1 : 0;
    t->delay > 3e5 ? t->delay = 3e5 : 0;

    write(1, t->buffer, t->size);       // draw the whole buffer in one call  
}
