#ifndef world_h_
#define world_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/array.h"
#include "base/common.h"
#include "physics/physics.h"

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

worldT* worldAlloc(void);
void worldInit(worldT* world);
worldT* worldNew(void);
void worldFree(worldT* world);
void worldAddBody(worldT* world, bodyT* body);
void worldStep(worldT* world, float dt);

bool areBodiesColliding(bodyT* a, bodyT* b);

#endif // world_h
