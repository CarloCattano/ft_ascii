// test key hooks

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

# define ARROW_UP		  0x41
# define ARROW_DOWN		0x42
# define ARROW_RIGHT	0x43
# define ARROW_LEFT		0x44

void handleKeyPress(char *key) {

    if (key[0] == '\033' && key[1] == '[') {
        switch (key[2]) {
            case 0x41:
                printf("up\n");
                break;
            case 0x42:
                printf("down\n");
                break;
            case 0x43:
                printf("right\n");
                break;
            case 0x44:
                printf("left\n");
                break;
        }
    } else {
        switch (key[0]) {
            case 'w':
                printf("w\n");
                break;
            case 's':
                printf("s\n");
                break;
            case 'd':
                printf("r\n");
                break;
            case 'a':
                printf("a\n");
                break;
            case 'q':
                printf("quit\n");
                break;
        }
    
    }
}
/* listening for keypresses */
void ft_keyhook() 
{
    char key[4];

    if (read(STDIN_FILENO, &key, 4) > 0) {
        handleKeyPress(key);
    }
}

int main()
{
    printf("test key hooks\n");

    while(1)
    {
        ft_keyhook();
        usleep(10000);       
    }

    return 0;
}
