/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "graphicscomponent.h"

#include "components/physicscomponent.h"

#include "engine/component.h"

#include "graphics/shader.h"
#include "graphics/trimesh.h"

#include <stdlib.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static void draw(gameComponentT* component, float dt) {
    gameComponentT* physics_component = getEntityComponent(component->entity, "physics");

    // The graphics component needs a physics component to pull the position
    // from.
    assert(physics_component != NULL);

    graphicsComponentDataT* graphics_data = component->data;
    physicsComponentDataT*  physics_data  = physics_component->data;

    // If there's no mesh to render, we exit the function here.
    if (!graphics_data->mesh)
        return;

    // The "Model" uniform variable is used by the vertex shader as the model
    // transform matrix.
    mat4x4 transform;
    mat_identity(&transform);

    mat4x4 transl;
    vec3 pos = bodyGetPosition(physics_data->body);
    mat_transl_xyz(pos.x, pos.y, pos.z, &transl);

    mat_mul(&graphics_data->transform, &transform, &transform);
    mat_mul(&transl, &transform, &transform);

    setShaderParam("Model", &transform);

    drawMesh(graphics_data->mesh);
}

static void cleanup(gameComponentT* component, gameSubsystemT* subsystem) {
    graphicsComponentDataT* data = component->data;

    if (data->mesh) {
        freeMesh(data->mesh);
        data->mesh = NULL;
    }
}

gameComponentT* newGraphicsComponent(triMeshT* mesh) {
    gameComponentT* component = newComponent("graphics");
    graphicsComponentDataT* data = calloc(1, sizeof(graphicsComponentDataT));

    data->mesh = mesh;
    mat_identity(&data->transform);

    component->data       = data;
    component->update_fn  = draw;
    component->cleanup_fn = cleanup;

    return (component);
}
