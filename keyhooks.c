#include "ftascii.h"

/* start listening for keypresses */
void ft_keyhook(term_t *t, fd_set *read_fds, struct timeval *timeout) {
        FD_ZERO(read_fds);
        FD_SET(STDIN_FILENO, read_fds);

        int result = select(STDIN_FILENO + 1, read_fds, NULL, NULL, timeout);

        if (result > 0 && FD_ISSET(STDIN_FILENO, read_fds)) {
            char key;
            if (read(STDIN_FILENO, &key, 1) == 1) {
                handleKeyPress(key, t);
            }
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

static void handlePlayerKeys(term_t *t, char key) {
    switch (key) {
        case 'w':
            t->player.dy += -1;
            break;
        case 'a':
            t->player.dx += -1;
            break;
        case 's':
            t->player.dy += 1;
            break;
        case 'd':
            t->player.dx += 1;
            break;
        case 'p' :
            t->player.brush_index = (t->player.brush_index + 2) % strlen(t->player.brushes);
            break;
        case 'o' :
            t->player.toggle = !t->player.toggle;
            break;
        case '0':
            t->delay *= 0.5;
            t->delay > 1e5 ? t->delay = 1e5 : t->delay;
            break;
        case '9':
            t->delay *= 2;
            t->delay < 1e2 ? t->delay = 1e2 : t->delay;
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
            free_buffer(t);
            systemExit();
    }
}
