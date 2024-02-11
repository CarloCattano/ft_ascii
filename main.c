#include "ftascii.h"
#include <locale.h>
#include <time.h>

void initializeTerm(term_t *term) {
    // Get terminal size
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    srand(time(NULL));

    setlocale(LC_ALL, "en_US.UTF-8");
    system("stty -echo -icanon -icrnl time 0 min 0");

    // Hide cursor
    write(1, NOMOUSE, 6);
    // Clear screen
    write(1, "\033[2J", 4);

    // Initialize term structure
    *term = (term_t){w.ws_col, w.ws_row, w.ws_col * w.ws_row, NULL, NULL, NULL, 1, 1, 0, {0}};
    
    init_term(term);
}

int main(int ac, char *av[])
{
    (void)av;
    if (ac > 1) {
        printf("Usage: %s\n", av[0]);
        return 1;
    }
 
    term_t term;
    initializeTerm(&term);

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
