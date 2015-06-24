/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "world.h"

#include "core/array.h"
#include "core/common.h"
#include "core/debug.h"

#include "physics/physics_p.h"

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

    if (world->first_body)
        world->first_body->prev = body;

    body->next = world->first_body;
    world->first_body = body;
}

void worldStep(worldT* world, float dt) {
    bodyT* b = world->first_body;

    while (b) {
        bodyT* body = b;
        b = b->next;

        // Static bodies do not move.
        if (body->type == StaticBody)
            continue;

        body->vel.x += body->acc.x * dt;
        body->vel.y += body->acc.y * dt;
        body->vel.z += body->acc.z * dt;
        body->pos.x += body->vel.x * dt;
        body->pos.y += body->vel.y * dt;
        body->pos.z += body->vel.z * dt;
        body->acc.x = body->acc.y = body->acc.z = 0.0f;

        /*if (body->pos.x + body->aabb.min.x < world->bounds.min.x) {
            body->vel.x *= -1.0f;
            body->pos.x = world->bounds.min.x - body->aabb.min.x;
        }
        else if (body->pos.x + body->aabb.max.x > world->bounds.max.x) {
            body->vel.x *= -1.0f;
            body->pos.x = world->bounds.max.x - body->aabb.max.x;
        }

        if (body->pos.y + body->aabb.min.y < world->bounds.min.y) {
            body->vel.y *= -1.0f;
            body->pos.y = world->bounds.min.y - body->aabb.min.y;
        }
        else if (body->pos.y + body->aabb.max.y > world->bounds.max.y) {
            body->vel.y *= -1.0f;
            body->pos.y = world->bounds.max.y - body->aabb.max.y;
        }

        if (body->pos.z + body->aabb.min.z < world->bounds.min.z) {
            body->vel.z *= -1.0f;
            body->pos.z = world->bounds.min.z - body->aabb.min.z;
        }
        else if (body->pos.z + body->aabb.max.z > world->bounds.max.z) {
            body->vel.z *= -1.0f;
            body->pos.z = world->bounds.max.z - body->aabb.max.z;
        }*/
    }
}
