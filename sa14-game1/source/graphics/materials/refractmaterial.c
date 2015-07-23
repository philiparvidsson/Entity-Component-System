#include "refractmaterial.h"

#include "base/common.h"
#include "engine/game.h"
#include "graphics/material.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "math/vector.h"

#include <GL/glew.h>

typedef struct {
    textureT* screen_tex;

    vec3  color_coeff;
    float refraction;
    float transparency;
} refractMaterialT;

static void refractBegin(materialT* m) {
    refractMaterialT* refract = m->data;

    loadTextureFromScreen(refract->screen_tex);

    setShaderParam("ColorCoeff"    , &refract->color_coeff);
    setShaderParam("Transparency"  , &refract->transparency);
    setShaderParam("RefractionMult", &refract->refraction);

    useTexture(refract->screen_tex, 0);

    glPushAttrib(GL_ENABLE_BIT);
    glDisable   (GL_CULL_FACE );
    glDisable   (GL_DEPTH_TEST);
}

static void refractEnd(materialT* m) {
    glPopAttrib();
}

materialT* createRefractMaterial(vec3  color_coeff,
                                 float refraction,
                                 float transparency)
{
    materialT* m = createCustomRefractMaterial(color_coeff, refraction,
                                               transparency, NULL, NULL);

    return (m);
}

materialT* createCustomRefractMaterial(vec3    color_coeff,
                                       float   refraction,
                                       float   transparency,
                                       string* vert_src,
                                       string* frag_src)
{
    shaderT* refract_shader = createShader();

    if (vert_src) {
        compileVertexShader(refract_shader, vert_src);

    }
    else {
        vert_src = readGamePakFile("default.vert");
        compileVertexShader(refract_shader, vert_src);
        free(vert_src);
    }

    if (frag_src) {
        compileFragmentShader(refract_shader, frag_src);
    }
    else {
        frag_src = readGamePakFile("materials/refractmaterial.frag");
        compileFragmentShader(refract_shader, frag_src);
        free(frag_src);
    }

    materialT*        m       = newMaterial();
    refractMaterialT* refract = malloc(sizeof(refractMaterialT));

    m->type       = "refract";
    m->data       = refract;
    m->sort_value = 2000;
    m->shader     = refract_shader;
    m->begin_fn   = refractBegin;
    m->end_fn     = refractEnd;
    
    refract->screen_tex = createTexture();

    refract->color_coeff  = color_coeff;
    refract->refraction   = refraction;
    refract->transparency = transparency;

    return (m);
}
