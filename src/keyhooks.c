#include "ftascii.h"

static void KeyPress(char *key, term_t *term) {
    // Escape sequence for arroy keys 
    if (key[0] == '\033' && key[1] == '[') {
        if (key[2] == ARROW_UP || key[2] == ARROW_RIGHT) {
            // handle arrow up or right
        } else if (key[2] == ARROW_DOWN || key[2] == ARROW_LEFT) {
            // handle arrow down or left
        }
    } else {    // Regular keypress for single characters
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

/* Handles keypresses as well as arrow keys */
void ft_keyhook(term_t *term) 
{
    char key[4];
    if (read(STDIN_FILENO, &key, 4) > 0) {
        KeyPress(key, term);
    } 
}

/* Resets the terminal to its original state and frees allocated memory */
void systemExit(term_t *t)
{
    free_all(t);
    system("stty echo icanon icrnl");
    system("reset");
    exit(0);
}
