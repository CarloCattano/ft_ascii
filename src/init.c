#include "ftascii.h"

void init_term(term_t *t)
{
    t->frame = 1;
    t->clear = 0;
    t->delay = 2e4;
    
    // Check if terminal size is valid
    if (t->size < 1){
        printf("Terminal size is invalid\n");
        exit(1);
    }
    t->pixels = (Pixel*)malloc(sizeof(Pixel) * t->size);
    if (t->pixels == NULL) {
        printf("Memory allocation failed for pixels\n");
        exit(1);
    }
    // Allocate buffer - final char output to the terminal 
    t->buffer = (char*)malloc(sizeof(char) * t->size * 8); // 8 or 9 ?? color (5) + uni(3)
    if (t->buffer == NULL) {
        printf("Memory allocation failed for buffer\n");
        free(t->pixels); 
        exit(1);
    }
    // Player aka Brushes / test drawing headsa
    t->players[0] = (player_t*)malloc(sizeof(player_t));
    if (t->players[0] == NULL) {
        printf("Memory allocation failed for player\n");
        free(t->pixels); 
        free(t->buffer);
        exit(1);
    }

    t->players[0]->posx = IMG_SIZE;
    t->players[0]->posy = IMG_SIZE;
    t->players[0]->dx = 0;
    t->players[0]->dy = 0;

    // Initialize buffer
    memset(t->buffer, '.', t->size);
    t->draw = true;
}
