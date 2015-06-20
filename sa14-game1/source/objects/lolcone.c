#include "lolcone.h"

#include "gfx/trimesh.h"

static void lolConeUpdate(gameObjectT *cone) {
    static float ff;

    ff += 0.005f;

    mat_rot_y(ff, &cone->transform);
}

void createLolCone(gameObjectT *o) {
    o->mesh = createBox(0.3f, 0.3f, 0.3f);
    o->update = lolConeUpdate;
}
