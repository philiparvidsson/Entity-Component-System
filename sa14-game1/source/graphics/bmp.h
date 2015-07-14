#ifndef bmp_h_
#define bmp_h_

#include <stdint.h>

typedef struct {
    uint8_t r, g, b, reserved;
} bitmapColorT;

typedef struct {
    uint32_t num_bytes;
    int width;
    int height;
    uint16_t num_planes;
    uint16_t bit_count;
    uint32_t compression;
    uint32_t image_num_bytes;
    int x_pixels_per_meter;
    int y_pixels_per_meter;
    uint32_t num_used_colors;
    uint32_t num_important_colors;
    bitmapColorT pixels[1];
} bitmapHeaderT;

#endif // bmp_h_
