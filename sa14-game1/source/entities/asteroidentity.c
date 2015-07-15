#include "asteroidentity.h"

#include "base/common.h"
#include "components/graphicscomponent.h"
#include "components/physicscomponent.h"
#include "engine/entity.h"
#include "graphics/trimesh.h"

static gameComponentT* createGraphicsComponent(void) {
    triMeshT* mesh = createBox(0.03f, 0.03f, 0.03f);
    gameComponentT* component = newGraphicsComponent(mesh);

    return (component);
}

static gameComponentT* createPhysicsComponent(void) {
    gameComponentT* component = newPhysicsComponent(1.0f * Kilogram);

    physicsComponentDataT* component_data = component->data;
    
    vec3 pos = { 0.0f, 0.0f, 0.0f };
    bodySetPosition(((physicsComponentDataT*)component_data)->body, pos);

    vec3 vel = { 0.2f, 0.1f, 0.0f };
    bodySetVelocity(((physicsComponentDataT*)component_data)->body, vel);

    return (component);
}

gameEntityT* newAsteroidEntity(void) {
    gameEntityT* entity = newEntity();

    entity->data = calloc(1, sizeof(asteroidT));

    attachComponent(entity, createGraphicsComponent());
    attachComponent(entity, createPhysicsComponent());

    return (entity);
}
