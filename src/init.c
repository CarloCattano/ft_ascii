#include "ftascii.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

static volatile sig_atomic_t g_resize_requested;
static volatile sig_atomic_t g_stop_requested;
static t_ascii              *g_active_ctx;

static void ft_signal_handler(int sig) {
    if (sig == SIGWINCH)
        g_resize_requested = 1;
    if (sig == SIGINT || sig == SIGTERM)
        g_stop_requested = 1;
}

static int ft_terminal_size(int *width, int *height) {
    struct winsize w;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
        return FT_ERR;
    if (w.ws_col == 0 || w.ws_row == 0)
        return FT_ERR;
    *width  = (int)w.ws_col;
    *height = (int)w.ws_row;
    return FT_OK;
}

static int ft_enable_raw(t_ascii *ctx) {
    struct termios raw;

    if (tcgetattr(STDIN_FILENO, &ctx->term.original) == -1)
        return FT_ERR;
    raw = ctx->term.original;
    raw.c_lflag &= (tcflag_t) ~(ECHO | ICANON);
    raw.c_iflag &= (tcflag_t) ~(ICRNL | IXON);
    raw.c_cc[VMIN]  = 0;
    raw.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        return FT_ERR;
    ctx->term.raw_enabled = 1;
    return FT_OK;
}

static int ft_canvas_alloc(t_canvas *c, int width, int height) {
    size_t len;

    if (width <= 0 || height <= 0)
        return FT_ERR;
    len      = (size_t)width * (size_t)height;
    c->front = calloc(len, sizeof(t_cell));
    c->back  = calloc(len, sizeof(t_cell));
    /* Agentic note: diff rendering can emit cursor + color + glyph per cell,
     * so the output arena is intentionally over-provisioned. */
    c->out_cap = len * 80 + 128;
    c->out     = malloc(c->out_cap);
    if (c->front == NULL || c->back == NULL || c->out == NULL)
        return FT_ERR;
    c->width  = width;
    c->height = height;
    c->len    = len;
    return FT_OK;
}

static void ft_canvas_free(t_canvas *c) {
    free(c->front);
    free(c->back);
    free(c->out);
    memset(c, 0, sizeof(*c));
}

static int ft_install_signals(void) {
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = ft_signal_handler;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        return FT_ERR;
    if (sigaction(SIGTERM, &sa, NULL) == -1)
        return FT_ERR;
    if (sigaction(SIGWINCH, &sa, NULL) == -1)
        return FT_ERR;
    return FT_OK;
}

int ft_ascii_init(t_ascii *ctx) {
    int width;
    int height;

    if (ctx == NULL)
        return FT_ERR;
    memset(ctx, 0, sizeof(*ctx));
    if (ft_terminal_size(&width, &height) == FT_ERR)
        return FT_ERR;
    if (ft_canvas_alloc(&ctx->canvas, width, height) == FT_ERR)
        return ft_ascii_shutdown(ctx), FT_ERR;
    if (ft_enable_raw(ctx) == FT_ERR)
        return ft_ascii_shutdown(ctx), FT_ERR;
    if (ft_install_signals() == FT_ERR)
        return ft_ascii_shutdown(ctx), FT_ERR;
    write(STDOUT_FILENO, NOMOUSE CLEAR_ALL, sizeof(NOMOUSE CLEAR_ALL) - 1);
    ctx->term.cursor_hidden = 1;
    ctx->running            = 1;
    ctx->dirty              = 1;
    ctx->fps                = 60;
    g_active_ctx            = ctx;
    ft_clear(ctx, FT_BLACK);
    return FT_OK;
}

void ft_ascii_shutdown(t_ascii *ctx) {
    if (ctx == NULL)
        return;
    if (ctx->term.raw_enabled)
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &ctx->term.original);
    if (ctx->term.cursor_hidden)
        write(STDOUT_FILENO, SHOW_CURSOR RESET_STYLE CLEAR_ALL,
              sizeof(SHOW_CURSOR RESET_STYLE CLEAR_ALL) - 1);
    ft_canvas_free(&ctx->canvas);
    ctx->running = 0;
    if (g_active_ctx == ctx)
        g_active_ctx = NULL;
}

static int ft_ascii_resize(t_ascii *ctx) {
    t_canvas old;
    int      width;
    int      height;

    if (ctx == NULL || ft_terminal_size(&width, &height) == FT_ERR)
        return FT_ERR;
    if (width == ctx->canvas.width && height == ctx->canvas.height)
        return FT_OK;
    old = ctx->canvas;
    memset(&ctx->canvas, 0, sizeof(ctx->canvas));
    if (ft_canvas_alloc(&ctx->canvas, width, height) == FT_ERR) {
        ft_canvas_free(&ctx->canvas);
        ctx->canvas = old;
        return FT_ERR;
    }
    ft_canvas_free(&old);
    ctx->dirty = 1;
    write(STDOUT_FILENO, CLEAR_ALL, 7);
    return FT_OK;
}

int ft_ascii_check_signal(t_ascii *ctx) {
    if (ctx == NULL)
        ctx = g_active_ctx;
    if (g_stop_requested && ctx != NULL)
        ft_ascii_stop(ctx);
    if (g_resize_requested && ctx != NULL) {
        g_resize_requested = 0;
        return ft_ascii_resize(ctx);
    }
    return FT_OK;
}

int ft_ascii_running(const t_ascii *ctx) {
    return (ctx != NULL && ctx->running);
}

void ft_ascii_stop(t_ascii *ctx) {
    if (ctx != NULL)
        ctx->running = 0;
}

void ft_set_fps(t_ascii *ctx, int fps) {
    if (ctx != NULL && fps > 0)
        ctx->fps = fps;
}
