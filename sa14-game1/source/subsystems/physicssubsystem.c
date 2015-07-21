/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "physicssubsystem.h"

#include "components/physicscomponent.h"
#include "engine/game.h"
#include "engine/subsystem.h"
#include "graphics/graphics.h"
#include "graphics/shader.h"
#include "math/matrix.h"
#include "physics/physics.h"

#include <stdlib.h>

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

#define TimeStep (1.0f/60.0f)

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

typedef struct {
    float time_frac;
    worldT* world;
} physicsSubsystemDataT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static void addBodyToWorld(gameSubsystemT* subsystem, gameComponentT* component) {
    physicsComponentDataT* phys_component = component->data;
    physicsSubsystemDataT* phys_data = subsystem->data;

    worldAddBody(phys_data->world, phys_component->body);
}

static void stepWorld(gameSubsystemT* subsystem, float dt) {
    physicsSubsystemDataT* phys_data = subsystem->data;

    dt += phys_data->time_frac;
    while (dt >= TimeStep) {
        worldStep(phys_data->world, TimeStep);
        dt -= TimeStep;
    }
    phys_data->time_frac = dt;
}

gameSubsystemT* newPhysicsSubsystem(void) {
    gameSubsystemT* subsystem = newSubsystem("physics");
    physicsSubsystemDataT* phys_data = calloc(1, sizeof(physicsSubsystemDataT));

    phys_data->world = worldNew();

    // The physics subsystem is a bit different because all components are
    // actually updated in the after_update_fn, not in each component's update
    // function. This allows us to do collision etc more efficiently.

    subsystem->data = phys_data;
    subsystem->after_update_fn = stepWorld;
    subsystem->add_component_fn = addBodyToWorld;

    return (subsystem);
}
