#include "ftascii.h"

#define accelMAX 4

void move_player(term_t *t, float speedx, float speedy)
{
    player_t *p = t->players[0];
    p->posx += speedx;
    p->posy += speedy;

    int half_img_size = IMG_SIZE / 2;
    int max_col_minus_half_img = t->MAX_COL - half_img_size;
    int max_row_minus_half_img = t->MAX_ROW - half_img_size ;

    p->posx += p->dx * speedx;
    p->posy += p->dy * speedy;

    // Check horizontal boundaries
    if (p->posx  < half_img_size || p->posx >= max_col_minus_half_img) {
        p->dx = -p->dx;
        if (p->posx < half_img_size) {
            p->posx = half_img_size;
        } else {
            p->posx = max_col_minus_half_img;
        }
    }

    // Check vertical boundaries
    if (p->posy <= half_img_size || p->posy >= max_row_minus_half_img) {
        p->dy = -p->dy;
        if (p->posy <= half_img_size) {
            p->posy = half_img_size;
        } else {
            p->posy = max_row_minus_half_img;
        }
    }
    // Clamp acceleration
    p->dx = (p->dx > accelMAX) ? accelMAX : (p->dx < -accelMAX ? -accelMAX : p->dx);
    p->dy = (p->dy > accelMAX) ? accelMAX : (p->dy < -accelMAX ? -accelMAX : p->dy);
}
