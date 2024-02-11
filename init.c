#include "ftascii.h"

void init_term(term_t *t)
{
    // Initialize frame, clear, delay
    t->frame = 1;
    t->clear = 0;
    t->delay = 1e2;
    
    // check term init
    if (t->size < 1){
        printf("Terminal size is invalid\n");
        exit(1);
    }

    t->pixels = (Pixel*)malloc(sizeof(Pixel) * t->size);
    t->buffer = (char*)malloc(sizeof(char) * t->size * 8);

    if (t->pixels == NULL || t->buffer == NULL){
          printf("Memory allocation failed\n");
          exit(1);
    }
    pix_set(t->pixels, t->size);
    fill_pixels(t->pixels, t->size, GREEN, "😀");
}
