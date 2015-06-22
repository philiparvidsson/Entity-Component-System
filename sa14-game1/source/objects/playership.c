#include "game.h"

#include "gfx/trimesh.h"

static void playerShipUpdate(gameObjectT *o) {
    static float ff;

    ff += 0.005f;

    mat_rot_y(ff, &o->transform);
}

void createPlayerShip(gameObjectT *o) {
    o->mesh = createBox(0.1, 0.1f, 0.1f);
    o->update = playerShipUpdate;
}
