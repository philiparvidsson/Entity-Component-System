#include "material.h"

#include "base/common.h"
#include "math/vector.h"
#include "graphics/material.h"
#include "graphics/materials/adsmaterial.h"
#include "graphics/materials/refractmaterial.h"

#include <string.h>

static materialT* active_material = NULL;

materialT* newMaterial(void) {
    materialT* m = malloc(sizeof(materialT));

    return (m);
}

const materialT* getNamedMaterial(const string* name) {
    materialT* material = NULL;

    // This is a debug material to visualize objects more clearly.
    if (strcmp(name, "debug")==0) {
        static materialT* debug_material;

        if (!debug_material) {
            debug_material = createADSMaterial(NULL, NULL, NULL,
                                               (vec3) { 1.0f, 0.0f, 1.0f },
                                               (vec3) { 0.2f, 0.2f, 0.2f },
                                               (vec3) { 0.2f, 0.2f, 0.2f },
                                               10.0f);

            debug_material->name = "debug";
        }

        material = debug_material;
    }
    else if (strcmp(name, "blue crystal")==0) {
        static materialT* blue_crystal;

        if (!blue_crystal) {
            blue_crystal = createRefractMaterial((vec3) {  0.0f,  0.7f, 1.0f },
                                                 50.0f);

            blue_crystal->name = "blue crystal";
        }

        material = blue_crystal;
    }
    else if (strcmp(name, "shiny black")==0) {
        static materialT* shiny_black;

        if (!shiny_black) {
            shiny_black = createADSMaterial(NULL, NULL, NULL,
                                            (vec3) {  1.0f,  1.0f,  1.0f },
                                            (vec3) { -1.0f, -1.0f, -1.0f },
                                            (vec3) {  1.0f,  1.0f,  1.0f },
                                            10.0f);

            shiny_black->name = "shiny black";
        }

        material = shiny_black;
    }
    
    if (!material)
        error("unknown material: '%s'", name);

    return (material);
}

materialT* activeMaterial(void) {
    return (active_material);
}

void useMaterial(materialT* m) {
    if (m == active_material)
        return;

    if (active_material) {
        if (active_material->end_fn)
            active_material->end_fn(active_material);
    }

    active_material = m;

    if (m) {
        useShader(m->shader);
        if (m->begin_fn)
            m->begin_fn(m);
    }
}
