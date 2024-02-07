#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define   CLEAR     "\033[H"
#define   NOMOUSE   "\033[?25l"

#define COL "\033[0;"
#define RESET "\033[0m"
#define RED "31m"
#define GREEN "32m"
#define YELLOW "33m"
#define BLUE "34m"
#define MAGENTA "35m"
#define CYAN "36m"
#define WHITE "37m"

#define UNICODE "▲○●♥♦▼◄►◊▓▒░█"
#define UNICODE2 "˹˼˽˾◢◣◤◥◠◡◜◝◞◟"
#define UNICODE3 "▀▁▂▃▄▅▆▇█"
#define UNICODE4 "▉▊▋▌▍▎▏▐"
#define UNICODE5 "▔▕▖▗▘▙▚▛▜▝▞▟"

typedef struct s_pixel
{
    char    *c;
    char    *color;

}           t_pixel;

t_pixel create_pixel(char* color, char* str)
{
    t_pixel     pixel;
    
    pixel.color = color;
    pixel.c = str;
    
    return (pixel);
}

void change_pixel(t_pixel* p, char* c, char* color)
{
    p->color = color;
    p->c = c;
}

char* build_pixel(t_pixel pixel)
{
    char* out = malloc(strlen(pixel.color) + strlen(pixel.c));

    strcpy(out, pixel.color);
    strcat(out, pixel.c);
    return (out);
}

int main()
{ 
    int w = 32;
    int h = 8;
    int size = w*h;
    char** buffer = calloc(size, sizeof(t_pixel));
    int i = 0;
    
    t_pixel* pixels = malloc(sizeof(t_pixel) * size);
    
    for(int i = 0;i<size; i++)
    {
        pixels[i] = create_pixel(COL RED, "a");
    }
    
    // change pixels
    for (int i = 0; i < size; i++)
    {
        if (i % 3)
            change_pixel(&pixels[i], "@", COL YELLOW);
    }
    
    // asign pixels to buffer
    for (int i = 0; i < size; i++)
    {
        buffer[i] = build_pixel(pixels[i]);
    }
    
    while (buffer[i] != NULL) {
        size += strlen(buffer[i]);
        i++;
    }

    char *out = malloc(size + i);
    int offset = 0;
    i = 0;
    
    while (buffer[i] != NULL) {
       size_t len = strlen(buffer[i]); 
       memcpy(out + offset, buffer[i], len);
       offset += len;
       i++;
    }

 write(1, out, size + i - 1);
}
