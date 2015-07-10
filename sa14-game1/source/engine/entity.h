#ifndef gameentity_h_
#define gameentity_h_

#include "engine/component.h"

typedef struct gameEntityT gameEntityT;

gameEntityT* newEntity(void);
void freeEntity(gameEntityT* entity);
void attachComponent(gameComponentT* component, gameEntityT* entity);

#endif // gameentity_h_
