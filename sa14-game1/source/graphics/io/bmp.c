#include "bmp.h"

#include "graphics/texture.h"

#include <GL/glew.h>

textureT* loadBMP(const void* data) {
    bitmapFileHeaderT* bmp     = data;
    textureT*          tex     = createTexture();
    textureT*          old_tex = useTexture(tex, 0);

    if (bmp->reserved[0]!='B' || bmp->reserved[1]!='M')
        error("Invalid bitmap");

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA8,
                 bmp->header.width,
                 bmp->header.height,
                 0,
                 GL_BGR,
                 GL_UNSIGNED_BYTE,
                 bmp->header.pixels);

    useTexture(old_tex, 0);

    return (tex);
}
