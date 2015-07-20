#ifndef lightsource_h_
#define lightsource_h_

#include "base/common.h"
#include "math/vector.h"

typedef struct {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} lightSourceT;

#endif // lightsource_h_
