#include "ftascii.h"

static char *colors[] = {RED, GREEN, YELLOW, CYAN, MAGENTA, BLUE, WHITE, RST};

static bool number42[IMG_SIZE][IMG_SIZE] = 
{
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

static bool number24[IMG_SIZE][IMG_SIZE] = 
{
    {1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0},
    {1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
    {1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1},
    {1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1},
    {1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1},
    {0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1},
    {0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1},
    {0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1},
    {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1},
    {0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
};

static void flip_arr(bool arr[12][12])
{
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			arr[i][j] = !arr[i][j];
		}
	}
}


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
    t->pixels[y].uni = "."; // TODO bug here wide unicodes break it 
}

static void map_pix(term_t *t, int x, int y, char *color, char *uni)
{
    int i = (y * t->MAX_COL) + x;
    fill_pixel(t->pixels, color, uni, i);
}

static void draw42(term_t *t, int x, int y, char *color, char *uni)
{
    for (int i = 0; i < IMG_SIZE; i++)
    {
        for (int j = 0; j < IMG_SIZE; j++)
        {
            if(t->clear)
            {
                if ( number42[i][j] == true)
                {
                    map_pix(t, x + j, y + i, color, uni);
                }
            } else if(!t->clear)
            {
                if ( number24[i][j] == true)
                {
                    map_pix(t, x + j, y + i, color, "آ");
                }
            }
        }
    }
}

void draw(term_t *t)
{
	for(int y = 0; y < t->size; y++)
	{
		if (check_border(y, t->MAX_COL, t->MAX_ROW)) {
			t->pixels[y].color = colors[rand() % 8];
			t->pixels[y].uni = "ش";
		} else {
			background(t, y);
			draw42(t, t->players[0]->posx - IMG_SIZE / 2 , 
					t->players[0]->posy - IMG_SIZE / 2 , YELLOW, "*");
		}                                                          
	}
	if(t->frame % 64 == 0 )
    {
        if (!t->clear){
            flip_arr(number24);
        } else if (t->clear) {
		    flip_arr(number42);
        }
    }

    move_player(t);
    map_pix(t, t->players[0]->posx, t->players[0]->posy, RED, "ش");
    assign_pix_buff(t->buffer, t->pixels, t->size); 
    img2win(t);
}
