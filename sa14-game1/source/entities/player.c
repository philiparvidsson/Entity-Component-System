#include "components/render.h"

#include "engine/entity.h"

#include "graphics/trimesh.h"

gameEntityT* createPlayerEntity(void) {
    gameEntityT* entity = newEntity();

    gameComponentT* render_component = createRenderComponent();

    setRenderMesh(render_component, createBox(0.1f, 0.1f, 0.1f));

    return (entity);
}
