#ifndef physics_p_
#define physics_p_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"

#include "math/vector.h"

#include "physics/physics.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

struct aabbT {
    vec3 min;
    vec3 max;
};

struct bodyT {
    aabbT aabb;

    vec3 pos;
    vec3 vel;
    vec3 acc;

    worldT*   world;
    bodyTypeT type;
    float     mass;

    bodyT* prev;
    bodyT* next;
};

struct worldT {
    aabbT bounds;

    bodyT* first_body;
};

#endif
