#include "ftascii.h"

void apply_to_player(term_t *t, player_t *p, void (*f)(term_t *, player_t *))
{
    f(t, p);
}

void move_player_to(player_t *p, int MAX_COL, int MAX_ROW, int i, float amp)
{
    // amp comes in dB, so we map -60 to 0 to 0 to 1 for our y position
    p->posy = (int)((1.0 - (amp / -60.0)) * MAX_ROW);
    
    // Calculate the x position based on the index i
    // we split the screen into FFT_SIZE columns
    //
    p->posx = (int)((float)i * (float)MAX_COL / (float)FFT_SIZE);
    p->posx += (MAX_COL / FFT_SIZE) / 2; // center the player in the column
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

