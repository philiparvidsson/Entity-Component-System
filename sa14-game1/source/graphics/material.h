#ifndef material_h_
#define material_h_

#define VECTOR_RGB

#include "base/common.h"
#include "math/vector.h"
#include "graphics/shader.h"

typedef struct materialT {
    shaderT* shader;
    void*    data;

    void (*use_fn)(struct materialT* m);
} materialT;

materialT* newMaterial(void);
const materialT* getNamedMaterial(const string* name);
void useMaterial(materialT* m);

#endif // material_h_
