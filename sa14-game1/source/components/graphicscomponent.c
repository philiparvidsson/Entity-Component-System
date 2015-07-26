/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "graphicscomponent.h"

#include "base/common.h"
#include "engine/component.h"
#include "graphics/material.h"

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static void cleanup(gameComponentT* component, gameSubsystemT* subsystem) {
    graphicsComponentDataT* gfx = component->data;

    if (gfx->mesh) {
        freeMesh(gfx->mesh);
        gfx->mesh = NULL;
    }
}

gameComponentT* newGraphicsComponent(triMeshT* mesh, materialT* material) {
    gameComponentT*         component = newComponent("graphics");
    graphicsComponentDataT* gfx_data  = calloc(1, sizeof(graphicsComponentDataT));

    gfx_data->mesh     = mesh;
    gfx_data->material = material ? material : getNamedMaterial("debug");

    mat_identity(&gfx_data->transform);
    mat_identity(&gfx_data->normal_transform);

    component->data       = gfx_data;
    component->cleanup_fn = cleanup;

    return (component);
}
