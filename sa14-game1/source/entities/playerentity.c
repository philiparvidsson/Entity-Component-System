#include "engine/entity.h"

gameEntityT* createPlayerEntity(void) {
    gameEntityT* entity = newEntity();

    gameComponentT* render_component = createRenderComponent();

    setRenderComponentMesh(render_component, createBox());

    return (entity);
}