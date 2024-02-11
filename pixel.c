#include "ftascii.h"

void pix_set(Pixel* p, int size)
{
    // f0 90 85 82
    char color[5] = RED;
    char unicode[4] = {0xf0, 0x90, 0x85, 0x82};

    for (int i = 0; i < size; i++, p++) // Increment p in each iteration
    {
        p->data.unicode = unicode;
        p->data.color = color;
    }   
}

void assign_pix_buff(char* buffer, Pixel* pixels, int size)
{
    for (int i = 0; i < size; i++, pixels++)
    {
        buffer[i * 8] = pixels->data.unicode[0];
        buffer[i * 8 + 1] = pixels->data.unicode[1];
        buffer[i * 8 + 2] = pixels->data.unicode[2];
        buffer[i * 8 + 3] = pixels->data.unicode[3];
        buffer[i * 8 + 4] = pixels->data.color[0];
        buffer[i * 8 + 5] = pixels->data.color[1];
        buffer[i * 8 + 6] = pixels->data.color[2];
        buffer[i * 8 + 7] = pixels->data.color[3];
    }   
}
