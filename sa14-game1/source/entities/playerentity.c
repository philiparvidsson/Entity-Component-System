#include "playerentity.h"

#include "components/graphicscomponent.h"
#include "components/physicscomponent.h"
#include "engine/entity.h"
#include "graphics/trimesh.h"
#include "graphics/io/3ds.h"

#include "math/matrix.h"

static void playerDerivativeFn(const float* state, float* derivs) {
    // state[0] = x.x
    // state[1] = x.y
    // state[2] = o
    // state[3] = v.x
    // state[4] = v.y
    // state[5] = w

    // derivs[0] = v.x
    // derivs[1] = v.y
    // derivs[2] = w
    // derivs[3] = a.x
    // derivs[4] = a.y
    // derivs[5] = t

    derivs[0] = state[3];
    derivs[1] = state[4];
    derivs[2] = state[5];
    
    derivs[3] = -state[3] * 5.0f;
    derivs[4] = -state[4] * 5.0f;
    derivs[5] = -state[5] * 5.0f;

    if (keyIsPressed(ArrowLeft )) derivs[5] += 10.0f;
    if (keyIsPressed(ArrowRight)) derivs[5] -= 10.0f;

    if (keyIsPressed(ArrowUp)) {
        vec2 f;

        derivs[3] += cosf(state[2]) * 10.0f;
        derivs[4] += sinf(state[2]) * 10.0f;
    }
}

static void handleInput(gameComponentT* component, float dt) {
    playerEntityDataT* player = component->entity->data;



    graphicsComponentDataT* gfx = getComponent(component->entity, "graphics")->data;
    bodyT* body = ((physicsComponentDataT*)component->data)->body;

    mat_rot_z(bodyOrientation(body) - 90.0f*3.1415f / 180.0f, &gfx->transform);
}

gameEntityT* newPlayerEntity(void) {
    gameEntityT* entity = newEntity();

    entity->data = calloc(1, sizeof(playerEntityDataT));
    
    const a3dsDataT* a3ds = gameResource      ("mesh:player", ResMesh);
          triMeshT*  mesh = a3dsCreateMesh    (a3ds, "Teapot002");
          materialT* mat  = a3dsCreateMaterial(a3ds, "Material #25");

    assert(mesh != NULL);
    assert(mat  != NULL);

    gameComponentT* gfx = newGraphicsComponent(mesh, mat);
    graphicsComponentDataT* gfx_data = gfx->data;
    assert(gfx_data->material != NULL);

    gameComponentT* phys = newPhysicsComponent(1.0f);
    phys->update_fn = handleInput;

    physicsComponentDataT* phys_data = phys->data;
    bodySetDerivativeFn(phys_data->body, playerDerivativeFn);

    attachComponent(entity, gfx);
    attachComponent(entity, phys);

    return (entity);
}
