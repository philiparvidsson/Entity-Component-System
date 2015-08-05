#include "playerentity.h"

#include "components/graphicscomponent.h"
#include "components/physicscomponent.h"
#include "engine/entity.h"
#include "graphics/trimesh.h"
#include "graphics/io/3ds.h"

#include "math/matrix.h"

static void handleInput(gameComponentT* component, float dt) {
    playerEntityDataT* player = component->entity->data;

    if (keyIsPressed(ArrowLeft))  player->angle += 2.0f * dt;
    if (keyIsPressed(ArrowRight)) player->angle -= 2.0f * dt;

    if (keyIsPressed(ArrowUp)) {
        vec2 f;

        f.x = cosf(player->angle) * 1.5f;
        f.y = sinf(player->angle) * 1.5f;

        physicsComponentDataT* physics_data = component->data;
        bodyApplyForce(physics_data->body, f, f);
    }

    graphicsComponentDataT* gfx = getComponent(component->entity, "graphics")->data;

    mat_rot_z(player->angle - 90.0f*3.1415f / 180.0f, &gfx->transform);
}

gameEntityT* newPlayerEntity(void) {
    gameEntityT* entity = newEntity();

    entity->data = calloc(1, sizeof(playerEntityDataT));
    
    const a3dsDataT* a3ds = gameResource      ("mesh:player", ResMesh);
          triMeshT*  mesh = createBox(0.3f, 0.3f, 0.3f);//a3dsCreateMesh    (a3ds, "Teapot001");
          materialT* mat  = a3dsCreateMaterial(a3ds, "Material #25");

    assert(mesh != NULL);
    assert(mat  != NULL);

    gameComponentT* gfx = newGraphicsComponent(mesh, mat);
    graphicsComponentDataT* gfx_data = gfx->data;
    assert(gfx_data->material != NULL);

    gameComponentT* phys = newPhysicsComponent(1.0f);
    phys->update_fn = handleInput;

    attachComponent(entity, gfx);
    attachComponent(entity, phys);

    return (entity);
}
