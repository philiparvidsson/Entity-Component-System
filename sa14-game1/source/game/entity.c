#include "game_private.h"
#include "entity.h"

entityT* entityAlloc() {
    entityT* entity = malloc(sizeof(entityT));
}

void entityInit(entityT* entity) {
    memset(entity, 0, sizeof(entityT));
}

entityT* entityNew() {
    entityT* entity = entityAlloc();
    entityInit(entity);

    return (entity);
}

void entityFree(entityT* entity) {
    free(entity);
}

void* entityGetData(entityT* entity) {
    return (entity->data);
}

void entitySetData(entityT* entity, void* data) {
    entity->data = data;
}

entityCleanupFuncT entityGetCleanupFunc(const entityT* entity) {
    return (entity->cleanup_func);
}

void entitySetCleanupFunc(entityT* entity, entityCleanupFuncT cleanup_func) {
    entity->cleanup_func = cleanup_func;
}

entityDrawFuncT entityGetDrawFunc(const entityT* entity) {
    return (entity->draw_func);
}

void entitySetDrawFunc(entityT* entity, entityDrawFuncT draw_func) {
    entity->draw_func = draw_func;
}

entityUpdateFuncT entityGetUpdateFunc(const entityT* entity) {
    return (entity->update_func);
}

void entitySetUpdateFunc(entityT* entity, entityUpdateFuncT update_func) {
    entity->update_func = update_func;
}
