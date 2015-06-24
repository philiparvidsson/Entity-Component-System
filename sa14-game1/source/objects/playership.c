#include "playership.h"

#include "game/game.h"

#include <stdlib.h>

typedef struct {
    triMeshT* model;
    mat4x4 transform;

    bodyT* body;
} playerShipT;

static void cleanupFunc(gameObjectT* o) {

    if (o->data) {
        playerShipT* p = o->data;

        bodyFree(p->body);
        freeMesh(p->model);

        free(o->data);
        o->data = NULL;
    }
}

static void drawFunc(gameObjectT* o) {
    playerShipT* p = o->data;

    setShaderParam("Model", &p->transform);
}

static void updateFunc(gameObjectT* o) {
    if (keyIsPressed(&o->game->keyboard, ArrowLeft)) {
        playerShipT* p = o->data;
        vec3 pos = bodyGetPosition(p->body);
        pos.x -= 0.01f;
        bodySetPosition(p->body, pos);
    }
}

gameObjectT* createPlayerShip(void) {
    gameObjectT* o = calloc(1, sizeof(gameObjectT));

    o->cleanupFunc = cleanupFunc;
    o->drawFunc    = drawFunc;
    o->updateFunc  = updateFunc;

    o->data = malloc(sizeof(playerShipT));

    playerShipT* p = o->data;

    p->body  = bodyNew(1.0f * Kilogram);
    p->model = createBox(0.1f, 0.1f, 0.1f);

    return (o);
}
