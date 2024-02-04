#include "ftascii.h"

void move_player(term_t *t)
{
    player_t *p = &t->player;

    int new_posx = p->posx + p->dx;
    int new_posy = p->posy + p->dy;

    if (new_posx <= 0 || new_posx >= t->MAX_COL) {
        p->dx = -p->dx;
    } else {
        p->posx = new_posx;
    }

    if (new_posy <= 0 || new_posy >= t->MAX_ROW) {
        p->dy = -p->dy;
    } else {
        p->posy = new_posy;
    }
    p->dx > 3 ? p->dx = 3 : 0;
    p->dy > 3 ? p->dy = 3 : 0;
}
