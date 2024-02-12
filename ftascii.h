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
#include <locale.h>
#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <fftw3.h>
#include <math.h>
#include "portaudio.h"

#define   CLEAR     "\033[H"
#define   NOMOUSE   "\033[?25l"

#define   RED       "\033[31m"
#define   GREEN     "\033[32m"
#define   YELLOW    "\033[33m"
#define   BLUE      "\033[34m"
#define   MAGENTA   "\033[35m"
#define   CYAN      "\033[36m"
#define   WHITE     "\033[37m"
#define   BLACK     "\033[30m"
#define   RST       "\033[0m"

#define unicodechars "▁▂▃▄▅▆▇█"

#define IMG_SIZE 12

#ifndef FFT_SIZE
# define  FFT_SIZE 16
#endif
#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 512

// enum for colors

typedef struct s_unicode
{
    char*       uni;
    char*       color;
    
} t_uni;

typedef struct s_pixel
{
    t_uni    data;
}            Pixel;

typedef struct player_s
{
    int             posx;
    int             posy;
    int             dx;
    int             dy;
}                   player_t;

typedef struct term_s
{
    int             MAX_COL;
    int             MAX_ROW;
    int             size;
    char*           buffer;
	char* 		    buffer_copy;
    Pixel*          pixels;
	unsigned int    frame;
    int 		    clear;
    unsigned int    delay;
    player_t* 	    players[4];

}                   term_t;

void                init_term(term_t *t);

/*      hooks       */
void                handleKeyPress(char key, term_t *t);
// void                handlectrl_c(int sig);
void                ft_keyhook(term_t *t);

void                draw(term_t *t);
void                move_player(term_t *t);

/*     utils       */
int                 compare_val_in_buffers(term_t *t, int i);
void                add_randomness_to_movement(player_t *p);
void                copy_last_buffer(term_t *t);
void                free_all(term_t *t);
void                systemExit();
void                save_last_frame_to_file(char * filename, char * buffer, int size);

void                assign_pix_buff(char* buffer, Pixel* pixels,int size);
void                pixel_set(Pixel* pixels, int size);
void                pix_set(Pixel* pixels,int size);
void                fill_pixel(Pixel* pixels, char* color, char* uni, int i);

int                 str_to_hex(char *str);
void                putpix(Pixel* pixels, char* color, char* uni);

char*               hex_to_unicode(int hex);
int 				run_app(void);
