#ifndef shape_h_
#define shape_h_

#include "base/common.h"
#include "math/aabb.h"
#include "math/vector.h"

#define ShapeMaxPoints 256

typedef struct {
    int  num_points;
    vec2 points[1];
} shapeT;

shapeT* shapeNew(int num_points);
shapeT* shapeNewSquare(float width, float height);

void shapeFree(shapeT* shape);

aabbT shapeAABB(const shapeT* shape);

#endif // shape_h_
