#include "common.h"
#include <math.h>

#define FLOW_N 96

typedef struct s_particle {
    float x;
    float y;
} t_particle;

static void init_particles(t_particle p[FLOW_N], const t_ascii *ctx) {
    int i;

    i = 0;
    while (i < FLOW_N) {
        p[i].x = (float)((i * 37) % ctx->canvas.width);
        p[i].y = 3.0f + (float)((i * 19) % (ctx->canvas.height - 4));
        i++;
    }
}

static float field_angle(float x, float y, float t) {
    return sinf(x * 0.09f + t) + cosf(y * 0.13f - t * 0.7f);
}

static void wrap_particle(t_particle *p, const t_ascii *ctx) {
    if (p->x < 1.0f)
        p->x = (float)(ctx->canvas.width - 2);
    if (p->x > (float)(ctx->canvas.width - 2))
        p->x = 1.0f;
    if (p->y < 3.0f)
        p->y = (float)(ctx->canvas.height - 2);
    if (p->y > (float)(ctx->canvas.height - 2))
        p->y = 3.0f;
}

static void update_particle(t_particle *p, const t_ascii *ctx, float t) {
    float a;

    a = field_angle(p->x, p->y, t);
    p->x += cosf(a) * 0.9f;
    p->y += sinf(a) * 0.45f;
    wrap_particle(p, ctx);
}

static void draw_vectors(t_ascii *ctx, float t) {
    int y;

    y = 4;
    while (y < ctx->canvas.height - 2) {
        int x;

        x = 2;
        while (x < ctx->canvas.width - 2) {
            float a;
            int   x2;
            int   y2;

            a  = field_angle((float)x, (float)y, t);
            x2 = x + (int)(cosf(a) * 3.0f);
            y2 = y + (int)(sinf(a) * 1.5f);
            sc_line(ctx, x, y, x2, y2, sc_palette(a + t));
            x += 10;
        }
        y += 5;
    }
}

static void draw_particles(t_ascii *ctx, t_particle p[FLOW_N], float t) {
    int i;

    i = 0;
    while (i < FLOW_N) {
        update_particle(&p[i], ctx, t);
        sc_plot(ctx, (int)p[i].x, (int)p[i].y, sc_palette(t + (float)i), "✦");
        i++;
    }
}

int main(void) {
    t_ascii    ctx;
    t_particle particles[FLOW_N];
    float      t;

    if (ft_ascii_init(&ctx) != FT_OK)
        return 1;
    ft_set_fps(&ctx, 20);
    init_particles(particles, &ctx);
    t = 0.0f;
    while (ft_ascii_running(&ctx)) {
        ft_poll_events(&ctx);
        ft_clear(&ctx, FT_BLACK);
        sc_title(&ctx, "flow2d: nature-of-code particle field");
        draw_vectors(&ctx, t);
        draw_particles(&ctx, particles, t);
        ft_present_diff(&ctx);
        t += 0.05f;
        ft_sleep_ms(1000U / (unsigned int)ctx.fps);
    }
    ft_ascii_shutdown(&ctx);
    return 0;
}
