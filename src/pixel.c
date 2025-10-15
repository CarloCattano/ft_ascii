#include "ftascii.h"
#include <wchar.h>

int wcwidth(const wchar_t wc);

void fill_pixel(Pixel* pixels, char* color, char* uni, int i) {
	wchar_t wide_char;

	int ret = mbtowc(&wide_char, uni, 4);

	if (ret == -1 || !ret)
		return;

	int width = wcwidth(wide_char);

	if (width == -1)
		return;

	if (width == 1) {
		pixels[i].uni = &uni[0];
		pixels[i].color = color;
	} else if (width >= 2) {
		pixels[i].uni = &uni[0];
		pixels[i].color = color;
	}
}

void putpix(Pixel* pixels, char* color, char* uni) {
	pixels->color = color;
	pixels->uni = uni;
}

void assign_pix_buff(char* buffer, Pixel* pixels, int size) {
	for (int i = 0; i < size; i++) {
		int col_l = strlen(pixels->color);
		int uni_l = strlen(pixels->uni);

		// Copy color data
		memcpy(buffer, pixels->color, col_l);
		buffer += col_l;

		// Copy Unicode data
		int j;
		for (j = 0; j < uni_l; j++) {
			if (pixels->uni[j] == '\0') {
				break;
			}
			*buffer++ = pixels->uni[j];
		}
		pixels++;
	}
}
