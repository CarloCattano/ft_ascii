#include "ftascii.h"

static void handlePlayerKeys(term_t *t, char key) {
    switch (key) {
        case 'w':
            /* t->players[0]->dy += -1; */
            /* t->players[1]->dy += 1; */
            /* t->players[2]->dy += -1; */
            /* t->players[3]->dy += 1; */
            break;
        case 'a':
            /* t->players[0]->dx += -1; */
            /* t->players[1]->dx += 1; */
            /* t->players[2]->dx += 1; */
            /* t->players[3]->dx += -1; */
            break;
        case 's':
            /* t->players[0]->dy += 1; */
            /* t->players[1]->dy += -1; */
            /* t->players[2]->dy += 1; */
            /* t->players[3]->dy += -1; */
            break;
        case 'd':
            /* t->players[0]->dx += 1; */
            /* t->players[1]->dx += -1; */
            /* t->players[2]->dx += -1; */
            /* t->players[3]->dx += 1; */
            break;
        case 'p' :
            /* for (int i = 0; i < 4; i++) { */
            /*     t->players[i]->brush_index = (t->players[i]->brush_index + 2) % strlen(t->players[i]->brushes); */
            /* } */
            break;
        case 'o' :
            /* for (int i = 1; i < 4; i++) { */
            /*     t->players[i]->toggle = t->players[i]->toggle; */
            /* } */
            break;
        case 'r':
            /* for (int i = 0; i < 4; i++) { */
            /*     t->players[i]->posx = t->MAX_COL / 2; */
            /*     t->players[i]->posy = t->MAX_ROW / 2; */
            /* } */
            break;
        case '0':
            t->delay *= 0.5;
            t->delay < 1 ? t->delay = 1 : t->delay;
            break;
        case '9':
            t->delay *= 2;
            t->delay > 3e4 ? t->delay = 3e4 : t->delay;
            break;
    }
}

void handleKeyPress(char key, term_t *t) {
    handlePlayerKeys(t, key);
    switch (key) {
        case 'c':
            t->clear = !t->clear;
            break;
        case 'l':
            write(1, GREEN, 5);
            break;
        case 'q':
            free_all(t);
            systemExit();
    }
}
/* start listening for keypresses */

void ft_keyhook(term_t *term) {
    char key;
    if (read(STDIN_FILENO, &key, 1) == 1) {
        handleKeyPress(key, term);
    }
}

void systemExit() {
    system("reset");
    exit(1);
}

void handlectrl_c(int sig) {
    (void)sig;
    systemExit();
}
