#include "playerentity.h"

#include "components/rendercomponent.h"

#include "engine/entity.h"

#include "graphics/trimesh.h"



gameEntityT* createPlayerEntity(void) {
    gameEntityT* player_entity = newEntity();

    gameComponentT* render_component = createRenderComponent();

    renderComponentDataT* render_data = render_component->data;
    render_data->mesh = createBox(0.1f, 0.1f, 0.1f);

    attachComponentToEntity(render_component, player_entity);

    return (player_entity);
}
