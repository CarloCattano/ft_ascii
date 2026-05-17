#include "common.h"
#include <math.h>

#define SC_PI 3.1415926535f

static void draw_seed(t_ascii *ctx, int cx, int cy, int r, float t) {
    int i;

    ft_circle(ctx, cx, cy, r, sc_palette(t), "·");
    i = 0;
    while (i < 6) {
        float a;
        int   x;
        int   y;

        a = t * 0.35f + (float)i * SC_PI / 3.0f;
        x = cx + (int)(cosf(a) * (float)r);
        y = cy + (int)(sinf(a) * (float)r * 0.55f);
        ft_circle(ctx, x, y, r, sc_palette(t + (float)i), "·");
        i++;
    }
}

static void draw_rays(t_ascii *ctx, int cx, int cy, int r, float t) {
    int i;

    i = 0;
    while (i < 24) {
        float a;
        int   x;
        int   y;

        a = t + (float)i * SC_PI / 12.0f;
        x = cx + (int)(cosf(a) * (float)r);
        y = cy + (int)(sinf(a) * (float)r * 0.55f);
        sc_line(ctx, cx, cy, x, y, sc_palette(a));
        i++;
    }
}

static void frame(t_ascii *ctx, float t) {
    int cx;
    int cy;
    int r;

    cx = ctx->canvas.width / 2;
    cy = ctx->canvas.height / 2 + 2;
    r  = ctx->canvas.height / 6;
    if (r < 3)
        r = 3;
    ft_clear(ctx, FT_BLACK);
    sc_title(ctx, "sacred2d: flower of life + rotating mandala");
    draw_rays(ctx, cx, cy, r * 3, -t * 0.2f);
    draw_seed(ctx, cx, cy, r, t);
    ft_circle(ctx, cx, cy, r * 2, sc_palette(t + 3.0f), "·");
    ft_circle(ctx, cx, cy, r * 3, sc_palette(t + 6.0f), "·");
    ft_present_diff(ctx);
}

int main(void) {
    t_ascii ctx;
    float   t;

    if (ft_ascii_init(&ctx) != FT_OK)
        return 1;
    ft_set_fps(&ctx, 24);
    t = 0.0f;
    while (ft_ascii_running(&ctx)) {
        ft_poll_events(&ctx);
        frame(&ctx, t);
        t += 0.08f;
        ft_sleep_ms(1000U / (unsigned int)ctx.fps);
    }
    ft_ascii_shutdown(&ctx);
    return 0;
}
