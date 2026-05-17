#include "ftascii.h"
#include <string.h>

static int ft_cell_index(const t_ascii *ctx, int x, int y, size_t *idx) {
    if (ctx == NULL || idx == NULL)
        return FT_ERR;
    if (x < 0 || y < 0 || x >= ctx->canvas.width || y >= ctx->canvas.height)
        return FT_ERR;
    *idx = (size_t)y * (size_t)ctx->canvas.width + (size_t)x;
    return FT_OK;
}

static void ft_copy_glyph(char dst[FT_GLYPH_MAX], const char *glyph) {
    size_t i;

    memset(dst, 0, FT_GLYPH_MAX);
    if (glyph == NULL || glyph[0] == '\0') {
        dst[0] = ' ';
        return;
    }
    i = 0;
    while (i < FT_GLYPH_MAX - 1 && glyph[i] != '\0') {
        dst[i] = glyph[i];
        i++;
    }
}

void ft_put_cell(t_ascii *ctx, int x, int y, t_cell cell) {
    size_t idx;

    if (ft_cell_index(ctx, x, y, &idx) == FT_ERR)
        return;
    ctx->canvas.back[idx] = cell;
    ctx->dirty            = 1;
}

void ft_put_utf8(t_ascii *ctx, int x, int y, t_color color, const char *glyph) {
    t_cell cell;

    memset(&cell, 0, sizeof(cell));
    cell.fg = color;
    cell.bg = FT_BLACK;
    ft_copy_glyph(cell.glyph, glyph);
    ft_put_cell(ctx, x, y, cell);
}

void ft_put_char(t_ascii *ctx, int x, int y, t_color color, char c) {
    char glyph[2];

    glyph[0] = c;
    glyph[1] = '\0';
    ft_put_utf8(ctx, x, y, color, glyph);
}
