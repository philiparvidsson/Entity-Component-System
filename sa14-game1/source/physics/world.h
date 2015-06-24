#ifndef world_h_
#define world_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/array.h"
#include "core/common.h"

#include "physics/physics.h"

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

worldT* worldAlloc(void);
void worldInit(worldT* world);
void worldFree(worldT* world);
void worldAddBody(worldT* world, bodyT* body);
void worldStep(worldT* world, float dt);

#endif // world_h
