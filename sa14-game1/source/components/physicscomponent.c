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

gameComponentT* newPhysicsComponent(float mass) {
    gameComponentT* component = newComponent("physics");
    physicsComponentDataT* data = calloc(1, sizeof(physicsComponentDataT));

    data->body = bodyNew(mass);

    component->data = data;

    return (component);
}
