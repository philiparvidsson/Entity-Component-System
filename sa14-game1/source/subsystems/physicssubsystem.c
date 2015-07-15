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

#define TimeStep (1.0f/120.0f)

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

static void addBodyToWorld(gameSubsystemT* subsystem, gameComponentT* component) {
    physicsComponentDataT* component_data = component->data;
    physicsSubsystemDataT* subsystem_data = subsystem->data;

    worldAddBody(subsystem_data->world, component_data->body);
}

static void stepWorld(gameSubsystemT* subsystem, float dt) {
    physicsSubsystemDataT* data = subsystem->data;

    // We use data->dt to accumulate time so we always simulate exactly as much
    // as we need to, with a fixed time step.

    dt += data->dt;
    while (dt >= TimeStep) {
        worldStep(data->world, TimeStep);
        dt -= TimeStep;
    }
    data->dt = dt;
}

gameSubsystemT* newPhysicsSubsystem(void) {
    gameSubsystemT* subsystem = newSubsystem("physics");
    physicsSubsystemDataT* data = calloc(1, sizeof(physicsSubsystemDataT));

    data->world = worldNew();

    // The physics subsystem is a bit different because all components are
    // actually updated in the after_update_fn, not in each component's update
    // function. This allows us to do collision etc more efficiently.

    subsystem->data = data;
    subsystem->after_update_fn = stepWorld;
    subsystem->add_component_fn = addBodyToWorld;

    return (subsystem);
}
