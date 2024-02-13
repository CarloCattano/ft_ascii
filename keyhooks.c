#include "ftascii.h"

static void handlePlayerKeys(term_t *t, char key) {
    switch (key) {
        case 'w':
            t->players[0]->dy -= 1;
            break;
        case 'a':
            t->players[0]->dx -= 1;
            break;
        case 's':
            t->players[0]->dy += 1;
            break;
        case 'd':
            t->players[0]->dx += 1;
            break;
        case 'p' :
           break;
        case 'o' :
           break;
        case 'r':
           break;
        case '0':
            t->delay *= 0.5;
            t->delay < 2 ? t->delay = 1 : t->delay;
            break;
        case '9':
            t->delay *= 2;
            t->delay > 3e5 ? t->delay = 3e5 : t->delay;
            break;

		case '1':
			t->sens += 0.1f;
			break;
		case '2':
			t->sens -= 0.1f;
			break;
		default:
			break;
    }
}

void handleKeyPress(char key, term_t *t) {
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
            break;
        default:
            handlePlayerKeys(t, key);
            break;
    }
}

void ft_keyhook(term_t *term) {
    char key;
    if (term->frame % 4 == 0) {
        if (read(STDIN_FILENO, &key, 1) == 1) {
            handleKeyPress(key, term);
        }
    }
}

void systemExit() {
    system("reset");
	printf("Exited gracefully\n");
    exit(1);
}

