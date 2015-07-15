/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "physicscomponent.h"

#include "base/common.h"
#include "engine/component.h"
#include "physics/physics.h"

#include <stdlib.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static void cleanup(gameComponentT* component, gameSubsystemT* subsystem) {
    physicsComponentDataT* phys = component->data;

    if (phys->body) {
        bodyFree(phys->body);
        phys->body = NULL;
    }
}

gameComponentT* newPhysicsComponent(float mass) {
    gameComponentT* component = newComponent("physics");
    physicsComponentDataT* data = calloc(1, sizeof(physicsComponentDataT));

    data->body = bodyNew(mass);

    component->data = data;
    component->cleanup_fn = cleanup;

    return (component);
}
