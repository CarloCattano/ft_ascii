#include "ftascii.h"

int main(int ac, char *av[])
{
    if (ac > 1) {
        printf("Usage: %s\n", av[0]);
        return 1;
    }

    // keyhook variables
    unsigned int delay = 5e2;
    struct timeval timeout;
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	
    // disable echo and buffering
    system("stty -echo -icanon -icrnl time 0 min 0"); 
    
    signal(SIGINT, handlectrl_c);

    write(1, NOMOUSE, 6);   // hide cursor

    // keyhook variables
    fd_set read_fds;
    timeout.tv_sec = 0;
    timeout.tv_usec = delay;   
 

    term_t term = {w.ws_col, w.ws_row, w.ws_col * w.ws_row,
                                    NULL, NULL, 1, 1, {0}};
    init_term(&term);
    
    while(1) 
    {
        init_keyhook(&term, &read_fds, &timeout);
        copy_last_buffer(&term);
        term.clear ? memset(term.buffer, ' ', term.size) : 0;
        move_player(&term);
        draw(&term);
        usleep(delay);
    }

    FD_CLR(STDIN_FILENO, &read_fds);
    free(term.buffer);
    free(term.buffer_copy);
    return 0;
}
