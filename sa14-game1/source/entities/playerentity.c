#include "playerentity.h"

#include "components/graphicscomponent.h"
#include "components/physicscomponent.h"

#include "engine/entity.h"

#include "graphics/trimesh.h"

static void handleInput(gameComponentT* component, float dt) {
}

gameEntityT* newPlayerEntity(void) {
    gameEntityT* player_entity = newEntity();

    // Graphics.
    triMeshT* mesh = createBox(0.1f, 0.1f, 0.1f);
    gameComponentT* graphics_component = newGraphicsComponent(mesh);

    // Physics.
    gameComponentT* physics_component = newPhysicsComponent(1.0f);

    attachComponentToEntity(graphics_component, player_entity);
    attachComponentToEntity(physics_component, player_entity);

    return (player_entity);
}
