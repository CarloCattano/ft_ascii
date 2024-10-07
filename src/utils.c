#include "ftascii.h"

/* compare curr buffer with last buffer in order to implement */
/* rendering only when necessary */

int compare_val_in_buffers(term_t *t, int i)
{
    if (t->buffer[i] != t->buffer_copy[i])
        return 1;
    return 0;
}

// TODO : compare buffers
void copy_last_buffer(term_t *t)
{
    t->buffer_copy = memcpy(t->buffer_copy, t->buffer, (size_t)t->size);
}

void free_all(term_t *t)
{   
    if (t == NULL)
        return;

    if (t->pixels != NULL)
    {
        free(t->pixels);
        t->pixels = NULL; // Set pointer to NULL after freeing to prevent double free
    }

    if (t->buffer != NULL) {
        free(t->buffer);
        t->buffer = NULL; // Set pointer to NULL after freeing to prevent double free
    }
    free(t);
}

