#include "material.h"

#include "base/common.h"
#include "math/vector.h"
#include "graphics/materials/adsmaterial.h"

#include <string.h>

materialT* newMaterial(void) {
    materialT* m = malloc(sizeof(materialT));

    return (m);
}

const materialT* getNamedMaterial(const string* name) {
    materialT* material = NULL;

    // This is a debug material to visualize objects more clearly.
    if (strcmp(name, "debug")==0 || true) {
        static materialT* debug_material;

        if (!debug_material) {
            debug_material = createADSMaterial(NULL, NULL, NULL,
                                               (vec3) { 1.0f, 0.0f, 1.0f },
                                               (vec3) { 0.2f, 0.2f, 0.2f },
                                               (vec3) { 0.2f, 0.2f, 0.2f },
                                               10.0f);
        }

        material = debug_material;
    }

    if (strcmp(name, "blue crystal")==0) {
        static materialT* blue_crystal;

        if (!blue_crystal) {
            blue_crystal = createADSMaterial(NULL, NULL, NULL,
                                             (vec3) {  1.0f,  1.0f, 1.0f },
                                             (vec3) { -1.0f, -0.3f, 0.0f },
                                             (vec3) {  3.0f,  3.0f, 3.0f },
                                             30.0f);
        }

        material = blue_crystal;
    }

    if (strcmp(name, "shiny black")==0) {
        static materialT* shiny_black;

        if (!shiny_black) {
            shiny_black = createADSMaterial(NULL, NULL, NULL,
                                            (vec3) {  1.0f,  1.0f,  1.0f },
                                            (vec3) { -1.0f, -1.0f, -1.0f },
                                            (vec3) {  1.0f,  1.0f,  1.0f },
                                            10.0f);
        }

        material = shiny_black;
    }
    
    if (!material)
        error("Unknown material: '%s'", name);

    return (material);
}

void useMaterial(materialT* m) {
    useShader(m->shader);

    if (m->use_fn) m->use_fn(m);
}
