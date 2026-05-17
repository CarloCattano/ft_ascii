#include "common.h"
#include <math.h>
#include <stdlib.h>

static unsigned char wave(float x) {
    return (unsigned char)(127.5f + 127.5f * sinf(x));
}

t_color sc_palette(float t) {
    t_color c;

    c.r = wave(t);
    c.g = wave(t + 2.0943951f);
    c.b = wave(t + 4.1887902f);
    return c;
}

void sc_title(t_ascii *ctx, const char *title) {
    ft_text(ctx, 2, 0, FT_WHITE, title);
    ft_text(ctx, 2, 1, FT_CYAN, "creative-coding showcase | q/esc exits");
}

void sc_plot(t_ascii *ctx, int x, int y, t_color color, const char *glyph) {
    ft_put_utf8(ctx, x, y, color, glyph);
}

void sc_line(t_ascii *ctx, int x0, int y0, int x1, int y1, t_color color) {
    int dx;
    int sx;
    int dy;
    int sy;
    int err;

    dx  = abs(x1 - x0);
    sx  = x0 < x1 ? 1 : -1;
    dy  = -abs(y1 - y0);
    sy  = y0 < y1 ? 1 : -1;
    err = dx + dy;
    while (1) {
        int e2;

        sc_plot(ctx, x0, y0, color, "•");
        if (x0 == x1 && y0 == y1)
            return;
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
