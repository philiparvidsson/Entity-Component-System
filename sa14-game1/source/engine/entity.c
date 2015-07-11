#include "entity.h"

#include "base/array.h"
#include "base/common.h"
#include "base/debug.h"

#include "engine/component.h"

#include <stdlib.h>
#include <string.h>

gameEntityT* newEntity(void) {
    gameEntityT* entity = calloc(1, sizeof(gameEntityT));

    entity->components = arrayNew(sizeof(gameComponentT*));

    return (entity);
}

void freeEntity(gameEntityT* entity) {
    if (entity->components) {
        arrayFree(entity->components);
        entity->components = NULL;
    }

    return (entity);
}

void attachComponentToEntity(gameComponentT* component, gameEntityT* entity) {
    assert(component->entity == NULL);

    component->entity = entity;
    arrayAdd(entity->components, &component);
}

gameComponentT* getEntityComponent(gameEntityT* entity, const string* subsystem_name) {
    int num_components = arrayLength(entity->components);
    for (int i = 0; i < num_components; i++) {
        gameComponentT* component = *(gameComponentT**)arrayGet(entity->components, i);
        if (strcmp(component->subsystem_name, subsystem_name)==0)
            return (component);
    }

    return (NULL);
}