#include "bmp.h"

#include "graphics/texture.h"

#include <GL/glew.h>

#pragma pack(1)
typedef struct {
    short magic_number;

    // @To-do: Implement this some day.
    char reserved[12];

    uint32_t num_bytes;
    int      width;
    int      height;
    uint16_t num_planes;
    uint16_t bit_count;
    uint32_t compression;
    uint32_t image_num_bytes;
    int      x_pixels_per_meter;
    int      y_pixels_per_meter;
    uint32_t num_used_colors;
    uint32_t num_important_colors;
    struct { uint8_t r, g, b; } pixels[1];
} bitmapHeaderT;

textureT* loadBMP(const void* data) {
    bitmapHeaderT* bmp     = data;
    textureT*      tex     = createTexture();
    textureT*      old_tex = useTexture(tex, 0);

    if (bmp->magic_number != 0x4d42)
        error("invalid bitmap data");

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA8,
                 bmp->width,
                 bmp->height,
                 0,
                 GL_BGR,
                 GL_UNSIGNED_BYTE,
                 bmp->pixels);

    useTexture(old_tex, 0);

    return (tex);
}
