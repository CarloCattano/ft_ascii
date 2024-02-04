#include "ftascii.h"

// create a player at a specific position, with a char set to draw

static void init_player(term_t *t)
{
    t->player = (player_t){10, 10, 1, 1,{'*','-','+', '<', '>', '~'},'x', 0};
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

