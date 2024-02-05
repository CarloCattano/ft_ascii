#include "ftascii.h"

/* create a players[0] at a specific position, with a char set to draw */

static void init_player(term_t *t, char* brushes)
{
    if (!brushes[0]) {
        perror("no brushes found");
        exit(1);
    }
    for (int i = 0; i < 4; i++) {
        t->players[i] = (player_t *)calloc(1, sizeof(player_t));
        
        if (!t->players[i]) {
            perror("player alloc failed");
            exit(1);
        }
        t->players[i]->brushes = calloc(strlen(brushes), sizeof(char));
    }
    *t->players[0] = (player_t){t->MAX_COL / 2, t->MAX_ROW / 2, -1, -1, brushes, '$',  0, 0};
    *t->players[1] = (player_t){t->MAX_COL / 2, t->MAX_ROW / 2,  1,  1, brushes, '@',  0, 0};
    *t->players[2] = (player_t){t->MAX_COL / 2, t->MAX_ROW / 2,  1, -1, brushes, '*',  0, 0};
    *t->players[3] = (player_t){t->MAX_COL / 2, t->MAX_ROW / 2, -1,  1, brushes, '#',  0, 0};
}

/* initialize main term struct */
void init_term(term_t *t)
{
    t->frame = 1;
    t->clear = 0;
    t->delay = 1e1;

    t->buffer = (char *)calloc(t->size, sizeof(char));
    t->buffer_copy = (char *)calloc(t->size, sizeof(char));

    if (!t->buffer || !t->buffer_copy) {
        perror("terminal allocation failed");
        exit(1);
    }
    memset(t->buffer, ' ', t->size);
    init_player(t, "|\\/|<~.");
}

//"_-<>^v\"'`,/\\|");
            //"_-{}~.,:;^'\" ");


