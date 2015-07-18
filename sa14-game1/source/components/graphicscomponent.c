/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "graphicscomponent.h"

#include "base/common.h"
#include "engine/component.h"

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

gameComponentT* newGraphicsComponent(triMeshT* mesh) {
    gameComponentT* component = newComponent("graphics");
    graphicsComponentDataT* gfx = calloc(1, sizeof(graphicsComponentDataT));

    gfx->mesh = mesh;
    mat_identity(&gfx->transform);
    mat_identity(&gfx->normal_transform);

    component->data = gfx;
    component->cleanup_fn = cleanup;

    return (component);
}
