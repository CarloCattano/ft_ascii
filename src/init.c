#include "ftascii.h"

void init_term(term_t *t)
{
    t->frame = 1;
    t->clear = 0;
    t->buffer_size = t->size;

	// Check if terminal size is valid
	if (t->size < 1) {
		printf("Terminal size is invalid\n");
		exit(1);
	}
	t->pixels = (Pixel*)malloc(sizeof(Pixel) * t->size);
	if (t->pixels == NULL) {
		printf("Memory allocation failed for pixels\n");
		exit(1);
	}
	// Allocate buffer - final char output to the terminal
	t->buffer = (char*)malloc(sizeof(char) * t->size *
							  8); // 8 or 9 ?? color (5) + uni(3)
	if (t->buffer == NULL) {
		printf("Memory allocation failed for buffer\n");
		free(t->pixels);
		exit(1);
	}

	// Initialize buffer
	memset(t->buffer, '.', t->size);
	t->draw = true;
}
