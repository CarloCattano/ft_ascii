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


/* void audio_stats(float *fft_values, int size) */
/* { */
/* 	float max = 0; */
/* 	float min = 0; */
/* 	float avg = 0; */
/* 	float cur = 0; */

/* 	for (int i = 0; i < size; i++) */
/* 	{ */
/* 		if (fft_values[i] > max) */
/* 			max = fft_values[i]; */
/* 		if (fft_values[i] < min) */
/* 			min = fft_values[i]; */
/* 		avg += fft_values[i]; */
/*     } */


    /* for (int i = 0; i < size; i++) */
    /* { */
    /*     if ( i > 5) */
    /*     { */
    /*         cur = fft_values[i]; */
    /*         printf("Max:\t%.2f\t", max); */
    /*         printf("Min:\t%.2f\t", min); */
    /*         printf("Avg:\t%.2f\n", avg / size); */
    /*     } */
    /* } */
/* } */
