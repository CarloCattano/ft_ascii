#include "ftascii.h"

Pixel create_pixel(char* color, char* str)
{
    Pixel     pixel;
    
    pixel.color = color;
    pixel.c = str;
    
    return (pixel);
}

void change_pixel(Pixel* p, char* c, char* color)
{
    p->color = color;
    p->c = c;
}

char* build_pixel(Pixel pixel)
{
    char* out = malloc(strlen(pixel.color) + strlen(pixel.c));

    strcpy(out, pixel.color);
    strcat(out, pixel.c);
    return (out);
}
