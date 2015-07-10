#include "engine_private.h"

#include "base/array.h"
#include "base/common.h"

#include <stdlib.h>

gameEntityT* newEntity(void) {
    gameEntityT* entity = calloc(1, sizeof(gameEntityT));

    entity->components = arrayNew(sizeof(gameEntityT*));

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
    arrayAdd(entity->components, &component);
}
