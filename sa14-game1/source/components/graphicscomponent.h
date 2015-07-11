#ifndef graphicscomponent_h_
#define graphicscomponent_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "engine/component.h"

#include "graphics/trimesh.h"

#include "math/matrix.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

typedef struct {
    triMeshT* mesh;      // The model mesh.
    mat4x4    transform; // The model transform matrix.
    void*     data;      // Custom user data.
} graphicsComponentDataT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

gameComponentT* newGraphicsComponent(triMeshT* mesh);

#endif // graphicsscomponent_h_
