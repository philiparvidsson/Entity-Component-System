#ifndef graphicscomponent_h_
#define graphicscomponent_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/common.h"
#include "engine/component.h"
#include "graphics/trimesh.h"
#include "math/matrix.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

typedef struct {
    triMeshT* mesh;             // The model mesh.
    mat4x4    transform;        // The model transform matrix.
    mat4x4    normal_transform; // The model normal transform matrix.
} graphicsComponentDataT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

gameComponentT* newGraphicsComponent(triMeshT* mesh);

#endif // graphicsscomponent_h_
