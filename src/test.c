#include <stdio.h>
#include <wchar.h>
#include <locale.h>

int wcwidth(const wchar_t wc);

int main() {
    
    setlocale(LC_ALL, ""); // Set the locale to the system's default
    
    wchar_t wide_char = L'ش';// Example wide character, Euro symbol
    int width = wcwidth(wide_char); // Get the width of the wide character
 
    if (width != -1) {
        printf("Width of wide character '%lc' is %d columns.\n", wide_char, width);
    } else {
        printf("Wide character '%lc' is not printable.\n", wide_char);
    }

    return 0;
}

