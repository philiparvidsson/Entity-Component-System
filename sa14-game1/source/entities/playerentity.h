#ifndef playerentity_h_
#define playerentity_h_

#include "engine/entity.h"

typedef struct {
    float angle;
} playerEntityDataT;

gameEntityT* newPlayerEntity(void);

#endif // playerentity_h_
