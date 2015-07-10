#ifndef rendercomponent_h_
#define rendercomponent_h_

#include "engine/component.h"

#include "graphics/trimesh.h"

#include "math/matrix.h"

typedef struct {
    triMeshT* mesh;
    mat4x4 transform;
    void* data;
} renderComponentDataT;

gameComponentT* createRenderComponent(void);

#endif // rendercomponent_h_
