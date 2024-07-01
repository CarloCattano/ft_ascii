#include "ftascii.h"
#include <signal.h>
#include <locale.h>
#include <time.h>

static term_t *term_pointer;

static void handlectrl_c(int sig) {
    (void)sig;
    systemExit(term_pointer);
}

static void initializeTerm(term_t *term)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    srand(time(NULL));
    setlocale(LC_ALL, "en_US.UTF-8");
    system("stty -echo -icanon -icrnl time 0 min 0");

    write(1, NOMOUSE, 6);  // Hide cursor
    write(1, CLEAR, 4); // Clear screen
    write(1, CURSOR, 6); 

    *term = (term_t){w.ws_col, w.ws_row, w.ws_col * w.ws_row, NULL, NULL, NULL, 1, 1, 100,0,{0}};
    term_pointer = term;

    init_term(term);
    signal(SIGINT, handlectrl_c);
}

void draw_callback(term_t *term)
{

        /*map_pix(term_t *t, int x, int y, char *color, char *uni);*/

    // create a tree shape with map_pix
    // trunk
    for (int i = 10; i < 42; i++) {
        map_pix(term, term->MAX_COL / 2, term->MAX_ROW - i, RED, "▓");
        // branches
        if (i % 2 == 0) {
            map_pix(term, term->MAX_COL / 2 - 1, term->MAX_ROW - i, GREEN, "▓");
            map_pix(term, term->MAX_COL / 2 + 1, term->MAX_ROW - i, GREEN, "▓");
        //leafs
            if (i % 4 == 0) {
                map_pix(term, term->MAX_COL / 2 - 2, term->MAX_ROW - i, GREEN, "▓");
                map_pix(term, term->MAX_COL / 2 + 2, term->MAX_ROW - i, GREEN, "▓");
            }
        }
    }

}

int main()
{
    term_t *term = malloc(sizeof(term_t));

    if (term == NULL) {
        perror("Failed to allocate memory for term");
        exit(EXIT_FAILURE);
    }

    initializeTerm(term);

    while (term->draw) {
        ft_keyhook(term);
        draw(term, &draw_callback);
        usleep(term->delay);
    }
    systemExit(term);
    return 0;
}

