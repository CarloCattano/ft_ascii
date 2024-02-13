#include "ftascii.h"

void init_players(term_t *t)
{
    for (int i = 0; i < MAX_PLAYERS; i++) {
        t->players[i] = (player_t*)malloc(sizeof(player_t));
    
        if (t->players[i] == NULL){
            printf("Memory allocation failed\n");
            exit(1);
        }

        t->players[i]->posx = IMG_SIZE;
        t->players[i]->posy = IMG_SIZE;
        t->players[i]->dx = 1;
        t->players[i]->dy = 1;
    }
}

void init_term(term_t *t)
{
    // Initialize frame, clear, delay
    t->frame = 1;
    t->clear = 0;
    t->delay = 1e3;
	t->sens = 1.0f;
    
    // check term init
    if (t->size < 1){
        printf("Terminal size is invalid\n");
        exit(1);
    }

    t->pixels = (Pixel*)malloc(sizeof(Pixel) * t->size);
    t->buffer = (char*)malloc(sizeof(char) * t->size * 8);

    init_players(t);

    if (t->pixels == NULL || t->buffer == NULL){
          printf("Memory allocation failed\n");
          exit(1);
    }

    pix_set(t->pixels, t->size);
    memset(t->buffer, '.', t->size * 8); // fill buffer with dots
}
