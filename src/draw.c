#include "ftascii.h"

char *all_colors[10] = {RED, ORANGE, YELLOW, GREEN, BLUE, INDIGO, VIOLET, MAGENTA, CYAN, WHITE};

// checks if the pixel is on the border of the screen to draw a border
static int check_border(int i, int MAX_COL, int MAX_ROW)
{
    if (i < MAX_COL || i > ((MAX_COL * MAX_ROW) - MAX_COL) || i % MAX_COL == 0 
            || i % MAX_COL == MAX_COL - 1){ 
            return 1;
    }
    return 0;
}

// Image to window - writes the buffer to the terminal
// in one single write call
static void img2win(term_t *t)
{
    size_t size = t->size * 8;

    assign_pix_buff(t->buffer, t->pixels, t->size);
    write(1, t->buffer, size);
    memset(t->buffer, 0, size);

    // frame counter clipping
    t->frame > 2048 ? t->frame = 1 : t->frame++;
}

static void background(term_t *t, int y)
{
    t->pixels[y].color = GREEN;
    t->pixels[y].uni = " "; 
}

// Fills the pixel with the specified color and unicode character
void map_pix(term_t *t, int x, int y, char *color, char *uni)
{
    int i = (y * t->MAX_COL) + x;
    fill_pixel(t->pixels, color, uni, i);
}

static void animated_border(term_t *t, int y)
{
    float frequency = 0.1;   
    float phase_shift = t->frame * 0.1;
    int color_index = (int)((sin(frequency * y + phase_shift) + 1) * 2) % 10;
    t->pixels[y].color = all_colors[color_index];
    t->pixels[y].uni = "█";
}

// Draws a border and the content to the terminal with the specified draw_callback
void draw(term_t *t, void (*draw_callback)(term_t *))
{
    for(int y = 0; y < t->size; y++)
    {
        // Border drawing with color changing animation
        if (check_border(y, t->MAX_COL, t->MAX_ROW)) { 
            animated_border(t, y);
        } 
        else {
            background(t, y);
            draw_callback(t);
        }                                                          
    }
    
    // Write the buffer to the terminal
    assign_pix_buff(t->buffer, t->pixels, t->size); 
    img2win(t);
}
