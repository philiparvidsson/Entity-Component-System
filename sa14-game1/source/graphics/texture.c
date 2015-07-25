/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "texture.h"

#include "base/common.h"
#include "base/fileio.h"
#include "graphics/graphics.h"
#include "graphics/io/bmp.h"
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

static inline GLenum texTarget(textureT* tex) {
    if (tex->multisample)
        return (GL_TEXTURE_2D_MULTISAMPLE);

    return (GL_TEXTURE_2D);
}

textureT* createMultisampledTexture(void) {
    textureT* tex = malloc(sizeof(textureT));

    glGenTextures(1, &tex->id);
    tex->multisample = true;
    tex->repeat      = false;

    return (tex);
}

textureT* createTexture(void) {
    textureT* tex = malloc(sizeof(textureT));

    glGenTextures(1, &tex->id);
    tex->multisample = false;

    textureT* old_tex = useTexture(tex, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    setTextureRepeat(tex, false);

    useTexture(old_tex, 0);

    return (tex);
}

textureT* loadTextureFromFile(const void* file_name) {
    textureT* tex = NULL;

    char* data = ioReadFile(file_name);

    if (!data)
        error("could not read file '%s'", file_name);

    loadTextureFromMemory(data);

    free(data);

    if (!tex)
        error("could not load texture from file '%s'", file_name);

    return (tex);
}

textureT* loadTextureFromMemory(const char* data) {
    textureT* tex = NULL;

    if (data[0]=='B' && data[1]=='M')
        tex = loadBMP(data);

    if (!tex)
        return (NULL);

    textureT* old_tex = useTexture(tex, 0);

    glGenerateMipmap(texTarget(tex));

    useTexture(old_tex, 0);

    return (tex);
}

void loadTextureFromScreen(textureT* tex) {
    int width  = screenWidth(),
        height = screenHeight();

    renderTargetT* old_rt  = useRenderTarget(NULL);
    textureT*      old_tex = useTexture(tex, 0);

    glCopyTexImage2D(texTarget(tex), 0, GL_RGBA8, 0, 0, width, height, 0);

    useTexture     (old_tex, 0);
    useRenderTarget(old_rt);
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

textureT* whiteTexture(void) {
    static textureT* white_tex = NULL;

    if (!white_tex) {
        white_tex = createTexture();

        textureT* old_tex = useTexture(white_tex, 0);
        float     data[]  = { 1.0f, 1.0f, 1.0f };
        glTexImage2D(texTarget(white_tex), 0, GL_RGBA8, 1, 1, 0, GL_RGB, GL_FLOAT, data);
        useTexture(old_tex, 0);
    }

    return (white_tex);
}
