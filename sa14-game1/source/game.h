/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/array.h"
#include "core/common.h"
#include "core/file_io.h"

#include "gfx/graphics.h"
#include "gfx/shader.h"
#include "gfx/trimesh.h"

#include "input/mouse.h"

#include "math/matrix.h"
#include "math/vector.h"

#include <stdlib.h>

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

typedef struct cameraT {
    vec3 pos;
    vec3 target;
    vec3 up;
    float fov;
};

typedef enum {
    CountDownState
} gameStateT;

typedef struct {
    gameStateT state;
} gameT;

typedef struct {
    triMeshT *mesh;
    mat4x4    transform;

    void(*update)(void);
} gameObjectT;

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

#define TimeStep (1.0f/120.0f)

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

void gameMain(void);
