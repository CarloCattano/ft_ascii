#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#define   CLEAR     "\033[H"
#define   NOMOUSE   "\033[?25l"

#define COL "\033[0;"
#define RESET "\033[0m"
#define GREEN "\033[032m"
#define YELLOW "\033[33m"
#define RED "\033[31m"


typedef struct s_pixel
{
    char    *c;
    char    *color;
    size_t  len;

}           t_pixel;

void change_pixel(t_pixel* p, char* c, char* color){
    p->color = color;
    p->c = c;
}

// TODO make out a reference from the outside
char* build_pixel(t_pixel pixel) 
{
    char* out = malloc(sizeof(pixel.color) + sizeof(pixel.c) + 1);
    strcpy(out, pixel.color);
    strcat(out, pixel.c);
    strcat(out, "\0");
    return out;
}

void assign_pix_buff(char** buffer,t_pixel* pixels,int size) {
    for (int i = 0; i < size - 1; i++) {        
        buffer[i] = build_pixel(pixels[i]);
    }
}

void pix_set(t_pixel* pixels,int size) {
    for (int i = 0; i < size; i++) {        
        pixels[i].c = "a";
        pixels[i].color = GREEN;
        pixels[i].len = strlen(pixels[i].color) + strlen(pixels[i].c);
    }
}

void draw(char** buffer) {
    
    int i = 0;
    while(buffer[i] != NULL) 
    {
        if(i % 8 == 0) write(1, "\n", 1);
        write(1, buffer[i], strlen(buffer[i]));
        i++;
    }
}

int main()
{ 
    int h = 8;
    int w = 8;
    int size = w * h;
    int i = -1;
    
    char**      buffer = calloc(size, sizeof(t_pixel));
    t_pixel*    pixels = (t_pixel*)malloc(sizeof(t_pixel) * size);

    pix_set(pixels, size);
    while(++i < 4)
    {
        change_pixel(&pixels[i + rand() % size],"℺", YELLOW);
        assign_pix_buff(buffer,pixels,size);
        draw(buffer);
        write(1, "\n----------", 10);
        // usleep(1e2);
        // write(1, CLEAR, strlen(CLEAR));
    }
}
