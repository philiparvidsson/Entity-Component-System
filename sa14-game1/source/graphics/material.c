#include "material.h"

#include "base/common.h"
#include "math/vector.h"

#include <string.h>

const materialT* defaultMaterial(void) {
    static const materialT default_material = {
        0.05f, 0.05f, 0.05f, // ambient
        0.75f, 0.75f, 0.75f, // diffuse
        1.00f, 1.00f, 1.00f, // specular
        1.00f                // shininess
    };

    return (&default_material);
}

const materialT* getNamedMaterial(const string* name) {
    if (strcmp(name, "default")==0)
        return (defaultMaterial());

    if (strcmp(name, "shiny black")==0) {
        static const materialT shiny_black = {
             0.0f,  0.0f,  0.0f, // ambient
             0.1f,  0.1f,  0.1f, // diffuse
            1.00f, 1.00f, 1.00f, // specular
            10.00f               // shininess
        };

        return (&shiny_black);
    }
    
    error("Unknown material");
    return (NULL);
}
