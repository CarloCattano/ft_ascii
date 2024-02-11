#include "ftascii.h"

char *colors[] = {RED, GREEN, YELLOW, CYAN, MAGENTA, BLUE, WHITE};

static int check_border(int i, int MAX_COL, int MAX_ROW)
{
    if (i < MAX_COL || i > ((MAX_COL * MAX_ROW) - MAX_COL) || i % MAX_COL == 0 
            || i % MAX_COL == MAX_COL - 1){ 
            return 1;
    }
    return 0;
}
void draw(term_t *t)
{
     for(int y = 0; y < t->size; y++) {
        if (check_border(y, t->MAX_COL, t->MAX_ROW)) {
            t->pixels[y].data.color = colors[rand() % 7];
            t->pixels[y].data.uni = "𐇺";
        } else {
            // Initialize color and Unicode for non-border pixels
            t->pixels[y].data.color = GREEN;
            t->pixels[y].data.uni = ".";
        }
    }

    int len = 0;

    assign_pix_buff(t->buffer, t->pixels, t->size);

    len = write(1, t->buffer, t->size * 8);

    (void)len;
    memset(t->buffer, 0, t->size * 8);
    t->frame > 2048 ? t->frame = 1 : t->frame++;
}
