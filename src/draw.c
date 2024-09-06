#include "ftascii.h"

char *all_colors[8] = {GREEN, BLUE, BLACK, MAGENTA, CYAN, WHITE, YELLOW, RED};


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
    {
    // Border drawing
    for(int y = 0; y < t->size; y++)
        if (check_border(y, t->MAX_COL, t->MAX_ROW)) { 
            // t->pixels[y].color = all_colors[rand() % 4];
            float frequency = 0.1;   
            float phase_shift = t->frame * 0.1;

            int color_index = (int)((sin(frequency * y + phase_shift) + 1) * 4) % 7;  // Scale from -1 to 1 -> 0 to 4 -> 0 to 5
            t->pixels[y].color = all_colors[color_index];
            t->pixels[y].uni = "█";
        } else {
            background(t, y);
            draw_callback(t);
        }                                                          
    }

    assign_pix_buff(t->buffer, t->pixels, t->size); 
    img2win(t);
}

