#ifndef gameentity_h_
#define gameentity_h_

#include "base/array.h"
#include "base/common.h"

typedef struct gameEntityT gameEntityT;

#include "engine/component.h"
#include "engine/game.h"

struct gameEntityT {
    gameT* game;
    arrayT* components;
};


gameEntityT* newEntity(void);
void freeEntity(gameEntityT* entity);

void attachComponentToEntity(gameComponentT* component, gameEntityT* entity);
gameComponentT* getEntityComponent(gameEntityT* entity, const string* subsystem_name);

#endif // gameentity_h_
