#ifndef FTASCII_H
#define FTASCII_H

#include "const.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <termios.h>

#define FT_GLYPH_MAX 8
#define FT_OK        (0)
#define FT_ERR       (-1)

#define FT_BLACK   ((t_color){0, 0, 0})
#define FT_RED     ((t_color){255, 0, 0})
#define FT_GREEN   ((t_color){0, 255, 0})
#define FT_BLUE    ((t_color){0, 0, 255})
#define FT_YELLOW  ((t_color){255, 220, 0})
#define FT_CYAN    ((t_color){0, 220, 255})
#define FT_MAGENTA ((t_color){255, 0, 255})
#define FT_WHITE   ((t_color){240, 240, 240})

typedef struct s_color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} t_color;

typedef struct s_cell {
    char    glyph[FT_GLYPH_MAX];
    t_color fg;
    t_color bg;
    uint8_t flags;
} t_cell;

typedef struct s_canvas {
    int     width;
    int     height;
    size_t  len;
    t_cell *front;
    t_cell *back;
    char   *out;
    size_t  out_len;
    size_t  out_cap;
} t_canvas;

typedef struct s_terminal {
    struct termios original;
    int            raw_enabled;
    int            cursor_hidden;
} t_terminal;

typedef struct s_ascii {
    t_canvas   canvas;
    t_terminal term;
    uint64_t   frame;
    int        running;
    int        dirty;
    int        fps;
} t_ascii;

typedef enum e_key {
    FT_KEY_NONE,
    FT_KEY_ESC,
    FT_KEY_UP,
    FT_KEY_DOWN,
    FT_KEY_LEFT,
    FT_KEY_RIGHT,
    FT_KEY_CHAR,
    FT_KEY_RESIZE,
    FT_KEY_QUIT
} t_key;

typedef struct s_event {
    t_key key;
    char  ch;
} t_event;

/* Agentic note: v2 exposes context/canvas/event primitives only; terminal
 * mutation stays inside the library so callers can render safely. */
int  ft_ascii_init(t_ascii *ctx);
void ft_ascii_shutdown(t_ascii *ctx);
int  ft_ascii_running(const t_ascii *ctx);
void ft_ascii_stop(t_ascii *ctx);
int  ft_ascii_check_signal(t_ascii *ctx);
void ft_set_fps(t_ascii *ctx, int fps);
void ft_sleep_ms(unsigned int ms);

void ft_clear(t_ascii *ctx, t_color color);
void ft_present_diff(t_ascii *ctx);
void ft_swap_buffers(t_ascii *ctx);

void ft_put_cell(t_ascii *ctx, int x, int y, t_cell cell);
void ft_put_utf8(t_ascii *ctx, int x, int y, t_color color, const char *glyph);
void ft_put_char(t_ascii *ctx, int x, int y, t_color color, char c);
void ft_rect(t_ascii *ctx, int x, int y, int w, int h, t_color color);
void ft_fill_rect(t_ascii *ctx, int x, int y, int w, int h, t_color color);
void ft_circle(t_ascii *ctx, int cx, int cy, int r, t_color color,
               const char *glyph);
void ft_text(t_ascii *ctx, int x, int y, t_color color, const char *text);

int  ft_poll_event(t_ascii *ctx, t_event *event);
void ft_poll_events(t_ascii *ctx);

#endif
