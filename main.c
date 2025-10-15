#include "ftascii.h"
#include <locale.h>
#include <signal.h>
#include <sys/stat.h>
#include <time.h>

static term_t *term_pointer;

static void handlectrl_c(int sig) {
    (void)sig;
    systemExit(term_pointer);
}

static void setup_signal_handlers() {
    signal(SIGINT, handlectrl_c);
    signal(SIGTERM, handlectrl_c);
}

static void initializeTerm(term_t *term) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    setlocale(LC_ALL, "en_US.UTF-8");
    system("stty -echo -icanon -icrnl time 0 min 0");

    write(1, NOMOUSE, 6); // Hide cursor
    write(1, CLEAR, 4);   // Clear screen
    write(1, CURSOR, 6);

    *term = (term_t){w.ws_col,
                     w.ws_row,
                     w.ws_col * w.ws_row,
                     NULL,
                     w.ws_col * w.ws_row,
                     NULL,
                     1,
                     1,
                     50000,
                     0};

    term_pointer = term;

    init_term(term);
    setup_signal_handlers();
}

struct ball {
    int x;
    int y;

    int dx;
    int dy;

    int vel;
};

struct ball ball;

void init_ball(struct ball *ball) {
    ball->x = 4;
    ball->y = 4;

    ball->dx = 1;
    ball->dy = 1;

    ball->vel = 1;
}

void move_ball(struct ball *ball) {
    ball->x += ball->dx;
    ball->y += ball->dy;

    if (ball->x >= term_pointer->MAX_COL - 1 || ball->x <= 1)
        ball->dx = -ball->dx;

    if (ball->y >= term_pointer->MAX_ROW - 1 || ball->y <= 1)
        ball->dy = -ball->dy;
}

static void draw_ball(term_t *term, struct ball *ball) {
    map_pix(term, ball->x, ball->y, RED, "⬤");
}

static void draw_callback(term_t *term) { draw_ball(term, &ball); }

int main() {
    term_t *term = malloc(sizeof(term_t));

    if (term == NULL) {
        perror("Failed to allocate memory for term");
        exit(EXIT_FAILURE);
    }

    initializeTerm(term);
    init_ball(&ball);

    while (term->draw) {
        ft_keyhook(term);
        move_ball(&ball);
        draw(term, &draw_callback);
        usleep(term->delay);
    }

    systemExit(term);
    return 0;
}
