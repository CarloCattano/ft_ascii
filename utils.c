#include "ftascii.h"

int compare_val_in_buffers(term_t *t, int i)
{
	if (t->buffer[i] != t->buffer_copy[i])
		return 0;
	return 1;
}

void add_randomness_to_movement(term_t *t)
{
    t->player.dx = (rand() % 3) - 1;
    t->player.dy = (rand() % 3) - 1;
}

void copy_last_buffer(term_t *t)
{
	t->buffer_copy = memcpy(t->buffer_copy, t->buffer, t->size);
}

// free a gicen buffer

void free_buffer(term_t *t)
{
    free(t->buffer);
    free(t->buffer_copy);
}
