#include "playership.h"

#include "game/game.h"

#include "gfx/trimesh.h"

#include "input/keyboard.h"

#include "math/matrix.h"

#include "physics/body.h"

#include <stdlib.h>

typedef struct {
    triMeshT* model;
    mat4x4 transform;

    bodyT* body;
} playerShipT;

static void cleanupFunc(entityT* e) {
    playerShipT* p = (playerShipT*)entityGetData(e);

    bodyFree(p->body);
    freeMesh(p->model);
}

static void drawFunc(entityT* e) {
    playerShipT* p = (playerShipT*)entityGetData(e);

    vec3 pos = bodyGetPosition(p->body);
    mat_transl_xyz(pos.x, pos.y, pos.z, &p->transform);

    setShaderParam("Model", &p->transform);

    drawMesh(p->model);
}

static void updateFunc(entityT* e) {
    playerShipT* p = (playerShipT*)entityGetData(e);
    vec3 pos = bodyGetPosition(p->body);

    if (keyIsPressed(&getGameInst()->keyboard, ArrowLeft))  pos.x -= 0.01f;
    if (keyIsPressed(&e->game->keyboard, ArrowRight)) pos.x += 0.01f;
    if (keyIsPressed(&e->game->keyboard, ArrowUp))    pos.y += 0.01f;
    if (keyIsPressed(&e->game->keyboard, ArrowDown))  pos.y -= 0.01f;

    bodySetPosition(p->body, pos);
}

entityT* createPlayerShip(void) {
    entityT* e = entityNew();

    e->cleanupFunc = cleanupFunc;
    e->drawFunc    = drawFunc;
    e->updateFunc  = updateFunc;

    e->data = malloc(sizeof(playerShipT));

    playerShipT* p = (playerShipT*)o->data;

    p->body  = bodyNew(1.0f * Kilogram);
    p->model = createBox(0.1f, 0.1f, 0.1f);

    mat_identity(&p->transform);

    return (e);
}
