#include "ftascii.h"

#define accelMAX 3

// for player in playerNumber do function

void apply_to_player(term_t *t, player_t *p, void (*f)(term_t *, player_t *))
{
    f(t, p);
}


void move_player_to(player_t *p, int MAX_COL, int MAX_ROW, int i, float amp)
{
    // Map i to x and y in the final 1D array

    // Calculate the y position based on the amplitude
    p->posy = (int)((1.0 - amp) * MAX_ROW);

    // Calculate the x position based on the index i
    p->posx = (MAX_COL / 4) * i;

    // Ensure that the player's position is within the screen boundaries
    if (p->posy < 0)
        p->posy = 0;
    if (p->posy >= MAX_ROW)
        p->posy = MAX_ROW - 1; // Adjusted to avoid painting out of bounds
    if (p->posx < 0)
        p->posx = 0;
    if (p->posx >= MAX_COL)
        p->posx = MAX_COL - 1; // Adjusted to avoid painting out of bounds
}

