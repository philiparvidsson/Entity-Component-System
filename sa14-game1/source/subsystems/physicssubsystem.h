#ifndef physicssubsystem_h_
#define physicssubsystem_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "engine/subsystem.h"

#include "physics/physics.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

typedef struct {
    float dt;
    worldT* world;
} physicsSubsystemDataT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

gameSubsystemT* newPhysicsSubsystem(void);

#endif // physicssubsystem_h_
