#include "ftascii.h"

void change_pixel(Pixel* p, char* c, char* color){
    p->color = color;
    p->c = c;
}

// TODO make out a reference from the outside
char* build_pixel(Pixel pixel) 
{
    char* out = malloc(sizeof(pixel.color) + sizeof(pixel.c) + 1);
    strcpy(out, pixel.color);
    strcat(out, pixel.c);
    strcat(out, "\0");
    return out;
}

void assign_pix_buff(char** buffer, Pixel* pixels,int size) {
    for (int i = 0; i < size - 1; i++) {        
        buffer[i] = build_pixel(pixels[i]);
    }
}

void pix_set(Pixel* pixels,int size) {
    for (int i = 0; i < size; i++) {        
        pixels[i].c = "a";
        pixels[i].color = GREEN;
        pixels[i].len = strlen(pixels[i].color) + strlen(pixels[i].c);
    }
}

