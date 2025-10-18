#include "ftascii.h"

static void KeyPress(char *key, term_t *term) {
    // Escape sequence parsing for arrow keys
    if (key[0] == '\033' && key[1] == '[') {
        switch (key[2]) {
        case ARROW_UP:
            //
            break;
        case ARROW_DOWN:
            //
            break;
        case ARROW_RIGHT:
            //
            break;
        case ARROW_LEFT:
            //
            break;
        }
    } else { // Regular single character
        switch (key[0]) {
        case 'w':
            // handle w
            break;
        case 's':
            // handle s
            break;
        case 'p':
            systemExit(term);
            break;
        default:
            break;
        }
    }
}

/* Handles keypresses and arrow keys */
void ft_keyhook(term_t *term) {
    char key[4];
    if (read(STDIN_FILENO, &key, 4) > 0) {
        KeyPress(key, term);
    }
}

/* Resets the terminal to its original state and frees allocated memory */
void systemExit(term_t *t) {
    free_all(t);
    system("stty echo icanon icrnl");
    system("reset");
    exit(0);
}
