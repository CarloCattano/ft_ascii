#include "ftascii.h"

static int check_border(term_t *t, int i)
{
    return (i < t->MAX_COL || i > (t->size - t->MAX_COL) || i % t->MAX_COL == 0 
            || i % t->MAX_COL == t->MAX_COL - 1);
}

/* char unis[] = "◝◞◟◠◡"; */

static void draw_player(term_t *t, int i, player_t *p)
{
    if (!check_border(t, i) && (i % t->MAX_COL) != 0 && (i % t->MAX_COL) != t->MAX_COL - 1)
    {
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
            /* if (!p->toggle) */
            /* { */
            /*     /1* p->brush_index = (((p->brush_index + 1) % 2) + (rand() % 6)) % strlen(p->brushes); *1/ */
            /* } */
            p->curr_brush = p->brushes[p->brush_index];
            t->buffer[offset] = p->curr_brush; 
        }
    }
}


void draw(term_t *t, float amplitude, float *fft_values, float* sens)
{
    for (int i = 0; i < t->size; i++)
    {
        if (check_border(t, i)){
            t->buffer[i] !=  '|' ? t->buffer[i] = '|' : 0;
        }
        /* }else if ( i < 2 * t->MAX_COL + (t->MAX_ROW / t->MAX_COL) && i > t->MAX_COL - t->MAX_ROW / 2) { */
        /*     char frame[8]; */
        /*     sprintf(frame, "%d", t->frame); */
        /*     t->buffer[i] = frame[i - (t->MAX_COL - t->MAX_ROW / 2)]; */
        /* } */
        else {
            int scale = ((rand() % 100) + 0.2f * *sens);
            /* if (amplitude > scale && t->frame % 2 == 0){ */
            if (fft_values[1] > 0.4f) {
                draw_player(t, i, t->players[0]);
                draw_player(t, i, t->players[1]);
                draw_player(t, i, t->players[2]);
                draw_player(t, i, t->players[3]);
            }
        }
    }     

    t->frame++;
    if (t->frame > 2048)
        t->frame = 1;
 
    t->sens > 4.0f ? t->sens = 4.0f : 0;
    t->sens < 0.25f ? t->sens = 0.25f : 0;

    t->delay < 1 ? t->delay = 1 : 0;
    t->delay > 10000 ? t->delay = 10000 : 0;


    write(1, t->buffer, t->size);       // draw the whole buffer in one call  
}
