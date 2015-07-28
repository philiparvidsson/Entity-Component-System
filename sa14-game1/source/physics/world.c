/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "physics_p.h"
#include "world.h"

#include "base/common.h"
#include "base/debug.h"
#include "math/aabb.h"
#include "math/matrix.h"
#include "math/shape.h"
#include "physics/body.h"

#include <stdlib.h>
#include <string.h>

#define MaxAttempts (8)

typedef struct {
    bool exists;
    vec2 normal;
    vec2 contact;
    bodyT* a;
    bodyT* b;
} collisionT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

worldT* worldAlloc(void) {
    worldT* world = calloc(1, sizeof(worldT));

    world->collisions = arrayNew(sizeof(collisionT));

    return (world);
}

void worldInit(worldT* world) {
    //memset(world, 0, sizeof(worldT));
}

worldT* worldNew(void) {
    worldT* world = worldAlloc();
    worldInit(world);

    return (world);
}

void worldFree(worldT* world) {
    if (!world)
        return;

    if (world->collisions) {
        arrayFree(world->collisions);
        world->collisions = NULL;
    }

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

static void findBodyBodyContacts(bodyT* a, bodyT* b) {
    for (int i0 = 0; i0 < a->shape->num_points; i0++) {
        int i1 = (i0+1) % a->shape->num_points;

        vec2 p = a->shape->points[i0];
        vec2 q = a->shape->points[i1];

        for (int j = 0; j < b->shape->num_points; j++) {
        }
    }
}

static collisionT findBodyBodyCollision(worldT* world, bodyT* a, bodyT* b) {
    collisionT collision = { 0 };

    collision.exists = false;

    // First, we do an AABB collision test since it's faster.

    aabbT a_aabb = bodyAABB(a);
    aabbT b_aabb = bodyAABB(b);

    if (a_aabb.max.x < b_aabb.min.x
     || a_aabb.min.x > b_aabb.max.x
     || a_aabb.max.y < b_aabb.min.y
     || a_aabb.min.y > b_aabb.max.y)
    {
        // Definitely not a collision, so we can return here.
        return (collision);
    }
    // The AABBs are colliding, so there might be a collision.

    return (collision);
}

static collisionT findWorldEdgeCollision(worldT* world, bodyT* body) {
    // We find the number of contacts that the object is making with the
    // world edges and then average them into a single contact point, at
    // which we then apply the collision impulse vector.

    collisionT collision = { 0 };

    collision.a      = body;
    collision.exists = false;

    int num_contacts = 0;

    mat2x2 rotation;
    mat_rot_z(body->state.o, &rotation);

    for (int i = 0; i < body->shape->num_points; i++) {
        vec2 local_pos, world_pos;
        vec_mat_mul(&body->shape->points[i], &rotation , &local_pos);
        vec_add    (&body->state.x         , &local_pos, &world_pos);

        bool coll = false;
        if (world_pos.x < -2.0f) { coll = true; collision.normal.x += 1.0f; }
        if (world_pos.x >  2.0f) { coll = true; collision.normal.x -= 1.0f; }
        if (world_pos.y < -1.0f) { coll = true; collision.normal.y += 1.0f; }
        if (world_pos.y >  1.0f) { coll = true; collision.normal.y -= 1.0f; }

        if (coll) {
            num_contacts++;
            vec_add(&local_pos, &collision.contact, &collision.contact);
        }
    }

    if (num_contacts > 0) {
        vec_scale(&collision.contact, 1.0f/num_contacts, &collision.contact);
        vec_normalize(&collision.normal, &collision.normal);

        collision.exists = true;
    }

    return (collision);
}

static int findCollisions(worldT* world) {
    arrayClear(world->collisions);

    bodyT* body = world->bodies;
    while (body) {
        collisionT c = findWorldEdgeCollision(world, body);

        if (c.exists)
            arrayAdd(world->collisions, &c);


        bodyT* other_body = world->bodies;
        while (other_body) {
            if (other_body == body) {
                other_body = other_body->next;
                continue;
            }

            //memset(&c, 0, sizeof(collisionT));
            c = findBodyBodyCollision(world, body, other_body);

            other_body = other_body->next;
        }

        body = body->next;
    }

    return arrayLength(world->collisions);
}

static void resolveCollisions(worldT* world) {
    for (int i = 0; i < arrayLength(world->collisions); i++) {
        collisionT* c = arrayGet(world->collisions, i);

        vec2 v = c->a->state.v;
        v.x += -c->contact.y * c->a->state.w;
        v.y +=  c->contact.x * c->a->state.w;

        vec_perp (&c->contact       , &v);
        vec_scale(&v, c->a->state.w, &v);
        vec_add  (&v, &c->a->state.v, &v);

        float inv_mass    = c->a->inv_mass;
        float inv_inertia = powf(vec_perp_dot(&c->contact, &c->normal), 2.0f);

        float j  = -(1.0f + c->a->restitution)*vec_dot(&v, &c->normal);
              j /= inv_mass + inv_inertia*c->a->inv_inertia;

        vec2 impulse = (vec2) { j*c->normal.x, j*c->normal.y };
        bodyApplyImpulse(c->a, impulse, c->contact);
    }
}

void worldStep(worldT* world, float dt2) {
    bodyT* b = world->bodies;
    while (b) {
        bodyT* body = b;
        b = b->next;
        
        //body->state.a.y = -3.0f;
        body->prev_state = body->state;
    }

    int counter      = pow(2, MaxAttempts);
    int num_attempts = 0;

    while (counter) {
        bool collision_found = false;

        float dt = dt2 / pow(2, num_attempts);

        //----------------------------------------
        // 1. Step world forward in time.
        //----------------------------------------

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
            body->state.w   += body->state.t   * dt;
            body->state.o   += body->state.w   * dt;

            body->state.a.x = body->state.a.y = 0.0f;
            body->state.t   = 0.0f;
        }

        //----------------------------------------
        // 2. Handle collisions (if any).
        //----------------------------------------

        if (findCollisions(world) > 0) {
            // There are collisions. If this is our last attempt, we must
            // resolve them. Otherwise, we can increase our attempt counter and
            // hope to simulate just a little bit more before encountering them,
            // giving our simulation higher precision.

            num_attempts++;

            if (num_attempts > MaxAttempts) {
                warn("collisions were not resolved");
                num_attempts = MaxAttempts;
            }

            if (num_attempts == MaxAttempts)
                resolveCollisions(world);

            bodyT* b = world->bodies;
            while (b) {
                bodyT* body = b;
                b = b->next;

                body->state.x = body->prev_state.x;
                body->state.o = body->prev_state.o;
                body->state.a = body->prev_state.a;
                body->state.t = body->prev_state.t;
            }
        }
        else {
            // We managed to simulate a step (though not necessarily a full one)
            // without encountering any collisions.

            bodyT* b = world->bodies;
            while (b) {
                bodyT* body = b;
                b = b->next;

                body->prev_state = body->state;
            }
            
            counter -= (int)pow(2, MaxAttempts) >> num_attempts;
        }
    }
}
