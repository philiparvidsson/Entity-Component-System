#ifndef asteroidentity_h_
#define asteroidentity_h_

#include "math/vector.h"

typedef struct {
    float angle1;
    float angle2;
    float a1;
    float a2;
    float scale;
    vec3 rot_axis_1;
    vec3 rot_axis_2;
} asteroidEntityDataT;

#endif // asteroidentity_h_
