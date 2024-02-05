#include "ftascii.h"

/* create a players[0] at a specific position, with a char set to draw */

static void init_player(term_t *t, char* brushes)
{
    if (!brushes[0]) {
        perror("no brushes found");
        exit(1);
    }

    t->players[0] = (player_t *)calloc(1, sizeof(player_t));
    t->players[1] = (player_t *)calloc(1, sizeof(player_t));


    *t->players[0] = (player_t){t->MAX_COL / 2, t->MAX_ROW / 2, -1, -1, 0, 0, 0, 0};
    *t->players[1] = (player_t){t->MAX_COL / 2, t->MAX_ROW / 2, 1, 1, 0, '@', 0, 0};

    t->players[0]->brushes = calloc(strlen(brushes), sizeof(char));
    t->players[1]->brushes = calloc(strlen(brushes), sizeof(char));
    if (!t->players[0]->brushes) {
        perror("brushes alloc failed");
        exit(1);
    }
    memcpy(t->players[0]->brushes, brushes, strlen(brushes));
    memcpy(t->players[1]->brushes, brushes, strlen(brushes));
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


