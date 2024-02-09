#include "ftascii.h"
#include <locale.h>

int main(int ac, char *av[])
{
    if (ac > 1) {
        printf("Usage: %s\n", av[0]);
        return 1;
    }
    /* keyhook variables */
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    setlocale(LC_ALL, "en_US.UTF-8");

    /* disable echo and buffering */
    system("stty -echo -icanon -icrnl time 0 min 0"); 

    signal(SIGINT, handlectrl_c);

    write(1, NOMOUSE, 6);   // hide cursor
    
    term_t term = {w.ws_col, w.ws_row, w.ws_col * w.ws_row,
                             NULL, NULL, NULL, 1, 1, 0, {0}};
    init_term(&term);

    while(1) 
    {
        ft_keyhook(&term);
        /* term.clear ? memset(term.buffer, ' ', term.size) : 0; */
        /* move_player(&term, term.players[0]); */
        /* move_player(&term, term.players[1]); */
        /* move_player(&term, term.players[2]); */
        /* move_player(&term, term.players[3]); */
        draw(&term);
        /* copy_last_buffer(&term); */
        usleep(term.delay);
        memset(term.buffer, ' ', term.size);  
    }
    
    /* free_all(&term); */
    systemExit();
    return 0;
}
