#ifndef refractmaterial_h_
#define refractmaterial_h_

#include "base/common.h"
#include "graphics/material.h"
#include "graphics/texture.h"
#include "math/vector.h"

materialT* createRefractMaterial(vec3 color_coeff, float refraction);

materialT* createCustomRefractMaterial(vec3    color_coeff,
                                       float   refraction,
                                       string* vert_src,
                                       string* frag_src);

#endif // refractmaterial_h_
