#include "ftascii.h"

int compare_val_in_buffers(term_t *t, int i)
{
    if (t->buffer[i] != t->buffer_copy[i])
        return 1;
    return 0;
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


void audio_stats(float *fft_values, int size)
{
	float max = 0;
	float min = 0;
	float avg = 0;
	float cur = 0;

	for (int i = 0; i < size; i++)
	{
		if (fft_values[i] > max)
			max = fft_values[i];
		if (fft_values[i] < min)
			min = fft_values[i];
		avg += fft_values[i];
	}
	avg = avg / size;
	cur = fft_values[0];
	printf("COMPUTED max: %.2f \nmin: %.2f\navg: %.2ff\ncur: %.2ff\n", max, min, avg, cur);
}