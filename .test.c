#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define   RED       "\033[31m"
#define   GREEN     "\033[32m"
#define   YELLOW    "\033[33m"

typedef struct s_uni {
     char*   uni;
     char*   color;
}    t_uni;

char* hex_to_unicode(int hex)
{
    char *str = malloc(4);
    str[0] = hex >> 24;
    str[1] = hex >> 16;
    str[2] = hex >> 8;
    str[3] = hex;
    return str;
}

int str_to_hex(char *str)
{
    // construct the hex out of the 4 bytes of the string
    int hex = 0;
    hex = (str[0] << 24) | (str[1] << 16) | (str[2] << 8) | str[3];
    return hex;
}

char* uni_to_str(t_uni uni)
{
    char *str = malloc(10);
 
    str[0] = uni.color[0];
    str[1] = uni.color[1];
    str[2] = uni.color[2];
    str[3] = uni.color[3];
    str[4] = uni.color[4];
    str[5] = uni.color[5];
    str[6] = uni.uni[0];
    str[7] = uni.uni[1];
    str[8] = uni.uni[2];
    str[9] = uni.uni[3];

    return str;
}

t_uni make_uni_from_str(char *str)
{
    t_uni uni;

    uni.uni = str;
    uni.color = GREEN;

    return uni;
}

int main()
{
    t_uni uni;
    
    uni.uni = calloc(4, sizeof(char));
    uni.color = calloc(6, sizeof(char));
    t_uni uni2 = make_uni_from_str("😀");

    int hex = 0xf0908599;
    char *str = hex_to_unicode(hex);
    char *color_code = malloc(6);

// 😀 pass unicode through
    uni2.color = GREEN;
    char *out2 = uni_to_str(uni2);
 
    write(1, out2, 10);
    write(1, "\n", 1);
    color_code = RED;
    uni.color = color_code;
    uni.uni = str;

    char *out = uni_to_str(uni);
    write(1, out, 10);
    
    return 0;
}
