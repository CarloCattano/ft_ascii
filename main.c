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

    /* setlocale(LC_ALL, "en_US.UTF-8"); */

    /* disable echo and buffering */
    system("stty -echo -icanon -icrnl time 0 min 0"); 

    signal(SIGINT, handlectrl_c);

    write(1, NOMOUSE, 6);   // hide cursor
    
    term_t term = {w.ws_col, w.ws_row, w.ws_col * w.ws_row,
                             NULL, NULL, NULL, 1, 1, 0, {0}};
    init_term(&term);

    assign_pix_buff(term.buffer, term.pixels, term.size);
    while(1) 
    {
        ft_keyhook(&term);
        draw(&term);
        usleep(term.delay);
    }
    
    /* free_all(&term); */
    systemExit();
    return 0;
}
