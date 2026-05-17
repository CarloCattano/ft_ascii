#include "ftascii.h"
#include <stdlib.h>
#include <time.h>

typedef enum e_dir { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT } t_dir;

typedef struct s_point {
    int x;
    int y;
} t_point;

typedef struct s_snake {
    t_point body[2048];
    int     len;
    t_dir   dir;
    t_point food;
    int     score;
    int     dead;
} t_snake;

static int same_point(t_point a, t_point b) {
    return (a.x == b.x && a.y == b.y);
}

static int snake_contains(const t_snake *snake, t_point p) {
    int i;

    i = 0;
    while (i < snake->len) {
        if (same_point(snake->body[i], p))
            return 1;
        i++;
    }
    return 0;
}

static int playable_width(const t_ascii *ctx) {
    if (ctx->canvas.width < 6)
        return 1;
    return ctx->canvas.width - 4;
}

static int playable_height(const t_ascii *ctx) {
    if (ctx->canvas.height < 6)
        return 1;
    return ctx->canvas.height - 4;
}

static void place_food(const t_ascii *ctx, t_snake *snake) {
    t_point p;
    int     guard;

    guard = 0;
    while (guard++ < 4096) {
        p.x = 2 + rand() % playable_width(ctx);
        p.y = 2 + rand() % playable_height(ctx);
        if (!snake_contains(snake, p)) {
            snake->food = p;
            return;
        }
    }
    snake->food = (t_point){2, 2};
}

static void snake_init(const t_ascii *ctx, t_snake *snake) {
    int cx;
    int cy;

    cx             = ctx->canvas.width / 2;
    cy             = ctx->canvas.height / 2;
    snake->len     = 5;
    snake->dir     = DIR_RIGHT;
    snake->score   = 0;
    snake->dead    = 0;
    snake->body[0] = (t_point){cx, cy};
    snake->body[1] = (t_point){cx - 1, cy};
    snake->body[2] = (t_point){cx - 2, cy};
    snake->body[3] = (t_point){cx - 3, cy};
    snake->body[4] = (t_point){cx - 4, cy};
    place_food(ctx, snake);
}

static int is_reverse(t_dir old_dir, t_dir new_dir) {
    if (old_dir == DIR_UP && new_dir == DIR_DOWN)
        return 1;
    if (old_dir == DIR_DOWN && new_dir == DIR_UP)
        return 1;
    if (old_dir == DIR_LEFT && new_dir == DIR_RIGHT)
        return 1;
    if (old_dir == DIR_RIGHT && new_dir == DIR_LEFT)
        return 1;
    return 0;
}

static void set_dir(t_snake *snake, t_dir dir) {
    if (!is_reverse(snake->dir, dir))
        snake->dir = dir;
}

static void handle_event(t_snake *snake, t_event event) {
    if (event.key == FT_KEY_UP || event.ch == 'w')
        set_dir(snake, DIR_UP);
    else if (event.key == FT_KEY_DOWN || event.ch == 's')
        set_dir(snake, DIR_DOWN);
    else if (event.key == FT_KEY_LEFT || event.ch == 'a')
        set_dir(snake, DIR_LEFT);
    else if (event.key == FT_KEY_RIGHT || event.ch == 'd')
        set_dir(snake, DIR_RIGHT);
}

static void poll_game_events(t_ascii *ctx, t_snake *snake) {
    t_event event;

    while (ft_poll_event(ctx, &event))
        handle_event(snake, event);
}

static t_point next_head(const t_snake *snake) {
    t_point p;

    p = snake->body[0];
    if (snake->dir == DIR_UP)
        p.y--;
    else if (snake->dir == DIR_DOWN)
        p.y++;
    else if (snake->dir == DIR_LEFT)
        p.x--;
    else if (snake->dir == DIR_RIGHT)
        p.x++;
    return p;
}

static int hit_wall(const t_ascii *ctx, t_point p) {
    return (p.x <= 0 || p.y <= 1 || p.x >= ctx->canvas.width - 1 ||
            p.y >= ctx->canvas.height - 1);
}

static int hit_self(const t_snake *snake, t_point p) {
    int i;

    i = 0;
    while (i < snake->len) {
        if (same_point(snake->body[i], p))
            return 1;
        i++;
    }
    return 0;
}

static void snake_step(const t_ascii *ctx, t_snake *snake) {
    t_point head;
    int     grow;
    int     i;

    if (snake->dead)
        return;
    head = next_head(snake);
    grow = same_point(head, snake->food);
    if (hit_wall(ctx, head) || hit_self(snake, head)) {
        snake->dead = 1;
        return;
    }
    if (grow && snake->len < 2048)
        snake->len++;
    i = snake->len - 1;
    while (i > 0) {
        snake->body[i] = snake->body[i - 1];
        i--;
    }
    snake->body[0] = head;
    if (grow) {
        snake->score++;
        place_food(ctx, snake);
    }
}

static void draw_hud(t_ascii *ctx, const t_snake *snake) {
    char score[64];

    ft_text(ctx, 2, 0, FT_WHITE, "snake: arrows/wasd move | q/esc exits");
    score[0] = '0' + (snake->score % 10);
    score[1] = '\0';
    ft_text(ctx, ctx->canvas.width - 10, 0, FT_YELLOW, "score:");
    ft_text(ctx, ctx->canvas.width - 3, 0, FT_YELLOW, score);
    if (snake->dead)
        ft_text(ctx, ctx->canvas.width / 2 - 5, ctx->canvas.height / 2, FT_RED,
                "GAME OVER");
}

static void draw_snake(t_ascii *ctx, const t_snake *snake) {
    int i;

    ft_put_utf8(ctx, snake->food.x, snake->food.y, FT_RED, "●");
    i = 0;
    while (i < snake->len) {
        if (i == 0)
            ft_put_utf8(ctx, snake->body[i].x, snake->body[i].y, FT_YELLOW,
                        "◉");
        else
            ft_put_utf8(ctx, snake->body[i].x, snake->body[i].y, FT_GREEN, "█");
        i++;
    }
}

static void draw_game(t_ascii *ctx, const t_snake *snake) {
    ft_clear(ctx, FT_BLACK);
    ft_rect(ctx, 0, 1, ctx->canvas.width, ctx->canvas.height - 1, FT_CYAN);
    draw_hud(ctx, snake);
    draw_snake(ctx, snake);
    ft_present_diff(ctx);
}

int main(void) {
    t_ascii ctx;
    t_snake snake;

    srand((unsigned int)time(NULL));
    if (ft_ascii_init(&ctx) != FT_OK)
        return 1;
    ft_set_fps(&ctx, 12);
    snake_init(&ctx, &snake);
    while (ft_ascii_running(&ctx)) {
        poll_game_events(&ctx, &snake);
        snake_step(&ctx, &snake);
        draw_game(&ctx, &snake);
        ft_sleep_ms(1000U / (unsigned int)ctx.fps);
    }
    ft_ascii_shutdown(&ctx);
    return 0;
}
