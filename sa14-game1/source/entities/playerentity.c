#include "playerentity.h"

#include "game/game.h"

#include "graphics/trimesh.h"

#include "input/keyboard.h"

#include "math/matrix.h"

#include "physics/body.h"

#include <stdlib.h>

typedef struct {
    triMeshT* model;
    mat4x4 transform;

    bodyT* body;
} playerShipT;

static void playerCleanup(entityT* e) {
    playerShipT* p = (playerShipT*)entityGetDataPtr(e);

    bodyFree(p->body);
    freeMesh(p->model);
    free(p);

    entitySetDataPtr(e, NULL);
}

static void playerDraw(entityT* e) {
    playerShipT* p = (playerShipT*)entityGetDataPtr(e);

    vec3 pos = bodyGetPosition(p->body);
    mat_transl_xyz(pos.x, pos.y, pos.z, &p->transform);


    setShaderParam("Model", &p->transform);

    drawMesh(p->model);
}

static void playerUpdate(entityT* e, float dt) {
    playerShipT* p = (playerShipT*)entityGetDataPtr(e);
    vec3 pos = bodyGetPosition(p->body);

    if (keyIsPressed(ArrowLeft))  pos.x -= 0.01f;
    if (keyIsPressed(ArrowRight)) pos.x += 0.01f;
    if (keyIsPressed(ArrowUp))    pos.y += 0.01f;
    if (keyIsPressed(ArrowDown))  pos.y -= 0.01f;

    bodySetPosition(p->body, pos);
}

entityT* createPlayerShip(void) {
    entityT* entity = entityNew();
    playerShipT* p = malloc(sizeof(playerShipT));

    entitySetDataPtr    (entity, p);
    entitySetCleanupFunc(entity, playerCleanup);
    entitySetDrawFunc   (entity, playerDraw);
    entitySetUpdateFunc (entity, playerUpdate);

    p->body  = bodyNew(1.0f * Kilogram);
    p->model = createBox(0.1f, 0.1f, 0.1f);

    mat_identity(&p->transform);

    return (entity);
}
