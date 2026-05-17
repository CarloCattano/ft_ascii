#include "ftascii.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int ft_same_cell(const t_cell *a, const t_cell *b) {
    return (memcmp(a, b, sizeof(t_cell)) == 0);
}

static void ft_writer_reset(t_canvas *c) {
    c->out_len = 0;
    if (c->out != NULL && c->out_cap > 0)
        c->out[0] = '\0';
}

static void ft_writer_add(t_canvas *c, const char *s) {
    size_t len;

    if (c == NULL || c->out == NULL || s == NULL)
        return;
    len = strlen(s);
    if (c->out_len + len >= c->out_cap)
        return;
    memcpy(c->out + c->out_len, s, len);
    c->out_len += len;
}

static void ft_writer_add_color(t_canvas *c, t_color fg, t_color bg) {
    char ansi[64];

    snprintf(ansi, sizeof(ansi), "\033[38;2;%u;%u;%um\033[48;2;%u;%u;%um", fg.r,
             fg.g, fg.b, bg.r, bg.g, bg.b);
    ft_writer_add(c, ansi);
}

static void ft_writer_add_cursor(t_canvas *c, int x, int y) {
    char ansi[32];

    snprintf(ansi, sizeof(ansi), "\033[%d;%dH", y + 1, x + 1);
    ft_writer_add(c, ansi);
}

static void ft_emit_cell(t_canvas *c, const t_cell *cell) {
    ft_writer_add_color(c, cell->fg, cell->bg);
    ft_writer_add(c, cell->glyph[0] == '\0' ? " " : cell->glyph);
}

void ft_clear(t_ascii *ctx, t_color color) {
    t_cell cell;
    size_t i;

    if (ctx == NULL || ctx->canvas.back == NULL)
        return;
    memset(&cell, 0, sizeof(cell));
    cell.fg       = FT_WHITE;
    cell.bg       = color;
    cell.glyph[0] = ' ';
    i             = 0;
    while (i < ctx->canvas.len)
        ctx->canvas.back[i++] = cell;
    ctx->dirty = 1;
}

void ft_swap_buffers(t_ascii *ctx) {
    if (ctx == NULL || ctx->canvas.front == NULL || ctx->canvas.back == NULL)
        return;
    memcpy(ctx->canvas.front, ctx->canvas.back,
           sizeof(t_cell) * ctx->canvas.len);
    ctx->frame++;
    ctx->dirty = 0;
}

void ft_present_diff(t_ascii *ctx) {
    int y;

    if (ctx == NULL || ctx->dirty == 0)
        return;
    ft_ascii_check_signal(ctx);
    ft_writer_reset(&ctx->canvas);
    y = 0;
    while (y < ctx->canvas.height) {
        int x;

        x = 0;
        while (x < ctx->canvas.width) {
            size_t idx;

            idx = (size_t)y * (size_t)ctx->canvas.width + (size_t)x;
            if (!ft_same_cell(&ctx->canvas.front[idx],
                              &ctx->canvas.back[idx])) {
                ft_writer_add_cursor(&ctx->canvas, x, y);
                ft_emit_cell(&ctx->canvas, &ctx->canvas.back[idx]);
            }
            x++;
        }
        y++;
    }
    ft_writer_add(&ctx->canvas, RESET_STYLE);
    write(STDOUT_FILENO, ctx->canvas.out, ctx->canvas.out_len);
    ft_swap_buffers(ctx);
}

static void ft_line(t_ascii *ctx, int x0, int y0, int x1, int y1,
                    t_color color) {
    int dx  = abs(x1 - x0);
    int sx  = x0 < x1 ? 1 : -1;
    int dy  = -abs(y1 - y0);
    int sy  = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while (1) {
        int e2;

        ft_put_utf8(ctx, x0, y0, color, "█");
        if (x0 == x1 && y0 == y1)
            break;
        e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void ft_rect(t_ascii *ctx, int x, int y, int w, int h, t_color color) {
    if (w <= 0 || h <= 0)
        return;
    ft_line(ctx, x, y, x + w - 1, y, color);
    ft_line(ctx, x, y + h - 1, x + w - 1, y + h - 1, color);
    ft_line(ctx, x, y, x, y + h - 1, color);
    ft_line(ctx, x + w - 1, y, x + w - 1, y + h - 1, color);
}

void ft_fill_rect(t_ascii *ctx, int x, int y, int w, int h, t_color color) {
    t_cell cell;
    int    yy;

    if (w <= 0 || h <= 0)
        return;
    memset(&cell, 0, sizeof(cell));
    cell.glyph[0] = ' ';
    cell.fg       = FT_WHITE;
    cell.bg       = color;
    yy            = y;
    while (yy < y + h) {
        int xx;

        xx = x;
        while (xx < x + w)
            ft_put_cell(ctx, xx++, yy, cell);
        yy++;
    }
}

static void ft_circle_octants(t_ascii *ctx, int cx, int cy, int x, int y,
                              t_color color, const char *glyph) {
    ft_put_utf8(ctx, cx + x, cy + y, color, glyph);
    ft_put_utf8(ctx, cx - x, cy + y, color, glyph);
    ft_put_utf8(ctx, cx + x, cy - y, color, glyph);
    ft_put_utf8(ctx, cx - x, cy - y, color, glyph);
    ft_put_utf8(ctx, cx + y, cy + x, color, glyph);
    ft_put_utf8(ctx, cx - y, cy + x, color, glyph);
    ft_put_utf8(ctx, cx + y, cy - x, color, glyph);
    ft_put_utf8(ctx, cx - y, cy - x, color, glyph);
}

void ft_circle(t_ascii *ctx, int cx, int cy, int r, t_color color,
               const char *glyph) {
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    if (glyph == NULL)
        glyph = "█";
    while (y >= x) {
        ft_circle_octants(ctx, cx, cy, x, y, color, glyph);
        x++;
        if (d > 0) {
            y--;
            d += 4 * (x - y) + 10;
        } else
            d += 4 * x + 6;
    }
}

void ft_text(t_ascii *ctx, int x, int y, t_color color, const char *text) {
    int i;

    if (text == NULL)
        return;
    i = 0;
    while (text[i] != '\0') {
        ft_put_char(ctx, x + i, y, color, text[i]);
        i++;
    }
}
