#ifndef entity_h_
#define entity_h_

#include "game/game.h"

entityT* entityAlloc();
void entityInit(entityT* entity);
entityT* entityNew();
void entityFree(entityT* entity);
void* entityGetDataPtr(entityT* entity);
void entitySetDataPtr(entityT* entity, void* data);
entityCleanupFuncT entityGetCleanupFunc(const entityT* entity);
void entitySetCleanupFunc(entityT* entity, entityCleanupFuncT cleanup_func);
entityDrawFuncT entityGetDrawFunc(const entityT* entity);
void entitySetDrawFunc(entityT* entity, entityDrawFuncT draw_func);
entityUpdateFuncT entityGetUpdateFunc(const entityT* entity);
void entitySetUpdateFunc(entityT* entity, entityUpdateFuncT update_func);

#endif // entity_h_
