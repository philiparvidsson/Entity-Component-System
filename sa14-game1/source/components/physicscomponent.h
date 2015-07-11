#ifndef physicscomponent_h_
#define physicscomponent_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/common.h"

#include "engine/component.h"

#include "math/matrix.h"

#include "physics/physics.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

typedef struct {
    bodyT* body; // The physical body.
    void*  data; // Custom user data.
} physicsComponentDataT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

gameComponentT* newPhysicsComponent(float mass);

#endif // physicscomponent_h_
