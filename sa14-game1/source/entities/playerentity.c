#include "playerentity.h"

#include "components/graphicscomponent.h"
#include "components/physicscomponent.h"

#include "engine/entity.h"

#include "graphics/trimesh.h"

#include "math/matrix.h"

static void handleInput(gameComponentT* component, float dt) {
    playerEntityDataT* player_data = component->entity->data;

    if (keyIsPressed(ArrowLeft))  player_data->angle += 1.5f * dt;
    if (keyIsPressed(ArrowRight)) player_data->angle -= 1.5f * dt;

    if (keyIsPressed(ArrowUp)) {
        vec3 f = { 0 };

        f.x = cosf(player_data->angle) * 0.3f;
        f.y = sinf(player_data->angle) * 0.3f;

        physicsComponentDataT* physics_data = component->data;
        bodyApplyForce(physics_data->body, f);
    }

    graphicsComponentDataT* graphics_data = getEntityComponent(component->entity, "graphics")->data;

    mat_rot_z(player_data->angle-90.0f*3.1415f/180.0f, &graphics_data->transform);
}

gameEntityT* newPlayerEntity(void) {
    gameEntityT* player_entity = newEntity();

    player_entity->data = calloc(1, sizeof(playerEntityDataT));

    // Graphics.
    triMeshT* mesh = createCone(0.02f, 0.04f, 8);
    gameComponentT* graphics_component = newGraphicsComponent(mesh);

    // Physics.
    gameComponentT* physics_component = newPhysicsComponent(1.0f);
    physics_component->update_fn = handleInput;

    attachComponentToEntity(graphics_component, player_entity);
    attachComponentToEntity(physics_component, player_entity);

    return (player_entity);
}
