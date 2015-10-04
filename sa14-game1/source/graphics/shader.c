/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "shader.h"

#include "base/array.h"
#include "base/common.h"
#include "base/debug.h"
#include "graphics/graphics.h"
#include "graphics/texture.h"
#include "graphics/trimesh.h"

#include <stdlib.h>

#include <GL/glew.h>

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: shaderT
 *
 * Description:
 *   Concrete implementation for the shader program data type. The fields are
 *   mainly related to deep system stuff and so should not be exposed publicly.
 *------------------------------------*/
struct shaderT {
    GLuint  id;      // The shader program identifier, given by OpenGL.
    arrayT* shaders; // The attached shaders. Used to detach and delete them.
};

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

static const shaderT* active_shader = NULL;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static void compileShader(GLenum type, shaderT* shader, const string* source) {
    assert(source != NULL);

    GLuint shader_id = glCreateShader(type);

    glShaderSource(shader_id, 1, &source, NULL);
    glCompileShader(shader_id);

    GLint result;

    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
    GLint info_log_length;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

    if (info_log_length > 1) {
        GLchar *log = malloc(sizeof(GLchar) * info_log_length);
        glGetShaderInfoLog(shader_id, info_log_length, NULL, log);
        warn("\n%s", log);
        free(log);
    }

    if (result == GL_FALSE)
        error("shader failed to compile");

    glAttachShader(shader->id, shader_id);
    glLinkProgram (shader->id);

    glGetProgramiv(shader->id, GL_LINK_STATUS, &result);
    glGetProgramiv(shader->id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 1) {
        GLchar *log = malloc(sizeof(GLchar) * info_log_length);
        glGetProgramInfoLog(shader->id, info_log_length, NULL, log);
        warn("\n%s", log);
        free(log);
    }

    if (result == GL_FALSE)
        error("shader program failed to link");

    arrayAdd(shader->shaders, &shader_id);
}

shaderT* createShader(void) {
    shaderT* shader = malloc(sizeof(shaderT));

    shader->id      = glCreateProgram();
    shader->shaders = arrayNew(sizeof(GLuint));
    return (shader);
}

void deleteShader(shaderT* shader) {
    for (int i = 0; i < arrayLength(shader->shaders); i++) {
        GLuint shader_id = *(GLuint*)arrayGet(shader->shaders, i);
        glDeleteShader(shader_id);
    }

    useShader(NULL);

    glDeleteProgram(shader->id);

    arrayFree(shader->shaders);
    free(shader);
}

shaderT* useShader(const shaderT* shader) {
    const shaderT* old_shader = active_shader;

    if (shader != active_shader) {
        glUseProgram(shader ? shader->id : 0);
        active_shader = shader;
    }

    return (old_shader);
}

void compileFragmentShader(shaderT* shader, const string* source) {
    compileShader(GL_FRAGMENT_SHADER, shader, source);
}

void compileGeometryShader(shaderT* shader, const string* source) {
    compileShader(GL_GEOMETRY_SHADER, shader, source);
}

void compileVertexShader(shaderT* shader, const string* source) {
    compileShader(GL_VERTEX_SHADER, shader, source);
}

bool setShaderParam(const string* name, const void* value) {
    if (!active_shader)
        error("no shader in use");

    GLuint index = (GLuint)(-1);
    glGetUniformIndices(active_shader->id, 1, &name, &index);

    if ((int)index == (-1)) {
        //warn("couldn't set shader uniform '%s'", name);
        return (false);
    }

    GLint type = 0;
    glGetActiveUniformsiv(active_shader->id, 1, &index, GL_UNIFORM_TYPE, &type);

    GLint loc = glGetUniformLocation(active_shader->id, name);
    switch (type) {
    case GL_INT          : glUniform1i       (loc,  *(GLint  *)value); break;
    case GL_UNSIGNED_INT : glUniform1ui      (loc,  *(GLuint *)value); break;
    case GL_FLOAT        : glUniform1f       (loc,  *(GLfloat*)value); break;
    case GL_FLOAT_VEC2   : glUniform2fv      (loc, 1,          value); break;
    case GL_FLOAT_VEC3   : glUniform3fv      (loc, 1,          value); break;
    case GL_FLOAT_VEC4   : glUniform4fv      (loc, 1,          value); break;
    case GL_FLOAT_MAT2   : glUniformMatrix2fv(loc, 1, GL_TRUE, value); break;
    case GL_FLOAT_MAT3   : glUniformMatrix3fv(loc, 1, GL_TRUE, value); break;
    case GL_FLOAT_MAT4   : glUniformMatrix4fv(loc, 1, GL_TRUE, value); break;
    default              : error("unknown uniform type");
    }

    return (true);
}

void shaderPostProcess(textureT* source_texture) {
    if (!active_shader)
        error("no shader in use");

    textureT* texture = source_texture;

    // Not very sane, but it's ok...
    static triMeshT* quad = NULL;
    if (!quad)
        quad = createQuad(2.0f, 2.0f);

    clearDisplay(1.0f, 0.0f, 1.0f);

    useTexture(texture, 0);

    drawMesh(quad);

    //freeMesh(quad);
    useTexture(NULL, 0);
}
