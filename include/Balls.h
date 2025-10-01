#pragma once
#include "ftascii.h"

struct ball {
    int x;
    int y;

    int dx;
    int dy;

    float vel;

    term_t *term;
};

// void init_ball(struct ball *ball, term_t *term);
void move_ball(struct ball *ball);
void draw_ball(term_t *term, struct ball *ball);
