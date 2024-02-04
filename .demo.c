#include <linux/limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/select.h>
#include <poll.h>

#define CLEAR "\033[H"
#define NOMOUSE "\033[?25l"

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BLACK "\033[30m"
#define RST "\033[0m"


typedef struct player_s
{
    int              posx;
    int              posy;
    int              dx;
    int              dy;
    char             brushes[6];
    char             curr_brush;

}   player_t;

typedef struct term_s
{
    int             MAX_COL;
    int             MAX_ROW;
    int             size;
    char*           buffer;
	char* 		 	buffer_copy;
	unsigned int    frame;
    int 			clear;

    player_t 		player;

}           term_t;

void handleKeyPress(char key, term_t *t) {
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
        case 'c':
            t->clear = !t->clear;
            break;
        case 'p' :
            t->player.curr_brush = t->player.brushes[rand() % 6];
            break;

        case 'q':
            printf("\033[?25h\n");
            exit(0);
    }
}

void handle_ctrl_c(int sig)
{
    printf("\n---------\n %d \n---------\n", sig);
    printf("\033[?25h\n");
	exit(0);
}

void store_initial_buffer(term_t *t)
{
	t->buffer_copy = memcpy(t->buffer_copy, t->buffer, t->size);
}

int compare_val_in_buffers(term_t *t, int i)
{
	if (t->buffer[i] != t->buffer_copy[i])
		return 0;
	return 1;
}

void draw_player(term_t *t)
{
	int offset = (t->player.posy * t->MAX_COL) + t->player.posx;
    if (compare_val_in_buffers(t, offset))
    {
        t->buffer[offset] = t->player.curr_brush;
    }
}

void add_randomness_to_movement(term_t *t)
{
    t->player.dx = (rand() % 3) - 1;
    t->player.dy = (rand() % 3) - 1;
}

void move_player(term_t *t)
{
    player_t *p = &t->player;

    int new_posx = p->posx + p->dx;
    int new_posy = p->posy + p->dy;

    if (new_posx <= 0 || new_posx >= t->MAX_COL) {
        p->dx = -p->dx;
    } else {
        p->posx = new_posx;
    }

    if (new_posy <= 0 || new_posy >= t->MAX_ROW) {
        p->dy = -p->dy;
    } else {
        p->posy = new_posy;
    }
    p->dx > 3 ? p->dx = 3 : 0;
    p->dy > 3 ? p->dy = 3 : 0;
}

void draw_inside(term_t *t, int i)
{
      if (i > t->MAX_COL && i < (t->size - t->MAX_COL) && i % t->MAX_COL != 0 && i % t->MAX_COL != t->MAX_COL - 1)
          draw_player(t);
}

void draw(term_t *t)
{
    for (int i = 0; i < t->size; i++)
    {
        if (i < t->MAX_COL || i > (t->size - t->MAX_COL) || i % t->MAX_COL == 0 
                || i % t->MAX_COL == t->MAX_COL - 1) {
            t->buffer[i] !=  '|' ? t->buffer[i] = '|' : 0;
        }
        else
            draw_inside(t, i);
    }

	t->frame++;
	
    if (t->frame > 16384)
		t->frame = 1;
    
    write(1, t->buffer, t->size);
}

int main(int ac, char *av[])
{
    if (ac > 1) {
        printf("Usage: %s\n", av[0]);
        return 1;
    }

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    unsigned int MAX_COL = w.ws_col;
    unsigned int MAX_ROW = w.ws_row;
    signal(SIGINT, handle_ctrl_c);
    
    unsigned int delay = 5e2;
    
    write(1, NOMOUSE, 6);
	system("stty -echo -icanon -icrnl time 0 min 0"); // disable input buffering
       
    fd_set read_fds;
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = delay; // 5 
    
    term_t term = {MAX_COL, MAX_ROW, MAX_COL * MAX_ROW, NULL, NULL, 0, 0, {0}};                     	
    term.buffer = (char *)calloc(term.size, sizeof(char));
	term.buffer_copy = (char *)calloc(term.size, sizeof(char));
    term.player = (player_t){10, 10, 1, 1,{'*','-','+', '<', '>', '~'},'x'};
    memset(term.buffer, ' ', term.size);

    if (term.buffer == NULL)
        return 1;

    while(1) {
    
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
 
        int result = select(STDIN_FILENO + 1, &read_fds, NULL, NULL, &timeout);
        if (result > 0 && FD_ISSET(STDIN_FILENO, &read_fds)) {
            char key;
            if (read(STDIN_FILENO, &key, 1) == 1) {
                handleKeyPress(key, &term);
            }
        }
        store_initial_buffer(&term);
        term.clear ? memset(term.buffer, ' ', term.size) : 0;
        move_player(&term);
        /* if (term.frame % 200 == 0) */
        draw(&term);
        usleep(delay);
    }
	
    FD_CLR(STDIN_FILENO, &read_fds);
    free(term.buffer);
    free(term.buffer_copy);
    return 0;
}
