#ifndef bmp_h_
#define bmp_h_

#include "graphics/texture.h"

#include <stdint.h>

#pragma pack(1)
typedef struct {
    uint8_t r, g, b;
} bitmapColorT;

#pragma pack(1)
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

#pragma pack(1)
typedef struct {
    // @To-do: Implement this some day.
    char reserved[14];

    bitmapHeaderT header;
} bitmapFileHeaderT;

textureT* loadBMP(const void* data);

#endif // bmp_h_
