#include "ftascii.h"

#define accelMAX 2

void move_player(term_t *t)
{
    player_t *p = t->players[0];

    p->posx += p->dx;
    p->posy += p->dy;

    if (p->posx <= 0 || p->posx >= t->MAX_COL) {
        p->dx = -p->dx;
    }

    if (p->posy <= 0 || p->posy >= t->MAX_ROW) {
        p->dy = -p->dy;
    }

    p->dx > accelMAX ? p->dx = accelMAX : p->dx;
    p->dy > accelMAX ? p->dy = accelMAX : p->dy;
    p->dx < -accelMAX ? p->dx = -accelMAX : p->dx;
    p->dy < -accelMAX ? p->dy = -accelMAX : p->dy;
}

