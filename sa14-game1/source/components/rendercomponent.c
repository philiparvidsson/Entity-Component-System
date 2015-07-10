#include "rendercomponent.h"

#include "engine/component.h"

#include "graphics/shader.h"
#include "graphics/trimesh.h"

#include <stdlib.h>

static void render(gameComponentT* component, float dt) {
    renderComponentDataT* render_data = (renderComponentDataT*)component->data;

    if (render_data->mesh) {
        setShaderParam("Model", &render_data->transform);
        drawMesh(render_data->mesh);
    }
}

gameComponentT* createRenderComponent(void) {
    gameComponentT* component = newComponent("render");

    renderComponentDataT* render_data = calloc(1, sizeof(renderComponentDataT));

    mat_identity(&render_data->transform);

    component->data = render_data;
    component->update_fn = render;

    return (component);
}
