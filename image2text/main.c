/**
 * Adapted from Daniel Shifmann image/video to ascii originally in p5.js
 *  by Carlo Cattano 2023
 *
 *  uses stb_image.h and stb_image_resize.h
 *  to be used in ft_ascii
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"

#define CLS "\033[2J"

// map 0 to 255 to 0 to 1
#define MAP(x, in_min, in_max, out_min, out_max)                               \
  ((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)

// TODO
/** Array of color escape sequeces
 * to map to the color retrieved from the image pixels
 * In this way , we add color to each letter as a prepended  escape sequence
 * Thus changing our buffer size to 5 chars for escape sequence and 1 char for
 * the letter
 */

// Colors array with escape sequences
char *colors[] = {
    "\033[0;30m", // black
    "\033[0;31m", // red
    "\033[0;32m", // green
    "\033[0;33m", // yellow
    "\033[0;34m", // blue
    "\033[0;35m", // magenta
    "\033[0;36m", // cyan
    "\033[0;37m", // white
};

// -------------------------------

int main(int ac, char **av) {

  if (ac != 3) {
    printf("Usage: %s <image> <size>\n", av[0]);
    exit(1);
  }

  int width, height, channels;
  unsigned char *img = stbi_load(av[1], &width, &height, &channels, 0);

  if (img == NULL) {
    printf("Error in loading the image\n");
    exit(1);
  }

  int new_width = atoi(av[2]);
  int new_height = new_width;

  unsigned char *new_img = malloc(new_width * new_height * channels);

  stbir_resize_uint8_linear(img, width, height, 0, new_img, new_width,
                            new_height, 0, channels);

  if (new_img == NULL) {
    printf("Error in resizing the image\n");
    free(img);
    exit(1);
  }

  char buffer[new_width * new_height];

  const char density[] = "@/\\O1?oc^-,'.        ";

  // TODO colors buffer to store the color escape sequence and the letter
  // char colors_buffer[new_width * new_height * 6];

  for (int i = 0; i < new_width; i++) {
    for (int j = 0; j < new_height; j++) {
      int index = (i * new_width + j) * channels;
      float r = new_img[index];
      float g = new_img[index + 1];
      float b = new_img[index + 2];

      float avg = (r + g + b) / 3.0;
      int len = strlen(density);
      char c = density[(unsigned int)MAP(avg, 255, 0, 0, len)];
      buffer[i * new_width + j] = c;
    }
  }

  write(1, CLS, 4); // clear screen

  for (int i = 0; i < new_width; i++) {
    for (int j = 0; j < new_height; j++) {
      char c = buffer[i * new_width + j];
      write(1, &c, 1);
    }
    write(1, "\n", 1);
  }

  stbi_image_free(img);
  free(new_img);
  return 0;
}
