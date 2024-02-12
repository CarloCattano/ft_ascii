#include "ftascii.h"

void apply_to_player(term_t *t, player_t *p, void (*f)(term_t *, player_t *))
{
    f(t, p);
}


void move_player_to(player_t *p, int MAX_COL, int MAX_ROW, int i, float amp)
{

    p->posy = (int)((1 - amp / 40.0) * MAX_ROW);
    
    // Calculate the total number of bins (including the wall at x=0)
    int total_bins = FFT_SIZE / 2 + 2; // Add 2 for the wall and the actual bins

    // Adjust the range to spread more to reach the end and account for the wall
    p->posx = i * (MAX_COL - 1) / total_bins;
    /* p->posx += 1; // Add 1 to account for the wall */
}

void compute_eq_bars(term_t *t, float *fft_values, float sens)
{
    int num_bins = FFT_SIZE / 2 + 1;
    if (num_bins > t->MAX_COL)
    {
        printf("Error: FFT_SIZE / 2 + 1 > MAX_COL %d\n", t->MAX_COL);
        exit(1);    
    }

    for (int i = 0; i < num_bins; i++)
    {
        move_player_to(t->players[i], t->MAX_COL, t->MAX_ROW, i + 1, fft_values[i] * sens);
    }
}
