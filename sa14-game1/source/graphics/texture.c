/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "texture.h"

#include "base/common.h"
#include "base/file_io.h"
#include "graphics/bmp.h"
#include "graphics/graphics.h"
#include "graphics/rendertarget.h"
#include "math/vector.h"

#include <stdlib.h>

#include <GL/glew.h>

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

// OpenGL guarantees at least 16 which is also probably more than enough.
#define MaxTextures (16)

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

struct textureT {
    GLuint id;
    bool repeat;
    bool multisample;

    int width;
    int height;
};

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

static textureT* active_textures[MaxTextures] = { 0 };

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static textureT* createTex(bool multisample) {
    textureT* tex = malloc(sizeof(textureT));

    tex->multisample = multisample;

    glGenTextures(1, &tex->id);

    textureT* old_tex = useTexture(tex, 0);

    glTexParameteri(texTarget(tex), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(texTarget(tex), GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    setTextureRepeat(tex, false);

    useTexture(old_tex, 0);
}

static textureT* loadTextureFromBMP(const void* bmp_data) {
    bitmapHeaderT* bitmap      = bmp_data;
    textureT*      texture     = createTexture(bitmap->width, bitmap->height);
    textureT*      old_texture = useTexture(texture, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, bitmap->width, bitmap->height, 0, GL_BGR, GL_UNSIGNED_BYTE, bitmap->pixels);

    useTexture(old_texture, 0);

    return (texture);
}

static inline GLenum texTarget(textureT* tex) {
    if (tex->multisample)
        return (GL_TEXTURE_2D_MULTISAMPLE);

    return (GL_TEXTURE_2D);
}

textureT* createMultisampledTexture(void) {
    return (createTex(true));
}

textureT* createTexture(void) {
    return (createTex(false));
}

textureT* createTextureFromScreen(void) {
    int width  = screenWidth(),
        height = screenHeight();

    renderTargetT* old_rt  = useRenderTarget(NULL);
    textureT*      tex     = createTexture(width, height);
    textureT*      old_tex = useTexture(tex, 0);

    glCopyTexImage2D(texTarget(tex), 0, GL_RGBA8, 0, 0, width, height, 0);

    useTexture     (old_tex, 0);
    useRenderTarget(old_rt);

    return (tex);
}

textureT* createWhiteTexture(void) {
    // Red, green and blue color components. All set to 1.0 to get white color.
    float data[] = { 1.0f, 1.0f, 1.0f };

    textureT* tex     = createTexture(1, 1);
    textureT* old_tex = useTexture(tex, 0);

    glTexImage2D(texTarget(tex), 0, GL_RGBA8, 1, 1, 0, GL_RGB, GL_FLOAT, data);

    useTexture(old_tex, 0);

    return (tex);
}

textureT* loadTextureFromFile(const void* file_name) {
    textureT* tex = NULL;

    char* data = readFile(file_name);

    if (!data)
        error("Could not read file '%s'", file_name);

    if (data[0]=='B' && data[1]=='M') {
        // Bitmap file. The BMP file header is 14 bytes and not really important
        // for loading it into a texture, so we skip past it.
        tex = loadTextureFromBMP(data+14);
    }

    free(data);

    if (!tex)
        error("Could not load texture from file '%s'", file_name);

    textureT* old_tex = useTexture(tex, 0);

    glGenerateMipmap(texTarget(tex));

    useTexture(old_tex, 0);

    return (tex);
}

textureT* loadTextureFromMemory(const void* data, int format) {
    if (format == TexFormatBMP)
        return loadTextureFromBMP(data);

    return (NULL);
}

textureT* useTexture(textureT* tex, int index) {
    assert(0 <= index && index < MaxTextures);

    textureT* old_tex = active_textures[index];

    if (tex == old_tex)
        return (old_tex);

    active_textures[index] = tex;

    glActiveTexture(GL_TEXTURE0 + index);

    if (tex)
        glBindTexture(texTarget(tex), tex ? tex->id : 0);
    else 
        glBindTexture(texTarget(old_tex), 0);

    return (old_tex);
}

void freeTexture(textureT* tex) {
    // @To-do: Is this pointless?
    for (int i = 0; i < MaxTextures; i++) {
        if (active_textures[i] == tex)
            useTexture(NULL, i);
    }

    glDeleteTextures(1, &tex->id);
    free(tex);
}

bool getTextureRepeat(const textureT* tex) {
    return (tex->repeat);
}

void setTextureRepeat(textureT* tex, bool value) {
    tex->repeat = value;

    textureT* old_tex = useTexture(tex, 0);

    GLint wrap_param = value ? GL_REPEAT : GL_CLAMP_TO_EDGE;
    glTexParameteri(texTarget(tex), GL_TEXTURE_WRAP_S, wrap_param);
    glTexParameteri(texTarget(tex), GL_TEXTURE_WRAP_T, wrap_param);

    useTexture(old_tex, 0);
}

vec2 getTextureSize(const textureT* tex) {
    return ((vec2) { tex->width, tex->height });
}
