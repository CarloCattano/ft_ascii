#include <unistd.h>

int main()
{
    write(1, "Hello,\0 world!\n", 15);
}
