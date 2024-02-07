#include "ftascii.h"

void change_pixel(Pixel* p, char* c, char* color)
{
    p->color = color;
    p->c = c;
}

// TODO make out a reference from the outside
char* build_pixel(Pixel pixel) 
{
    char* out = malloc(pixel.len + 1); 
    
    if (!out) perror("Malloc failed"), exit(1);

    strcpy(out, pixel.color);
    strcat(out, pixel.c);
    return out;
}

void assign_pix_buff(char* buffer, Pixel* pixels, int size)
{
    memset(buffer, 0, size);
    for (int i = 0; i < size; i++) 
    {        
        char* pixel_output = build_pixel(pixels[i]);
        strcat(buffer, pixel_output);
        free(pixel_output); 
    }
}

void pix_set(Pixel* pixels,int size)
{
    for (int i = 0; i < size; i++)
    {        
        pixels[i].c = "a";
        pixels[i].color = GREEN;
        pixels[i].len = strlen(pixels[i].color) + strlen(pixels[i].c);
    }
}

