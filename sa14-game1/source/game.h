/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/array.h"
#include "core/common.h"
#include "core/file_io.h"

#include "gfx/graphics.h"
#include "gfx/shader.h"
#include "gfx/trimesh.h"

#include "input/keyboard.h"
#include "input/mouse.h"

#include "math/matrix.h"
#include "math/vector.h"

#include "physics/body.h"
#include "physics/world.h"

#include <stdlib.h>

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

typedef struct {
    keyboardStateT keyboard;
    mouseStateT    mouse;

    arrayT* objects;

    worldT* world;
} gameT;

typedef struct gameObjectT {
    gameT    *game;

    triMeshT* model;
    mat4x4    transform;

    bodyT    *body;

    void(*cleanupFunc)(struct gameObjectT*);
    void(*updateFunc)(struct gameObjectT*);
} gameObjectT;

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

#define TimeStep (1.0f/120.0f)

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

void gameMain(void);
