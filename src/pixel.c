#include "ftascii.h"
#include <wchar.h>

int wcwidth(const wchar_t wc);

void fill_pixel(Pixel* pixels,char* color, char* uni, int i)
{
    wchar_t wide_char;
    
    mbtowc(&wide_char, uni, 4);
    
    int width = wcwidth(wide_char);

    if (width == -1) return;

    if (width == 1) {
        pixels[i].uni = &uni[0];
        pixels[i].color = color;
    } else if(width >= 2) {
        pixels[i].uni = &uni[0];
        pixels[i].color = color;
    }
}

void putpix(Pixel* pixels, char* color, char* uni)
{
    pixels->color = color;
    pixels->uni = uni;
}

void assign_pix_buff(char* buffer, Pixel* pixels, int size)
{
    for (int i = 0; i < size; i++)
    {
        int col_l = strlen(pixels->color);
        int uni_l = strlen(pixels->uni);

        // Copy color data
        memcpy(buffer, pixels->color, col_l);
        buffer += col_l;

        // Copy Unicode data
        int j;
        for (j = 0; j < uni_l; j++) {
            if (pixels->uni[j] == '\0') {
                break;
            }
            *buffer++ = pixels->uni[j];
        }
        pixels++;
    }   
}

// char* hex_to_unicode(int hex)
// {
//     char *str = malloc(4);
//     str[0] = hex >> 24;
//     str[1] = hex >> 16;
//     str[2] = hex >> 8;
//     str[3] = hex;
//     return str;
// }

/* int str_to_hex(char *str)
{
    int hex = 0;
    hex = (str[0] << 24) | (str[1] << 16) | (str[2] << 8) | str[3];
    return hex;
} */

// char* uni_to_str(Pixel uni)
// {
//     char *str = malloc(10);
//     str[0] = uni.color[0];
//     str[1] = uni.color[1];
//     str[2] = uni.color[2];
//     str[3] = uni.color[3];
//     str[4] = uni.color[4];
//     str[6] = uni.uni[0];
//     str[7] = uni.uni[1];
//     str[8] = uni.uni[2];
//     str[9] = uni.uni[3];
//     return str;
// }
