#include "playership.h"

#include "game/game.h"

#include <stdlib.h>

typedef struct {
    triMeshT* model;
    mat4x4 transform;

    bodyT* body;
} playerShipT;

static void cleanupFunc(gameObjectT* o) {
    if (!o->data)
        return;

    playerShipT* p = (playerShipT*)o->data;

    bodyFree(p->body);
    freeMesh(p->model);

    free(o->data);
    o->data = NULL;
}

static void drawFunc(gameObjectT* o) {
    playerShipT* p = o->data;

    vec3 pos = bodyGetPosition(p->body);
    mat_transl_xyz(pos.x, pos.y, pos.z, &p->transform);

    setShaderParam("Model", &p->transform);

    drawMesh(p->model);
}

static void updateFunc(gameObjectT* o) {
    playerShipT* p = (playerShipT*)o->data;
    vec3 pos = bodyGetPosition(p->body);

    if (keyIsPressed(&o->game->keyboard, ArrowLeft))  pos.x -= 0.01f;
    if (keyIsPressed(&o->game->keyboard, ArrowRight)) pos.x += 0.01f;
    if (keyIsPressed(&o->game->keyboard, ArrowUp))    pos.y += 0.01f;
    if (keyIsPressed(&o->game->keyboard, ArrowDown))  pos.y -= 0.01f;

    bodySetPosition(p->body, pos);
}

gameObjectT* createPlayerShip(void) {
    gameObjectT* o = calloc(1, sizeof(gameObjectT));

    o->cleanupFunc = cleanupFunc;
    o->drawFunc    = drawFunc;
    o->updateFunc  = updateFunc;

    o->data = malloc(sizeof(playerShipT));

    playerShipT* p = (playerShipT*)o->data;

    p->body  = bodyNew(1.0f * Kilogram);
    p->model = createBox(0.1f, 0.1f, 0.1f);

    mat_identity(&p->transform);

    return (o);
}
