/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "physics_p.h"
#include "world.h"

#include "base/common.h"
#include "base/debug.h"
#include "math/matrix.h"
#include "physics/body.h"

#include <stdlib.h>
#include <string.h>

#define MaxAttempts (8)


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

static bool collisionsExist(worldT* world) {
    bodyT* body = world->bodies;
    while (body) {
        mat2x2 orientation;
        mat_rot_z(body->state.o, &orientation);

        for (int i = 0; i < body->shape->num_points; i++) {
            vec2 local_pos, world_pos;
            vec_mat_mul(&body->shape->points[i], &orientation, &local_pos);
            vec_add    (&body->state.x         , &local_pos  , &world_pos);

            if (world_pos.x < -2.0f) return (true);
            if (world_pos.x >  2.0f) return (true);
            if (world_pos.y < -1.0f) return (true);
            if (world_pos.y >  1.0f) return (true);
        }

        body = body->next;
    }

    return (false);
}

static void resolveWorldEdgeCollisions(worldT* world) {
    bodyT* body = world->bodies;
    while (body) {
        vec2 impulse = { 0 };
        int num_contacts = 0;
        vec2 contacts[MaxShapePoints];

        mat2x2 orientation;
        mat_rot_z(body->state.o, &orientation);

        for (int i = 0; i < body->shape->num_points; i++) {
            vec2 local_pos, world_pos;
            vec_mat_mul(&body->shape->points[i], &orientation, &local_pos);
            vec_add    (&body->state.x         , &local_pos  , &world_pos);

            vec2 normal;

            bool collision_found = false;
            if (world_pos.x < -2.0f) {
                collision_found = true;
                normal = (vec2) { 1.0f, 0.0f };
            }
            else if (world_pos.x > 2.0f) {
                collision_found = true;
                normal = (vec2) { -1.0f, 0.0f };
            }
            else if (world_pos.y < -1.0f) {
                collision_found = true;
                normal = (vec2) { 0.0f, 1.0f };
            }
            else if (world_pos.y > 1.0f) {
                collision_found = true;
                normal = (vec2) { 0.0f, -1.0f };
            }

            if (collision_found) {
                vec2 v = body->state.v;
                v.x += -local_pos.y * body->state.w;
                v.y +=  local_pos.x * body->state.w;

                vec_perp (&local_pos        , &v);
                vec_scale(&v,  body->state.w, &v);
                vec_add  (&v, &body->state.v, &v);

                float j = -(1 + body->restitution) * vec_dot(&v, &normal)
                    /
                    (body->inv_mass + powf(vec_perp_dot(&normal, &local_pos), 2.0f) * body->inv_inertia);

                impulse.x += j*normal.x;
                impulse.y += j*normal.y;

                contacts[num_contacts++] = local_pos;
            }
        }

        if (num_contacts > 0) {
            vec_scale(&impulse, 1.0f / num_contacts, &impulse);
            for (int i = 0; i < num_contacts; i++) {
                bodyApplyImpulse(body, impulse, contacts[i]);
            }
        }

        body = body->next;
    }
}

static void resolveCollisions(worldT* world) {
    resolveWorldEdgeCollisions(world);
}

void worldStep(worldT* world, float dt2) {
    bodyT* b = world->bodies;
    while (b) {
        bodyT* body = b;
        b = b->next;

        body->state.a.x = -3.81f;

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

        if (collisionsExist(world)) {
            // There are collisions. If this is our last attempt, we must
            // resolve them. Otherwise, we can increase our attempt counter and
            // hope to simulate just a little bit more before encountering them,
            // giving our simulation higher precision.

            num_attempts++;

            if (num_attempts > MaxAttempts) {
                //warn("collisions were not resolved");
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

#ifdef _DEBUG
    assert(!collisionsExist(world));
#endif // _DEBUG
}
