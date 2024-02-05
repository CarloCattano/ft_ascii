#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#define COL "\033[0;"
#define RESET "\033[0m"
#define RED "31m"
#define GREEN "32m"
#define YELLOW "33m"
#define BLUE "34m"
#define MAGENTA "35m"
#define CYAN "36m"
#define WHITE "37m"

// UNICODE BLOCKS
// █ U+2588
// ▓ U+2593
// ▒ U+2592
// ░ U+2591
// ░▒▓█

// UNICODE SYMBOLS
// ▲ U+25B2 // ○ U+25CB
// ● U+25CF
// ★ U+2605 // ☆ U+2606 // ♠ U+2660 // ♣ U+2663 // ♥ U+2665 // ♦ U+2666 
// // ♪ U+266A // ♫ U+266B // ♯ U+266F // ♭ U+266D // ▼ U+25BC // ◄ U+25C4 // ► U+25BA // ◊ U+25CA
// array of 13 unicode symbols 
#define UNICODE "▲○●♥♦▼◄►◊▓▒░█"
#define UNICODE2 "˹˼˽˾◢◣◤◥◠◡◜◝◞◟"
#define UNICODE3 "▀▁▂▃▄▅▆▇█"

#define UNICODE4 "▉▊▋▌▍▎▏▐"

#define UNICODE5 "▔▕▖▗▘▙▚▛▜▝▞▟"

#define UNICODE6 "□■▢▣▤▥▦▧▨▩▪▫▬▭▮▯"
#define UNICODE7 "▰▱▲△▴▵▶▷▸▹►▻▼▽▾▿"
#define UNICODE8 "◀◁◂◃◄◅◆◇◈◉◊○◌◍◎"
#define UNICODE9 "●◐◑◒◓◔◕◖◗◘◙◚◛◜◝◞◟"
#define UNICODE10 "◠◡◢◣◤◥◦◧◨◩◪◫◬◭◮◯"
#define UNICODE11 "◰◱◲◳◴◵◶◷◸◹◺◻◼◽◾◿"
#define UNICODE12 "⬒⬓⬔⬕⬖⬗⬘⬙⬚⬛⬜⬝⬞⬟⬠⬡"
#define UNICODE13 "⬢⬣⬤⬥⬦⬧⬨⬩⬪⬫⬬⬭⬮⬯⬰⬱"
#define UNICODE14 "⬲⬳⬴⬵⬶⬷⬸⬹⬺⬻⬼⬽⬾⬿⭀⭁"
#define UNICODE16 "⭒⭓⭔⭕⭖⭗⭘⭙⭚⭛⭜⭝⭞⭟⭠⭡"
#define UNICODE17 "⭢⭣⭤⭥⭦⭧⭨⭩⭪⭫⭬⭭⭮⭯⭰⭱"
#define UNICODE19 "⮂⮃⮄⮅⮆⮇⮈⮉⮊⮋⮌⮍⮎⮏⮐⮑"
#define UNICODE21 "⮢⮣⮤⮥⮦⮧⮨⮩⮪⮫⮬⮭⮮⮯⮰⮱"
#define UNICODE23 "⯂⯃⯄⯅⯆⯇⯈⯉⯊⯋⯌⯍⯎⯏⯐⯑"
#define UNICODE29 "ⰲⰳⰴⰵⰶⰷⰸⰹⰺⰻⰼⰽⰾⰿⱀⱁ"

// struct holding the data that contains a char buffer, a color, and a position
// the colors are going to be escape sequences appended to the buffer
typedef struct s_pixel
{
    char    *buffer;
    char    *color;
    int     x;
    int     y;
}           t_pixel;

t_pixel create_pixel(char* color, char* str)
{
    t_pixel     pixel;
    
    pixel.buffer = str;
    pixel.color = color;
    return (pixel);
}

char* output_buffer(t_pixel pixel)
{
    char *output;
    output = (char*)malloc(strlen(pixel.color) + strlen(pixel.buffer) + strlen(RESET) + 1);
    strcpy(output, pixel.color);
    strcat(output, pixel.buffer);
    strcat(output, RESET);
    return (output);
}

int main(int ac, char **av)
{
    t_pixel screen[8][14];
    int i = 0;
    int j = 0;
    while (i < 8)
    {
        j = 0;
        while (j < 8)
        {
            screen[i][j] = create_pixel(COL RED, UNICODE2); 
            j++;
        }
        i++;
    }

    i = 0;
    j = 0;
    while (i < 8)
    {
        j = 0;
        while (j < 8)
        {
            write(1, output_buffer(screen[i][j]), strlen(output_buffer(screen[i][j])));
            j++;
        }
        write(1, "\n", 1);
        i++;
    }

    return (0);
}

