#include "ftascii.h"

/* compare curr buffer with last buffer in order to implement */
/* rendering only when necessary */

void free_all(term_t* t) {
	if (t == NULL)
		return;

	if (t->pixels != NULL) {
		free(t->pixels);
		t->pixels =
			NULL; // Set pointer to NULL after freeing to prevent double free
	}

	if (t->buffer != NULL) {
		free(t->buffer);
		t->buffer =
			NULL; // Set pointer to NULL after freeing to prevent double free
	}
	free(t);
}
