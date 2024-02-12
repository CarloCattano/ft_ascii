#include "ftascii.h"

static int init(term_t *t) 
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    
    /* disable echo and buffering */
    system("stty -echo -icanon -icrnl time 0 min 0"); 

    signal(SIGINT, handlectrl_c);

    write(1, NOMOUSE, 6);   // hide cursor
    usleep(1000);
    term_t term = {w.ws_col, w.ws_row, w.ws_col * w.ws_row,
                             NULL, NULL, NULL, 1, 1, 0};
    init_term(&term);
    return 0;
}

void ft_ascii(float* fft_values) {
   
    term_t term;
    init(&term) ? exit(1) : 0;

    while(1) 
    {
        ft_keyhook(&term);
        term.clear ? memset(term.buffer, ' ', term.size) : 0;
        if (fft_values[0] > 0.2f * term.sens) {
            move_player(&term, term.players[0]);
            move_player(&term, term.players[1]);
            move_player(&term, term.players[2]);
            move_player(&term, term.players[3]);
        }
        fft_values[0] < 0.4f ? write(1, RST, 4) : 0;
        fft_values[3] > 1.7f ? memset(term.buffer, ' ', term.size) : 0;
        fft_values[4] > 0.2f ? write(1, RED, 5) : 0;
        fft_values[0] > 2.5f ? write(1, GREEN, 5) : 0;
        
        draw(&term, 0.9, fft_values, &term.sens);
        copy_last_buffer(&term);
        usleep(term.delay);
    }
}
