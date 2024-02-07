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

    // Create pixels
    Pixel* pixels = malloc(sizeof(Pixel) * t->size);
    if (!pixels) {
        perror("Memory allocation failed");
        exit(1);
    }

    for (int i = 0; i < t->size; i++) {
        pixels[i] = create_pixel(RED, "z");
    }

    // Put pixels in buffer
    for (int i = 0; i < t->size; i++) {
        t->buffer[i] = build_pixel(pixels[i]);
    }

    // Calculate total size and copy buffer to out
    size_t total_size = 0;
    for (int i = 0; i < t->size; i++) {
        total_size += strlen(t->buffer[i]);
    }

    char *out = malloc(total_size + 1); // Include space for null terminator
    if (!out) {
        perror("Memory allocation failed");
        exit(1);
    }

    int offset = 0;
    for (int i = 0; i < t->size; i++) {
       size_t len = strlen(t->buffer[i]);
       memcpy(out + offset, t->buffer[i], len);
       offset += len;
    }

    // Null-terminate out
    out[offset] = '\0';

    // Assign out to buffer
    for (int i = 0; i < t->size; i++) {
        t->buffer[i] = out;
    }

    free(pixels); // Free pixels array
}
