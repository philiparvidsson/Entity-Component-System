#ifndef shape_h_
#define shape_h_

#include "base/common.h"
#include "math/vector.h"

typedef struct {
    vec2* points;
    int num_points;
    int num_edges;
} shapeT;

#endif // shape_h_
