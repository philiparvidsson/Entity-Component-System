#ifndef physics_p_h_
#define physics_p_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/array.h"
#include "base/common.h"
#include "math/shape.h"
#include "math/vector.h"
#include "physics/body.h"
#include "physics/physics.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

typedef struct {
    vec2  x; // Position.
    float o; // Orientation.
    vec2  v; // Velocity.
    float w; // Angular velocity.
    vec2  a; // Acceleration.
    float t; // Torque.
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

    arrayT* collisions; // Used to hold collisions during collision testing.
};

#endif // physics_p_h_
