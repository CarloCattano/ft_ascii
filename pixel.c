#include "ftascii.h"

void change_pixel(Pixel* p, char* c, char* color)
{
    /* (void)c; */

    p->data.unicode[0] = c[0];
    p->data.unicode[1] = c[1];
    p->data.unicode[2] = c[2];
    p->data.unicode[3] = c[3];

    p->data.color[0] = color[0];
    p->data.color[1] = color[1];
    p->data.color[2] = color[2];
    p->data.color[3] = color[3];
}

void pix_set(Pixel* p,int size)
{
    for (int i = 0; i < size; i++)
    {
        p->data.unicode[0] = 0xA0;
        p->data.unicode[1] = 0x90;
        p->data.unicode[2] = 0xEE;
        p->data.unicode[3] = 0x00;
    }   
}

void assign_pix_buff(char* buffer, Pixel* pixels, int size)
{
    memset(buffer, 0, size);
    for (int i = 0; i < size; i++)
    {
        buffer[i] = pixels[i].data.unicode[0];
        buffer[i + 1] = pixels[i].data.unicode[1];
        buffer[i + 2] = pixels[i].data.unicode[2];
        buffer[i + 3] = pixels[i].data.unicode[3];
        i += 3;
    }
}

