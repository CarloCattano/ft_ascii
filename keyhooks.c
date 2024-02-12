#include "ftascii.h"

static void handlePlayerKeys(term_t *t, char key) {
    switch (key) {
        case 'w':
           break;
        case 'a':
            break;
        case 's':
            break;
        case 'd':
            break;
        case 'p' :
            for (int i = 0; i < FFT_SIZE / 2 + 1; i++) {
                t->players[i]->brush_index = (t->players[i]->brush_index + 3) % strlen(t->players[i]->brushes);
            }
            break;
        case 'o' :
            for (int i = 1; i < FFT_SIZE / 2 + 1; i++) {
                t->players[i]->toggle = !t->players[i]->toggle;
            }
            break;
        case 'u':
            for (int i = 0; i < FFT_SIZE / 2 + 1; i++) {
                t->players[i]->brush_index = rand() % strlen(t->players[i]->brushes);
                t->players[i]->curr_brush = t->players[i]->brushes[t->players[i]->brush_index];
            }
            break;
        case 'r':
            break;
        case '0':
            t->delay *= 0.5;
            t->delay < 1 ? t->delay = 1 : t->delay;
            break;
        case '9':
            t->delay *= 2;
            t->delay > 2e6 ? t->delay = 2e6 : t->delay;
            break;
        case '1':
            t->sens += 0.1;
            break;
        case '2':
            t->sens -= 0.1;
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

