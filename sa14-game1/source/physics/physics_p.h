#ifndef physics_p_h_
#define physics_p_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/common.h"
#include "math/vector.h"
#include "physics/physics.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

struct aabbT {
    vec2 min;
    vec2 max;
};

typedef struct {
    vec2 x; // Position.
    vec2 v; // Velocity.
    vec2 a; // Acceleration.

    float o;  // Orientation.
    float w;  // Angular velocity.
    float t;  // Torque.

    float i; // Moment of inertia.
} bodyStateT;

struct bodyT {
    worldT* world;

    bodyStateT state;
    bodyStateT prev_state;

    bodyTypeT type;
    float     inv_mass;

    bodyT* prev;
    bodyT* next;
};

struct worldT {
    aabbT bounds;

    bodyT* bodies;
};

#endif // physics_p_h_
