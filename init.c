#include "ftascii.h"

void init_term(term_t *t)
{
    // Initialize frame, clear, delay
    t->frame = 1;
    t->clear = 0;
    t->delay = 3e4;
    
    // check term init
    if (t->size < 1){
        printf("Terminal size is invalid\n");
        exit(1);
    }

    t->pixels = (Pixel*)malloc(sizeof(Pixel) * t->size);
    t->buffer = (char*)malloc(sizeof(char) * t->size * 8);

    t->players[0] = (player_t*)malloc(sizeof(player_t));

    t->players[0]->posx = t->MAX_COL / 2;
    t->players[0]->posy = t->MAX_ROW / 2;
    t->players[0]->dx = 0;
    t->players[0]->dy = 0;

    if (t->pixels == NULL || t->buffer == NULL || t->players[0] == NULL){
          printf("Memory allocation failed\n");
          exit(1);
    }

    pix_set(t->pixels, t->size);
    memset(t->buffer, '.', t->size * 8);
}
