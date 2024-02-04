#include "ftascii.h"

// start listening for keypresses
void init_keyhook(term_t *t, fd_set *read_fds, struct timeval *timeout)
{
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

static void systemExit()
{
    system("reset");
    usleep(1e5); // wait for reset to finish 
    exit(1);
}

void handlectrl_c(int sig)
{
    (void)sig;
    systemExit();
}

void handleKeyPress(char key, term_t *t) {
    switch (key) {
        case 'w':
            t->player.dy += 0;
            break;
        case 'a':
            t->player.dx += 0;
            break;
        case 's':
            t->player.dy += 2;
            break;
        case 'd':
            t->player.dx += 2;
            break;
        case 'c':
            t->clear = !t->clear;
            break;
        case 'p' :
            t->player.brush_index = (t->player.brush_index + 1) % strlen(t->player.brushes);
            break;

        case 'q':
            free_buffer(t);
            systemExit();
    }
}
