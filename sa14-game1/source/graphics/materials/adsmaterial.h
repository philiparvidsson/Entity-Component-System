#ifndef adsmaterial_h_
#define adsmaterial_h_

#include "base/common.h"
#include "graphics/material.h"
#include "graphics/texture.h"
#include "math/vector.h"

materialT* createADSMaterial(textureT* ambient_tex,
                             textureT* diffuse_tex,
                             textureT* specular_tex,
                             vec3      ambient_coeff,
                             vec3      diffuse_coeff,
                             vec3      specular_coeff,
                             float     shininess,
                             bool      wireframe);

materialT* createCustomADSMaterial(textureT* ambient_tex,
                                   textureT* diffuse_tex,
                                   textureT* specular_tex,
                                   vec3      ambient_coeff,
                                   vec3      diffuse_coeff,
                                   vec3      specular_coeff,
                                   float     shininess,
                                   bool      wireframe,
                                   string*   vert_src,
                                   string*   frag_src);

#endif // adsmaterial_h_
