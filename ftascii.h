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
	float			sens;

}                   term_t;

int 				ft_ascii(float *fft_values);
void                init_term(term_t *t);
/*      hooks       */
void                handleKeyPress(char key, term_t *t);
void 				handlectrl_c(int sig);
// void                handlectrl_c(int sig);
void                ft_keyhook(term_t *t);

void                draw(term_t *t, float *fft_values, float sens);

void                move_player(term_t *t);

/*     utils       */
int                 compare_val_in_buffers(term_t *t, int i);
void                copy_last_buffer(term_t *t);
void                free_all(term_t *t);
void                systemExit();

void                assign_pix_buff(char* buffer, Pixel* pixels,int size);
void                pixel_set(Pixel* pixels, int size);
void                pix_set(Pixel* pixels,int size);
void                fill_pixel(Pixel* pixels, char* color, char* uni, int i);

int                 str_to_hex(char *str);
void                putpix(Pixel* pixels, char* color, char* uni);

char*               hex_to_unicode(int hex);

// audio
void 				audio_stats(float *fft_values, int size);
