#include "ftascii.h"

void init_term(term_t *t)
{
    // Initialize frame, clear, delay
    t->frame = 1;
    t->clear = 0;
    t->delay = 1e3;

    // Allocate memory for buffer and buffer_copy
    t->buffer = (char **)calloc(t->size, sizeof(char*));
    t->buffer_copy = (char **)calloc(t->size, sizeof(char*));

    if (!t->buffer || !t->buffer_copy) {
        perror("Terminal allocation failed");
        exit(1);
    }

    
    // Initialize pixels
    Pixel* pixels = (Pixel *)calloc(t->size, sizeof(Pixel));
    if (!pixels) perror("Pixel allocation failed"),exit(1);
    
    pix_set(pixels, t->size);


}
