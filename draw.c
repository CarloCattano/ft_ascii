#include "ftascii.h"

char *colors[] = {RED, GREEN, YELLOW, CYAN, MAGENTA, BLUE, WHITE, RST};

int number42[IMG_SIZE][IMG_SIZE] = {
    {1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0},
    {1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1},
    {1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1},
    {1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1},
    {1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1},
    {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1},
    {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1},
    {0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0},
    {0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1},
    {0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0},
};

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
    int len = 0;
    
    assign_pix_buff(t->buffer, t->pixels, t->size);
    len = write(1, t->buffer, t->size * 8);
    memset(t->buffer, 0, t->size * 8);
    write(1, "\033[H", 3);
    (void)len;
    t->frame > 2048 ? t->frame = 1 : t->frame++;
}

void background(term_t *t, int y)
{
    t->pixels[y].data.color = GREEN;
    t->pixels[y].data.uni = "."; // TODO bug here wide unicodes break it 
}

void map_pix(term_t *t, int x, int y, char *color, char *uni)
{
    int i = (y * t->MAX_COL) + x;
    fill_pixel(t->pixels, color, uni, i);
}

void draw42(term_t *t, int x, int y, char *color, char *uni)
{
    for (int i = 0; i < IMG_SIZE; i++)
    {
        for (int j = 0; j < IMG_SIZE; j++)
        {
            if (number42[i][j] == 1)
            {
                map_pix(t, x + j, y + i, color, uni);
            }
        }
    }
}

void draw(term_t *t)
{
     for(int y = 0; y < t->size; y++)
     {
        if (check_border(y, t->MAX_COL, t->MAX_ROW)) {
            t->pixels[y].data.color = colors[rand() % 8];
            t->pixels[y].data.uni = "🟕";
        } else {
            background(t, y);
            draw42(t, t->players[0]->posx - IMG_SIZE / 2 , 
                    t->players[0]->posy - IMG_SIZE / 2 , YELLOW, "ꡙ");
        }                                                          
    }
    move_player(t);
    map_pix(t, t->players[0]->posx, t->players[0]->posy, RED, "🟕");
    assign_pix_buff(t->buffer, t->pixels, t->size); 
    img2win(t);
}
