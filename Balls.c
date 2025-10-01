#include "Balls.h"

void move_ball(struct ball *ball)
{
    ball->x += ball->dx * ball->vel;
    // need pointer for term 
    if (ball->x >= ball->term->MAX_COL - 1 || ball->x < 2) {
      ball->dx = -ball->dx;
    }
    ball->y = ball->y;

}

// void init_ball(struct ball *ball, term_t *term)
// {
//     ball->x = 4;
//     ball->y = 4;
//
//     ball->dx = 1;
//     ball->dy = 0;
//
//     ball->vel = 1;
//     
//     ball->term = term;
// }

void draw_ball(term_t *term, struct ball *ball)
{
    map_pix(term, ball->x, ball->y, RED, "⬤");
}

// #endif
