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
    char*           brushes;
    char            curr_brush;
    int             brush_index;
    int             toggle;
}                   player_t;

typedef struct term_s
{
    int             MAX_COL;
    int             MAX_ROW;
    int             size;
    char*           buffer;
	char* 		    buffer_copy;
	unsigned int    frame;
    int 		    clear;
    unsigned int    delay;
    player_t* 	    players[4];

}                   term_t;


int                 ft_ascii(void);
void                init_term(term_t *t);

/*      hooks       */
void                handleKeyPress(char key, term_t *t);
void                handlectrl_c(int sig);
void                ft_keyhook(term_t *t);

void                draw(term_t *t);
void                move_player(term_t *t, player_t *p);

/*     utils       */
int                 compare_val_in_buffers(term_t *t, int i);
void                add_randomness_to_movement(player_t *p);
void                copy_last_buffer(term_t *t);
void                free_all(term_t *t);
void                systemExit();
void                save_last_frame_to_file(char * filename, char * buffer, int size);

