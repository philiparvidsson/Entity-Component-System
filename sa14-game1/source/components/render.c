#include "render.h"

#include "engine/component.h"

#include "graphics/trimesh.h"

typedef struct {
    triMeshT* mesh;
} renderDataT;

static void render(gameComponentT* component, float dt) {
    renderDataT* render_data = (renderDataT*)getComponentDataPtr(component);

    drawMesh(render_data->mesh);
}

gameComponentT* createRenderComponent(void) {
    gameComponentT* component = newComponent("render");

    renderDataT* render_data = malloc(sizeof(renderDataT));

    setComponentDataPtr(component, render_data);
    setComponentUpdateFn(component, render);

    return (component);
}

void setRenderTransform(gameComponentT* component) {

}
