#include "ftascii.h"

static int check_border(term_t *t, int i)
{
    return (i < t->MAX_COL || i > (t->size - t->MAX_COL) || i % t->MAX_COL == 0 
            || i % t->MAX_COL == t->MAX_COL - 1);
}
char unis[] = "◝◞◟◠◡";

// draw the buffer

static int ft_strlen(char *s)
{
    int i = 0;
    while (s[i] && s[i + 1] != '\0')
        i++;
    s[i] = '\0';
    return i;
}

void draw(term_t *t)
{
    for (int i = 0; i < t->size; i++)
    {
        if (check_border(t, i)){
            change_pixel(&t->pixels[i], "|",GREEN);
        } else {
            if (t->buffer[i] != t->buffer_copy[i] && t->frame % 256 == 0)
            {
                flip_pixel(&t->pixels[i]);
            }
        }
    }
    assign_pix_buff(t->buffer, t->pixels, t->size);

    write(1, t->buffer, ft_strlen(t->buffer));
    
    t->frame > 2048 ? t->frame = 1 : t->frame++;
}

