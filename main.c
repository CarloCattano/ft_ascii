#include "ftascii.h"
#include <stdio.h>

typedef struct s_ball {
    int x;
    int y;
    int dx;
    int dy;
} t_ball;

static void ball_init(t_ball *ball) {
    ball->x  = 4;
    ball->y  = 4;
    ball->dx = 1;
    ball->dy = 1;
}

static void ball_move(const t_ascii *ctx, t_ball *ball) {
    ball->x += ball->dx;
    ball->y += ball->dy;
    if (ball->x <= 2 || ball->x >= ctx->canvas.width - 3)
        ball->dx = -ball->dx;
    if (ball->y <= 2 || ball->y >= ctx->canvas.height - 3)
        ball->dy = -ball->dy;
}

static void draw_scene(t_ascii *ctx, const t_ball *ball) {
    ft_clear(ctx, FT_BLACK);
    ft_rect(ctx, 0, 0, ctx->canvas.width, ctx->canvas.height, FT_CYAN);
    ft_text(ctx, 2, 1, FT_WHITE, "libftascii v2 - q/esc exits");
    ft_circle(ctx, ball->x, ball->y, 1, FT_RED, "⬤");
    ft_present_diff(ctx);
}

int main(void) {
    t_ascii ctx;
    t_ball  ball;

    if (ft_ascii_init(&ctx) != FT_OK) {
        fprintf(stderr, "ft_ascii_init failed: run inside a real terminal\n");
        return 1;
    }
    ft_set_fps(&ctx, 30);
    ball_init(&ball);
    while (ft_ascii_running(&ctx)) {
        ft_poll_events(&ctx);
        ball_move(&ctx, &ball);
        draw_scene(&ctx, &ball);
        ft_sleep_ms(1000U / (unsigned int)ctx.fps);
    }
    ft_ascii_shutdown(&ctx);
    return 0;
}
