#ifndef material_h_
#define material_h_

#define VECTOR_RGB

#include "base/common.h"
#include "math/vector.h"

typedef struct {
    vec3  ambient;
    vec3  diffuse;
    vec3  specular;
    float shininess;
} materialT;

const materialT* defaultMaterial(void);
const materialT* getNamedMaterial(const string* name);

#endif // material_h_
