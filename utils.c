#include "ftascii.h"

/* compare curr buffer with last buffer in order to implement */
/* rendering only when necessary */

int compare_val_in_buffers(term_t *t, int i)
{
    if (t->buffer[i] != t->buffer_copy[i])
        return 1;
    return 0;
}

void add_randomness_to_movement(player_t *p)
{
    p->dx = (rand() % 3) - 1;
    p->dy = (rand() % 3) - 1;
}

void copy_last_buffer(term_t *t)
{
	t->buffer_copy = memcpy(t->buffer_copy, t->buffer, t->size);
}

void free_all(term_t *t)
{
    free(t->buffer);
    free(t->buffer_copy);
    for (int i = 0; i < 4; i++) {
        free(t->players[i]);
    }
}

void save_last_frame_to_file(char* filename, char* buffer, int size)
{
    FILE *f = fopen(filename, "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    fwrite(buffer, 1, size, f);
    fclose(f);
}
