#ifndef gameentity_h_
#define gameentity_h_

#include "base/array.h"
#include "base/common.h"

#include "engine/component.h"
#include "engine/game.h"

struct gameEntityT {
    gameT* game;
    arrayT* components;
};

typedef struct gameEntityT gameEntityT;

gameEntityT* newEntity(void);
void freeEntity(gameEntityT* entity);

void attachComponentToEntity(gameComponentT* component, gameEntityT* entity);

#endif // gameentity_h_
