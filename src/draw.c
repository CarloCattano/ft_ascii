#include "ftascii.h"

char *all_colors[8] = {RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE, BLACK};


static int check_border(int i, int MAX_COL, int MAX_ROW)
{
    if (i < MAX_COL || i > ((MAX_COL * MAX_ROW) - MAX_COL) || i % MAX_COL == 0 
            || i % MAX_COL == MAX_COL - 1){ 
            return 1;
    }
    return 0;
}

static void img2win(term_t *t)
{
    size_t size = t->size * 8;

    assign_pix_buff(t->buffer, t->pixels, t->size);
    write(1, t->buffer, size);
    memset(t->buffer, 0, size);
    write(1, "\033[H", 3);
    t->frame > 2048 ? t->frame = 1 : t->frame++;
}

static void background(term_t *t, int y)
{
    t->pixels[y].color = GREEN;
    t->pixels[y].uni = " "; // TODO bug here wide unicodes break it 
}

void map_pix(term_t *t, int x, int y, char *color, char *uni)
{
    int i = (y * t->MAX_COL) + x;
    fill_pixel(t->pixels, color, uni, i);
}


void draw(term_t *t, void (*draw_callback)(term_t *))
{
    for(int y = 0; y < t->size; y++)
    {
        // Border drawing
        if (check_border(y, t->MAX_COL, t->MAX_ROW)) { 
            t->pixels[y].color = all_colors[rand() % 4];
            t->pixels[y].uni = "█";
        } else {
            background(t, y);
            draw_callback(t);
        }                                                          
    }

    assign_pix_buff(t->buffer, t->pixels, t->size); 
    img2win(t);
}

