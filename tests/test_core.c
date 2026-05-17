#include "ftascii.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int g_failures;

#define CHECK(name, expr)                                                      \
    do {                                                                       \
        if (!(expr)) {                                                         \
            fprintf(stderr, "FAIL: %s\n", name);                               \
            g_failures++;                                                      \
        }                                                                      \
    } while (0)

static int alloc_ctx(t_ascii *ctx, int width, int height) {
    size_t len;

    memset(ctx, 0, sizeof(*ctx));
    len                 = (size_t)width * (size_t)height;
    ctx->canvas.width   = width;
    ctx->canvas.height  = height;
    ctx->canvas.len     = len;
    ctx->canvas.front   = calloc(len, sizeof(t_cell));
    ctx->canvas.back    = calloc(len, sizeof(t_cell));
    ctx->canvas.out_cap = len * 80 + 128;
    ctx->canvas.out     = malloc(ctx->canvas.out_cap);
    ctx->running        = 1;
    ctx->fps            = 60;
    return (ctx->canvas.front != NULL && ctx->canvas.back != NULL &&
            ctx->canvas.out != NULL);
}

static void free_ctx(t_ascii *ctx) {
    free(ctx->canvas.front);
    free(ctx->canvas.back);
    free(ctx->canvas.out);
    memset(ctx, 0, sizeof(*ctx));
}

static size_t idx(const t_ascii *ctx, int x, int y) {
    return (size_t)y * (size_t)ctx->canvas.width + (size_t)x;
}

static int color_eq(t_color a, t_color b) {
    return (a.r == b.r && a.g == b.g && a.b == b.b);
}

static void test_clear_sets_background(void) {
    t_ascii ctx;

    CHECK("alloc clear ctx", alloc_ctx(&ctx, 4, 3));
    ft_clear(&ctx, FT_BLUE);
    CHECK("clear dirty", ctx.dirty == 1);
    CHECK("clear glyph", ctx.canvas.back[0].glyph[0] == ' ');
    CHECK("clear bg", color_eq(ctx.canvas.back[0].bg, FT_BLUE));
    CHECK("clear fg", color_eq(ctx.canvas.back[0].fg, FT_WHITE));
    free_ctx(&ctx);
}

static void test_put_utf8_and_bounds(void) {
    t_ascii ctx;
    size_t  p;

    CHECK("alloc put ctx", alloc_ctx(&ctx, 5, 5));
    ft_put_utf8(&ctx, 2, 3, FT_RED, "@ ");
    p = idx(&ctx, 2, 3);
    CHECK("put glyph", strcmp(ctx.canvas.back[p].glyph, "@ ") == 0);
    CHECK("put fg", color_eq(ctx.canvas.back[p].fg, FT_RED));
    ctx.dirty = 0;
    ft_put_utf8(&ctx, -1, 2, FT_GREEN, "X");
    CHECK("out of bounds does not dirty", ctx.dirty == 0);
    free_ctx(&ctx);
}

static void test_text_and_swap(void) {
    t_ascii ctx;

    CHECK("alloc text ctx", alloc_ctx(&ctx, 8, 2));
    ft_text(&ctx, 1, 1, FT_YELLOW, "abc");
    CHECK("text a", ctx.canvas.back[idx(&ctx, 1, 1)].glyph[0] == 'a');
    CHECK("text b", ctx.canvas.back[idx(&ctx, 2, 1)].glyph[0] == 'b');
    CHECK("text c", ctx.canvas.back[idx(&ctx, 3, 1)].glyph[0] == 'c');
    ft_swap_buffers(&ctx);
    CHECK("swap frame", ctx.frame == 1);
    CHECK("swap dirty clear", ctx.dirty == 0);
    CHECK("swap copied", ctx.canvas.front[idx(&ctx, 3, 1)].glyph[0] == 'c');
    free_ctx(&ctx);
}

static void test_rect_and_fill(void) {
    t_ascii ctx;

    CHECK("alloc rect ctx", alloc_ctx(&ctx, 8, 6));
    ft_clear(&ctx, FT_BLACK);
    ft_rect(&ctx, 1, 1, 4, 3, FT_CYAN);
    CHECK("rect top", ctx.canvas.back[idx(&ctx, 1, 1)].glyph[0] != ' ');
    CHECK("rect right", ctx.canvas.back[idx(&ctx, 4, 2)].glyph[0] != ' ');
    ft_fill_rect(&ctx, 2, 2, 2, 2, FT_MAGENTA);
    CHECK("fill bg", color_eq(ctx.canvas.back[idx(&ctx, 2, 2)].bg, FT_MAGENTA));
    free_ctx(&ctx);
}

static void test_running_stop(void) {
    t_ascii ctx;

    CHECK("alloc running ctx", alloc_ctx(&ctx, 2, 2));
    CHECK("running true", ft_ascii_running(&ctx) == 1);
    ft_ascii_stop(&ctx);
    CHECK("running false", ft_ascii_running(&ctx) == 0);
    free_ctx(&ctx);
}

int main(void) {
    test_clear_sets_background();
    test_put_utf8_and_bounds();
    test_text_and_swap();
    test_rect_and_fill();
    test_running_stop();
    if (g_failures != 0)
        return 1;
    printf("unit tests passed\n");
    return 0;
}
