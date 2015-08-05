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
    materialT* material;         // Shader material.
    //shaderT*   shader;           // Shader, or NULL to use default.
    //textureT*  texture;          // Texture, or NULL to use default.

    mat4x4 model_view_proj;      // Current frame MVP matrix.
    mat4x4 prev_model_view_proj; // Last frame MVP matrix.
} graphicsComponentDataT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

gameComponentT* newGraphicsComponent(triMeshT* mesh, materialT* material);

#endif // graphicsscomponent_h_
