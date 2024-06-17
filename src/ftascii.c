#include "ftascii.h"

void init(term_t *t)
{
    /* keyhook variables */
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    /* disable echo and buffering */
    system("stty -echo -icanon -icrnl time 0 min 0"); 

    signal(SIGINT, handlectrl_c);

    write(1, NOMOUSE, 6);   // hide cursor
    
    *t = (term_t){w.ws_col, w.ws_row, w.ws_col * w.ws_row,
                  NULL, NULL, 1, 1, 0, {0}};

    init_term(t);
}

int ft_ascii(void)
{
    term_t term;
    init(&term);

    while(1) 
    {
        ft_keyhook(&term);
        term.clear ? memset(term.buffer, ' ', term.size) : 0;
        move_player(&term, term.players[0]);
        move_player(&term, term.players[1]);
        move_player(&term, term.players[2]);
        move_player(&term, term.players[3]);
        draw(&term);
        copy_last_buffer(&term);
        usleep(term.delay);
    }
    
    free_all(&term);
    systemExit();
    return 0;
}
