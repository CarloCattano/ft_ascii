#include <unistd.h>
#include <stdio.h>
#include <string.h>

typedef struct s_uni {
    char a;
    char b;
    char c;
    char d;
} t_uni;

int main()
{
    
        t_uni uni;

    /* for(int i = 0x00;i< 0xbb;i++) */
    /* { */
        /* E2 0x98 0xBA */
        /* 0xEE 0x84 0x9C */ 
        /* EE 90 A0 */
        uni.a = 0xA0;
        uni.b = 0x90;
        uni.c = 0xEE;
        uni.d = 0x00;
// 0x0000263A
        write(1, &uni.a,1);
        write(1, &uni.b,1);
        write(1, &uni.c,1);
        write(1, &uni.d,1);
    /* } */
    // write emoji with write
    /* int count = 0; */
    /* count = write(1, "😀\n", 4); */
    /* printf("write count: %d\n", count); */
    /* printf("write count: %x\n", "😀"); */
    /* printf("write count: %u\n", "😀"); */
    return 0;
}
