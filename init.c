#include "ftascii.h"

/* create a player at a specific position, with a char set to draw */

static void init_player(term_t *t, char* brushes)
{
    if (!brushes[0]) {
        perror("no brushes found");
        exit(1);
    }

    int x = t->MAX_COL / 2;
    int y = t->MAX_ROW / 2;

    t->player = (player_t){x, y, 1, 1,NULL,brushes[0], 0};
    t->player.brushes = calloc(strlen(brushes), sizeof(char));

    if (!t->player.brushes) {
        perror("brushes alloc failed");
        exit(1);
    }
    memcpy(t->player.brushes, brushes, strlen(brushes));
}


/* initialize main term struct */
void init_term(term_t *t)
{
    t->frame = 1;
    t->clear = 0;
    t->delay = 2e2;

    t->buffer = (char *)calloc(t->size, sizeof(char));
    t->buffer_copy = (char *)calloc(t->size, sizeof(char));

    if (!t->buffer || !t->buffer_copy) {
        perror("terminal allocation failed");
        exit(1);
    }
    memset(t->buffer, ' ', t->size);
    init_player(t, "<>()`'\"\\|/-_~.,:;^! ");
}

