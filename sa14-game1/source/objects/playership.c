#include "game.h"

#include "gfx/trimesh.h"

#include "physics/physics.h"

static void playerShipCleanup(gameObjectT* o) {
    bodyFree(o->body);
    o->body = NULL;

    freeMesh(o->model);
    o->model = NULL;
}

static void playerShipUpdate(gameObjectT* o) {
    vec3 vel = (vec3) { 0.0f, 0.0f, 0.0f };


    if (keyIsPressed(&o->game->keyboard, ArrowLeft)) vel.x = -0.3f;
    else if (keyIsPressed(&o->game->keyboard, ArrowRight)) vel.x = 0.3f;

    if (keyIsPressed(&o->game->keyboard, ArrowUp)) vel.y = 0.3f;
    else if (keyIsPressed(&o->game->keyboard, ArrowDown)) vel.y = -0.3f;

    bodySetVelocity(o->body, vel);
}

void createPlayerShip(gameObjectT* o) {
    o->body        = bodyNew(1.0f * Kilogram);
    o->model       = createBox(0.1f, 0.1f, 0.1f);
    o->cleanupFunc = playerShipCleanup;
    o->updateFunc  = playerShipUpdate;
}
