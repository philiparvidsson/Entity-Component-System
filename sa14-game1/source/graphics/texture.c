#include "texture.h"

#include "base/common.h"
#include "base/file_io.h"

#include "graphics/bmp.h"
#include "graphics/graphics.h"

#include <GL/glew.h>
#include <stdlib.h>

// OpenGL guarantees at least 16 which is also probably more than enough.
#define MaxTextures (16)

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

textureT* createTextureFromBMP(const void* bmp_data) {
    textureT*      texture     = createTexture();
    bitmapHeaderT* bitmap      = bmp_data;
    textureT*      old_texture = useTexture(texture, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, bitmap->width, bitmap->height, 0, GL_BGR, GL_UNSIGNED_BYTE, bitmap->pixels);
    glGenerateMipmap(texture->id);

    useTexture(old_texture, 0);

    return (texture);
}

textureT* loadTextureFromFile(const void* file_name) {
    textureT* texture = NULL;

    char* data = readFile(file_name);

    if (data[0]=='B' && data[1]=='M') {
        // Bitmap file. The BMP file header is 14 bytes and not really important
        // for loading it into a texture, so we skip past it.
        texture = createTextureFromBMP(data+14);
    }

    free(data);

    if (!texture)
        error("Could not load texture");

    return (texture);
}

textureT* createTextureFromScreen(void) {
    textureT* texture     = createTexture();
    textureT* old_texture = useTexture(texture, 0);

    int width  = screenWidth(), height = screenHeight();
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 0, 0, width, height, 0);

    useTexture(old_texture, 0);

    return (texture);
}

textureT* createWhiteTexture(void) {
    // Red, green and blue color components. All set to 1.0 to get white color.
    float data[] = { 1.0f, 1.0f, 1.0f };

    textureT* texture     = createTexture();
    textureT* old_texture = useTexture(texture, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, 1, 1, 0, GL_RGB, GL_FLOAT, data);

    useTexture(old_texture, 0);
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
