#ifndef SHOWCASE_COMMON_H
#define SHOWCASE_COMMON_H

#include "ftascii.h"

typedef struct s_vec3 {
    float x;
    float y;
    float z;
} t_vec3;

t_color sc_palette(float t);
void    sc_title(t_ascii *ctx, const char *title);
void    sc_line(t_ascii *ctx, int x0, int y0, int x1, int y1, t_color color);
void    sc_plot(t_ascii *ctx, int x, int y, t_color color, const char *glyph);

#endif
