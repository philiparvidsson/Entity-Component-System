#ifndef graphicscomponent_h_
#define graphicscomponent_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/common.h"
#include "engine/component.h"
#include "graphics/material.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/trimesh.h"
#include "math/matrix.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

typedef struct {
    triMeshT*  mesh;             // Model mesh.
    mat4x4     transform;        // Model transform matrix.
    mat4x4     normal_transform; // Normal transform matrix.
    materialT* material;         // Shader material.
    //shaderT*   shader;           // Shader, or NULL to use default.
    textureT*  texture;          // Texture, or NULL to use default.
} graphicsComponentDataT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

gameComponentT* newGraphicsComponent(triMeshT* mesh);

#endif // graphicsscomponent_h_
