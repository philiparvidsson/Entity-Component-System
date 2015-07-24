/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "physics_p.h"
#include "world.h"

#include "base/common.h"
#include "base/debug.h"

#include <stdlib.h>
#include <string.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

worldT* worldAlloc(void) {
    worldT* world = malloc(sizeof(worldT));

    return (world);
}

void worldInit(worldT* world) {
    memset(world, 0, sizeof(worldT));
}

worldT* worldNew(void) {
    worldT* world = worldAlloc();
    worldInit(world);

    return (world);
}

void worldFree(worldT* world) {
    if (world)
        free(world);
}

void worldAddBody(worldT* world, bodyT* body) {
    assert(body->world == NULL);

    body->world = world;

    if (world->bodies)
        world->bodies->prev = body;

    body->next = world->bodies;
    world->bodies = body;
}

void worldStep(worldT* world, float dt) {
    bodyT* b = world->bodies;
    while (b) {
        bodyT* body = b;
        b = b->next;

        body->prev_state = body->state;
    }

    b = world->bodies;
    while (b) {
        bodyT* body = b;
        b = b->next;

        // Static bodies do not move.
        if (body->type == StaticBody)
            continue;

        body->state.v.x += body->state.a.x * dt;
        body->state.v.y += body->state.a.y * dt;
        body->state.x.x += body->state.v.x * dt;
        body->state.x.y += body->state.v.y * dt;

        body->state.a.x = body->state.a.y = 0.0f;
    }
}
