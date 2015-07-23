#ifndef material_h_
#define material_h_

#define VECTOR_RGB

#include "base/common.h"
#include "graphics/shader.h"
#include "math/vector.h"

typedef struct materialT {
    string* name;
    string* type;
    int sort_value;

    shaderT* shader;
    void*    data;

    void (*begin_fn)(struct materialT* m);
    void (*end_fn)(struct materialT* m);
} materialT;

materialT* newMaterial(void);
const materialT* getNamedMaterial(const string* name);
materialT* activeMaterial(void);
void useMaterial(materialT* m);

#endif // material_h_
