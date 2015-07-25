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
} refractMaterialT;

static void refractBegin(materialT* m) {
    refractMaterialT* refract = m->data;

    loadTextureFromScreen(refract->screen_tex);

    setShaderParam("ColorCoeff"    , &refract->color_coeff);
    setShaderParam("RefractionMult", &refract->refraction);

    useTexture(refract->screen_tex, 0);

    glPushAttrib(GL_ENABLE_BIT);
    glDisable   (GL_CULL_FACE );
    glDisable   (GL_DEPTH_TEST);
}

static void refractEnd(materialT* m) {
    glPopAttrib();
}

materialT* createRefractMaterial(vec3 color_coeff, float refraction) {
    materialT* m = createCustomRefractMaterial(color_coeff, refraction, NULL,
                                               NULL);

    return (m);
}

materialT* createCustomRefractMaterial(vec3    color_coeff,
                                       float   refraction,
                                       string* vert_src,
                                       string* frag_src)
{
    shaderT* refract_shader;

    if (!vert_src && !frag_src) {
        refract_shader = gameResource("shader:refractmaterial", ResShader);
    }
    else {
        if (!vert_src)
            vert_src = gameResource("shaders/default.vert", ResString);

        if (!frag_src)
            frag_src = gameResource("shaders/materials/refractmaterial.frag", ResString);

        refract_shader = createShader();
        compileVertexShader  (refract_shader, vert_src);
        compileFragmentShader(refract_shader, frag_src);
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

    return (m);
}
