#ifndef lightsource_h_
#define lightsource_h_

#include "base/common.h"
#include "math/vector.h"

typedef struct {
    vec3  position;
    float intensity;
} lightSourceT;

#endif // lightsource_h_
