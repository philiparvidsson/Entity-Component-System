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
 * FUNCTIONS
 *----------------------------------------------*/

static void addBodyToWorld(gameSubsystemT* subsystem, gameComponentT* component) {
    physicsComponentDataT* component_data = component->data;
    physicsSubsystemDataT* subsystem_data = subsystem->data;

    worldAddBody(subsystem_data->world, component_data->body);
}

static void updatePhysics(gameSubsystemT* subsystem, float dt) {
    physicsSubsystemDataT* data = subsystem->data;

    while (dt >= TimeStep) {
        worldStep(data->world, TimeStep);
        dt -= TimeStep;
    }
}

gameSubsystemT* newPhysicsSubsystem(void) {
    gameSubsystemT* subsystem = newSubsystem("physics");
    physicsSubsystemDataT* data = calloc(1, sizeof(physicsSubsystemDataT));

    data->world = worldNew();

    subsystem->data            = data;
    subsystem->after_update_fn = updatePhysics;

    subsystem->add_component_fn = addBodyToWorld;
    //subsystem->remove_component_fn = addBodyToWorld;

    return (subsystem);
}
