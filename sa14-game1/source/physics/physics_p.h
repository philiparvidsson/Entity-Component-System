#ifndef physics_p_h_
#define physics_p_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/common.h"
#include "math/shape.h"
#include "math/vector.h"
#include "physics/body.h"
#include "physics/physics.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

typedef struct {
    vec2 x; // Position.
    vec2 v; // Velocity.
    vec2 a; // Acceleration.

    float o;  // Orientation.
    float w;  // Angular velocity.
    float t;  // Torque.
} bodyStateT;

struct bodyT {
    worldT* world;

    shapeT* shape;

    bodyStateT state;
    bodyStateT prev_state;

    bodyTypeT type;
    float     inv_mass;
    float     inv_inertia;
    float     restitution;

    bodyT* prev;
    bodyT* next;
};

struct worldT {
    bodyT* bodies;
};

#endif // physics_p_h_
