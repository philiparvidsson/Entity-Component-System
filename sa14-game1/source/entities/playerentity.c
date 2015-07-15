#include "playerentity.h"

#include "components/graphicscomponent.h"
#include "components/physicscomponent.h"

#include "engine/entity.h"

#include "graphics/trimesh.h"

#include "math/matrix.h"

static int lol;
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

    graphicsComponentDataT* gfx = getComponent(component->entity, "graphics")->data;

    mat_rot_z(player_data->angle-90.0f*3.1415f/180.0f, &gfx->transform);
    mat_rot_z(player_data->angle-90.0f*3.1415f/180.0f, &gfx->normal_transform);

    lol++;
    if (lol == 60) {
        lol = 0;

        gameEntityT* e = newAsteroidEntity();
        addEntityToGame(e);
    }
}

gameEntityT* newPlayerEntity(void) {
    gameEntityT* entity = newEntity();

    entity->data = calloc(1, sizeof(playerEntityDataT));

    triMeshT* mesh = createCone(0.02f, 0.04f, 8);
    gameComponentT* gfx_c = newGraphicsComponent(mesh);

    gameComponentT* phys_c = newPhysicsComponent(1.0f);
    phys_c->update_fn = handleInput;

    attachComponent(entity, gfx_c);
    attachComponent(entity, phys_c);

    return (entity);
}
