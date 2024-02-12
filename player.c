#include "ftascii.h"

void apply_to_player(term_t *t, player_t *p, void (*f)(term_t *, player_t *))
{
    f(t, p);
}

void move_player_to(player_t *p, int MAX_COL, int MAX_ROW, int i, float amp)
{
    p->posy = (int)((1 - amp / 30.0) * MAX_ROW);
    
    p->posx = i * 2;
    p->posx += 2;
}

