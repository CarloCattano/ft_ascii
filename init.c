#include "ftascii.h"

void init_term(term_t *t)
{
    // Initialize frame, clear, delay
    t->frame = 1;
    t->clear = 0;
    t->delay = 1e4;

    // Allocate memory for buffer and buffer_copy
    t->buffer = (char *)calloc(t->size * sizeof(Pixel) , sizeof(char*));
    t->buffer_copy = (char *)calloc(t->size * sizeof(Pixel), sizeof(char*));
    t->pixels = (Pixel *)calloc(t->size, sizeof(Pixel));

    if (!t->buffer || !t->buffer_copy || !t->pixels) {
        perror("Terminal allocation failed");
        exit(1);
    }
    
    pix_set(t->pixels, t->size);
}
