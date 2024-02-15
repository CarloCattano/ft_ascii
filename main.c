#include "ftascii.h"
#include <locale.h>
#include <time.h>

int main(int ac, char *av[])
{
    (void)av;
    if (ac > 1) {
        printf("Usage: %s\n", av[0]);
        return 1;
    }

    ft_ascii();

    return 0;
}
