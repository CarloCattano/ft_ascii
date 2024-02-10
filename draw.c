#include "ftascii.h"

static void draw_player(term_t *t, int i, player_t *p)
{
    if (i > t->MAX_COL && i < (t->size - t->MAX_COL) && i % t->MAX_COL != 0 
            && i % t->MAX_COL != t->MAX_COL - 1) 
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
            if (!p->toggle)
            {
                p->brush_index = (((p->brush_index + 1) % 2) + (rand() % 6)) % strlen(p->brushes);
            }
            p->curr_brush = p->brushes[p->brush_index];
            t->buffer[offset] = p->curr_brush; 
        }
       
    }
}

void draw(term_t *t, float amplitude)
{
    for (int i = 0; i < t->size; i++)
    {
        if (i < t->MAX_COL || i > (t->size - t->MAX_COL) || i % t->MAX_COL == 0 
                || i % t->MAX_COL == t->MAX_COL - 1) {
            t->buffer[i] !=  '|' ? t->buffer[i] = '|' : 0;
            //rectangle of 2x18 on the top right , in order to 
            // draw debug info in there 
        }else if ( i < 2 * t->MAX_COL + (t->MAX_ROW / t->MAX_COL) && i > t->MAX_COL - t->MAX_ROW / 2) {
            /* char debug[16] = "** "; */
            /* strcat(debug, "Frame: "); */
            /* // cut the end of the string out */
            /* // and dont add the null terminator */
            /* // to the buffer */
            /* char frame[5]; */
            /* sprintf(frame, "%d", t->frame); */
            /* strcat(debug, frame); */
            /* strcat(debug, " **"); */
            /* t->buffer[i] = debug[i - t->MAX_COL]; */
        }   
        else {
            int scale = ((rand() % 100) + 0.2f);
            if (amplitude > scale && t->frame % 2 == 0){
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

    write(1, t->buffer, t->size);       // draw the whole buffer in one call  
}

