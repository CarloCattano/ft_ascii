#include "common.h"
#include <math.h>

static const t_vec3 g_cube[8] = {{-1, -1, -1}, {1, -1, -1}, {1, 1, -1},
                                 {-1, 1, -1},  {-1, -1, 1}, {1, -1, 1},
                                 {1, 1, 1},    {-1, 1, 1}};

static const int g_edges[12][2] = {{0, 1}, {1, 2}, {2, 3}, {3, 0},
                                   {4, 5}, {5, 6}, {6, 7}, {7, 4},
                                   {0, 4}, {1, 5}, {2, 6}, {3, 7}};

static t_vec3 rotate_point(t_vec3 p, float t) {
    float cx;
    float sx;
    float cy;
    float sy;
    float x;
    float z;

    cx  = cosf(t * 0.7f);
    sx  = sinf(t * 0.7f);
    cy  = cosf(t * 1.1f);
    sy  = sinf(t * 1.1f);
    x   = p.x * cy - p.z * sy;
    z   = p.x * sy + p.z * cy;
    p.x = x;
    p.z = z;
    p.y = p.y * cx - p.z * sx;
    p.z = p.y * sx + p.z * cx;
    return p;
}

static void project(t_ascii *ctx, t_vec3 p, int *x, int *y) {
    float scale;
    float depth;

    depth = 3.6f + p.z;
    scale = (float)ctx->canvas.height / depth;
    *x    = ctx->canvas.width / 2 + (int)(p.x * scale * 1.8f);
    *y    = ctx->canvas.height / 2 + (int)(p.y * scale * 0.9f);
}

static void draw_cube(t_ascii *ctx, float t, float radius) {
    int i;
    int px[8];
    int py[8];

    i = 0;
    while (i < 8) {
        t_vec3 p;

        p = g_cube[i];
        p.x *= radius;
        p.y *= radius;
        p.z *= radius;
        p = rotate_point(p, t);
        project(ctx, p, &px[i], &py[i]);
        i++;
    }
    i = 0;
    while (i < 12) {
        int a;
        int b;

        a = g_edges[i][0];
        b = g_edges[i][1];
        sc_line(ctx, px[a], py[a], px[b], py[b], sc_palette(t + (float)i));
        i++;
    }
}

static void draw_orbit(t_ascii *ctx, float t) {
    int i;
    int cx;
    int cy;

    cx = ctx->canvas.width / 2;
    cy = ctx->canvas.height / 2;
    i  = 0;
    while (i < 72) {
        float a;
        float r;
        int   x;
        int   y;

        a = t + (float)i * 0.1745329f;
        r = 3.0f + sinf(t * 2.0f + (float)i) * 2.0f;
        x = cx + (int)(cosf(a) * r * 4.0f);
        y = cy + (int)(sinf(a * 2.0f) * r);
        sc_plot(ctx, x, y, sc_palette(a), "·");
        i++;
    }
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
        ft_clear(&ctx, FT_BLACK);
        sc_title(&ctx, "cube3d: rotating wireframe + harmonic orbit");
        draw_orbit(&ctx, t);
        draw_cube(&ctx, t, 0.9f);
        draw_cube(&ctx, -t * 0.7f, 0.45f);
        ft_present_diff(&ctx);
        t += 0.05f;
        ft_sleep_ms(1000U / (unsigned int)ctx.fps);
    }
    ft_ascii_shutdown(&ctx);
    return 0;
}
