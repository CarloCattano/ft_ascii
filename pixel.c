#include "ftascii.h"

void change_pixel(Pixel* p, char* c, char* color)
{
    p->c = c;
    p->color = color;
    p->len = strlen(p->color) + strlen(p->c);
}

// TODO make out a reference from the outside
char* build_pixel(Pixel pixel) 
{
    char* out = malloc(pixel.len + 1); 
    
    if (!out) perror("Malloc failed"), exit(1);

    strncpy(out, pixel.color, strlen(pixel.color));
    strncat(out, pixel.c, strlen(pixel.c));

    return out;
}

void assign_pix_buff(char* buffer, Pixel* pixels, int size)
{
    memset(buffer, 0, size);
    for (int i = 0; i < size; i++) 
    {        
        char* pixel_output = build_pixel(pixels[i]);
        strncat(buffer, pixel_output, strlen(pixels[i].color) + strlen(pixels[i].c));
        free(pixel_output); 
    }
}

void pix_set(Pixel* pixels,int size)
{
    for (int i = 0; i < size; i++)
    {        
        pixels[i].c = " ";
        pixels[i].color = GREEN;
        pixels[i].len = strlen(pixels[i].color) + strlen(pixels[i].c);
    }
}

