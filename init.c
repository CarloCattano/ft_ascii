#include "ftascii.h"

// create a player at a specific position, with a char set to draw

static void init_player(term_t *t)
{
    char* brushes = "<>()/ \\+-_|~";
    t->player = (player_t){10, 10, 1, 1,NULL,'x', 0};
    
    t->player.brushes = calloc(strlen(brushes), sizeof(char));
    if (!t->player.brushes) {
        perror("calloc");
        exit(1);
    }
    memcpy(t->player.brushes, brushes, strlen(brushes));
}

// initialize main term struct
void init_term(term_t *t)
{
    t->frame = 1;
    t->clear = 1;
    
    t->buffer = (char *)calloc(t->size, sizeof(char));
    t->buffer_copy = (char *)calloc(t->size, sizeof(char));
    
    if (!t->buffer || !t->buffer_copy) {
        perror("calloc");
        exit(1);
    }
    memset(t->buffer, ' ', t->size);
    init_player(t);
}

