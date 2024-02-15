#include "ftascii.h"
#include <locale.h>
#include <time.h>

term_t *term_pointer;

void handlectrl_c(int sig) {
    (void)sig;
    systemExit(term_pointer);
}

static void initializeTerm(term_t *term)
{
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
    term_pointer = term;
 
    init_term(term);
    signal(SIGINT, handlectrl_c);
}

int ft_ascii()
{
    term_t *term = malloc(sizeof(term_t));

    if (term == NULL) {
        perror("Failed to allocate memory for term");
        exit(EXIT_FAILURE);
    }

    initializeTerm(term);

    while(1) 
    {
        ft_keyhook(term);
        draw(term);
        usleep(term->delay);
    }
    systemExit(term);
}
