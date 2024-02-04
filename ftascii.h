#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/select.h>
#include <poll.h>

#define   CLEAR     "\033[H"
#define   NOMOUSE   "\033[?25l"

#define   RED       "\033[31m"
#define   GREEN     "\033[32m"
#define   YELLOW    "\033[33m"
#define   BLUE      "\033[34m"
#define   WHITE     "\033[37m"
#define   BLACK     "\033[30m"
#define   RST       "\033[0m"

typedef struct player_s
{
    int             posx;
    int             posy;
    int             dx;
    int             dy;
    char            brushes[6];
    char            curr_brush;
    int             brush_index;
}   player_t;

typedef struct term_s
{
    int             MAX_COL;
    int             MAX_ROW;
    int             size;
    char*           buffer;
	char* 		    buffer_copy;
	unsigned int    frame;
    int 		    clear;

    player_t 	    player;

}                   term_t;

void                init_term(term_t *t);

void                draw(term_t *t);
void                move_player(term_t *t);

// hooks            
void                handleKeyPress(char key, term_t *t);
void                handlectrl_c(int sig);
void                init_keyhook(term_t *t, fd_set *set, struct timeval *timeout);

int                 compare_val_in_buffers(term_t *t, int i);
void                add_randomness_to_movement(term_t *t);

void                copy_last_buffer(term_t *t);
void                free_buffer(term_t *t);
