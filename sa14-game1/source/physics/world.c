/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "physics_p.h"
#include "world.h"

#include "base/common.h"
#include "base/debug.h"
#include "math/matrix.h"
#include "math/shape.h"
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
        // We find the number of contacts that the object is making with the
        // world edges and then average them into a single contact point, at
        // which we then apply the collision impulse vector.

        int num_contacts = 0;

        vec2 contact = { 0 };
        vec2 normal  = { 0 };

        mat2x2 rotation;
        mat_rot_z(body->state.o, &rotation);

        for (int i = 0; i < body->shape->num_points; i++) {
            vec2 local_pos, world_pos;
            vec_mat_mul(&body->shape->points[i], &rotation , &local_pos);
            vec_add    (&body->state.x         , &local_pos, &world_pos);

            // The variable f indicates whether there is a contact.
            bool coll = false;
            if (world_pos.x < -2.0f) { coll = true; normal.x += 1.0f; }
            if (world_pos.x >  2.0f) { coll = true; normal.x -= 1.0f; }
            if (world_pos.y < -1.0f) { coll = true; normal.y += 1.0f; }
            if (world_pos.y >  1.0f) { coll = true; normal.y -= 1.0f; }

            if (coll) {
                num_contacts++;
                vec_add(&contact, &local_pos, &contact);
            }
        }

        if (num_contacts > 0) {
            vec_scale(&contact, 1.0f / num_contacts, &contact);

            vec2 v = body->state.v;
            v.x += -contact.y * body->state.w;
            v.y +=  contact.x * body->state.w;

            vec_perp     (&contact          , &v);
            vec_scale    (&v,  body->state.w, &v);
            vec_add      (&v, &body->state.v, &v);
            vec_normalize(&normal, &normal);

            float inv_mass    = body->inv_mass;
            float inv_inertia = powf(vec_perp_dot(&contact, &normal), 2.0f);

            float j  = -(1.0f + body->restitution)*vec_dot(&v, &normal);
                  j /= inv_mass + inv_inertia*body->inv_inertia;

            vec2 impulse = (vec2) { j*normal.x, j*normal.y };
            bodyApplyImpulse(body, impulse, contact);
        }

        body = body->next;
    }
}

static void resolveObjectCollisions(worldT* world) {
    bodyT* a_body = world->bodies;
    while (a_body) {
        bodyT* b_body = world->bodies;
        while (b_body) {
            mat2x2 a_r, b_r;
            mat_rot_z(a_body->state.o, &a_r);
            mat_rot_z(b_body->state.o, &b_r);

            vec2 p = b_body->state.x;
            vec_sub(&p, &a_body->state.x, &p);

            

            b_body = b_body->next;
        }


        a_body = a_body->next;
    }
}

static void resolveCollisions(worldT* world) {
    resolveWorldEdgeCollisions(world);
    resolveObjectCollisions(world);
}

void worldStep(worldT* world, float dt2) {
    bodyT* b = world->bodies;
    while (b) {
        bodyT* body = b;
        b = b->next;

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

#ifdef _DEBUG
    assert(!collisionsExist(world));
#endif // _DEBUG
}
