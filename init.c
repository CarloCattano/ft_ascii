#include "ftascii.h"

static void init_player(term_t *t, char* brushes)
{
    if (!brushes[0]) {
        perror("no brushes found");
        exit(1);
    }

    for (int i = 0; i < FFT_SIZE / 2 + 1; i++) {
        t->players[i] = (player_t *)calloc(1, sizeof(player_t));
        
        if (!t->players[i]) {
            perror("player alloc failed");
            exit(1);
        }
        t->players[i]->brushes = calloc(strlen(brushes), sizeof(char));
        if (!t->players[i]->brushes) {
            perror("brushes alloc failed");
            exit(1);
        }
        t->players[i]->brush_index = 0;
        t->players[i]->toggle = 1;
        t->players[i]->curr_brush = brushes[0];
        strcpy(t->players[i]->brushes, brushes);
    }
  }

/* initialize main term struct */
void init_term(term_t *t)
{
    t->frame = 1;
    t->clear = 0;
    t->delay = 1e1;
    t->sens = 1.0f;
    t->buffer = (char *)calloc(t->size, sizeof(char));
    t->buffer_copy = (char *)calloc(t->size, sizeof(char));

    if (!t->buffer || !t->buffer_copy) {
        perror("terminal allocation failed");
        exit(1);
    }
    memset(t->buffer, ' ', t->size);
    init_player(t, "|\\/|<>., &^'`[{_\"~*%");
}
