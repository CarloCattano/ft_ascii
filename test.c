#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

# define ENTER			0xA
# define ESCAPE			0x1B
# define ARROW_UP		0x415B0A
# define ARROW_DOWN		0x425B0A
# define ARROW_RIGHT	0x435B0A
# define ARROW_LEFT		0x445B0A

void handleKeyPress(int key) {
    switch (key) {
        case 'c':
            printf("c\n");
            break;
        case 'l':
            printf("l\n");
            break;
        case 'q':
            printf("q\n");
            break;
        case ARROW_UP:
            printf("up\n");
            break;
        case ARROW_DOWN:
            printf("down\n");
            break;
        case ARROW_RIGHT:
            printf("right\n");
            break;
        case ARROW_LEFT:
            printf("left\n");
            break;
        default:
            printf("\n");
            break;
    }
}

/* listening for keypresses */
void ft_keyhook() 
{
    int key;
    if (read(STDIN_FILENO, &key, 3) == 1) {
	    handleKeyPress(key);
    }
}

int main() {
    while (1) {
        ft_keyhook();
    }

    return 0;
}
