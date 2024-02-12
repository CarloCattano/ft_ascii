#include "ftascii.h"

void init(term_t *t)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    
    /* disable echo and buffering */
    system("stty -echo -icanon -icrnl time 0 min 0"); 

    signal(SIGINT, handlectrl_c);

    write(1, NOMOUSE, 6);   // hide cursor
    usleep(1000);
    *t = (term_t){w.ws_col, w.ws_row, w.ws_col * w.ws_row,
                             NULL, NULL, NULL, 1, 1, 0};
    init_term(t);

}

void ft_ascii(float* fft_values) 
{
    term_t term;
    init(&term);

    while(1) 
    {
        ft_keyhook(&term);
        term.clear ? memset(term.buffer, ' ', term.size) : 0;
        draw(&term, fft_values, &term.sens);
        copy_last_buffer(&term);
        usleep(term.delay);
        // flush the buffer
        write(1, "\033[H", 3);
    }
}
