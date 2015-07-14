#include "texture.h"

#include "base/common.h"
#include "base/file_io.h"

#include "graphics/graphics.h"

#include <GL/glew.h>
#include <windows.h>

#include <stdlib.h>

#define MaxTextures (32)

struct textureT {
    GLuint id;
    bool repeat;
};

textureT* active_textures[MaxTextures] = { 0 };

static textureT* createTexture(void) {
    textureT* tex = malloc(sizeof(textureT));

    glGenTextures(1, &tex->id);
    textureT* old_tex = useTexture(tex, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    setTextureRepeat(tex, false);
    useTexture(old_tex, 0);

    return (tex);
}

void freeTexture(textureT* texture) {
    glDeleteTextures(1, &texture->id);
    free(texture);
}

textureT* useTexture(textureT* texture, int index) {
    assert(0 <= index && index < MaxTextures);

    textureT* old_tex = active_textures[index];
    active_textures[index] = texture;

    glActiveTexture(GL_TEXTURE0+index);
    glBindTexture(GL_TEXTURE_2D, texture ? texture->id : 0);

    return (old_tex);
}

textureT* loadTextureBMP(const void* bmp_data) {
    textureT*   tex     = createTexture();
    BITMAPINFO* bminfo  = bmp_data;
    textureT*   old_tex = useTexture(tex, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, bminfo->bmiHeader.biWidth, bminfo->bmiHeader.biHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, bminfo->bmiColors);
    glGenerateMipmap(tex->id);

    useTexture(old_tex, 0);

    return (tex);
}

textureT* loadTextureFileBMP(const void* file_name) {
    void* bmp_data = readFile(file_name);
    textureT* tex = loadTextureBMP(((char*)bmp_data)+14);
    free(bmp_data);
    return (tex);
}

textureT* createTextureFromScreen(void) {
    textureT* tex = createTexture();
    useTexture(tex, 0);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 0, 0,
                     screenWidth(), screenHeight(), 0);

    return (tex);
}

bool getTextureRepeat(textureT* texture) {
    return (texture->repeat);
}

void setTextureRepeat(textureT* texture, bool value) {
    texture->repeat = value;

    textureT* old_tex = useTexture(texture, 0);
    GLint wrap_param = value ? GL_REPEAT : GL_CLAMP_TO_EDGE;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_param);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_param);
    useTexture(old_tex, 0);
}
