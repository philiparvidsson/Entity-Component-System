/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "physics_p.h"
#include "world.h"

#include "base/common.h"
#include "base/debug.h"
#include "math/aabb.h"
#include "math/integrate.h"
#include "math/matrix.h"
#include "math/shape.h"
#include "physics/body.h"

#include <stdlib.h>
#include <string.h>

// Specifies the integration function to use for integrating positions,
// velocities, etc.
#define IntegrateFn rk4Integrate

// The maximum number of subdivides to do for a single step attempt in the
// worldStep() function.
#define MaxSubdivs 8

// This struct is used as a return value for some functions. I think it's ok
// performance-wise. I believe that the ABI specifies that it will be returned
// on the caller's stack due to its size.
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

static inline bool pointInsideAABB(const vec2* p, const aabbT* aabb) {
    if ((p->x >= aabb->min.x)
     && (p->x  < aabb->max.x)
     && (p->y >= aabb->min.y)
     && (p->y  < aabb->max.y))
    {
        return (true);
    }

    return (false);
}

static collisionT findBodyBodyCollision(worldT* world, bodyT* a, bodyT* b) {
    collisionT collision = { 0 };

    collision.exists = false;

    // First, we do an AABB collision test since it's faster.

    aabbT a_aabb = bodyRotatedAABB(a);
    aabbT b_aabb = bodyRotatedAABB(b);

    if (a_aabb.max.x < b_aabb.min.x
     || a_aabb.min.x > b_aabb.max.x
     || a_aabb.max.y < b_aabb.min.y
     || a_aabb.min.y > b_aabb.max.y)
    {
        // Definitely not a collision, so we can return here.
        return (collision);
    }
    // The AABBs are colliding, so there might be a collision.

    // @To-do: I'm a lazy bastard and decided to only implement fine collision
    //         resolution for rectangles. Boo-hoo!
    
    // Require exactly 4 points and assume they represent a rectangle.
    assert(a->shape->num_points == 4);
    assert(b->shape->num_points == 4);

    aabbT aabb = bodyAABB(a);

    mat2x2 ra, rb;
    mat_rot_z(-a->state.o, &ra);
    mat_rot_z(b->state.o, &rb);

    int num_contacts = 0;

    for (int i = 0; i < 4; i++) {
        vec2 p = b->shape->points[i];

        vec_mat_mul(&p, &rb, &p);
        //vec_add    (&p, &b->state.x, &p);
        vec_sub    (&p, &a->state.x, &p);
        vec_mat_mul(&p, &ra, &p);

        if (pointInsideAABB(&p, &aabb)) {
            // We should probably clip here but I trust that the binary search
            // for time-of-collision is accurate enough that we don't need to.

            collision.exists = true;

            // Point p is in local space (body a frame of reference).
            vec_add(&p, &collision.contact, &collision.contact);


            num_contacts++;
        }
    }

    assert(!collision.exists);

    return (collision);
}

static collisionT findBodyWorldCollision(worldT* world, bodyT* body) {
    // We find the number of contacts that the object is making with the
    // world edges and then average them into a single contact point, at
    // which we then apply the collision impulse vector. Seems to work fine.

    collisionT c = { 0 };

    c.a      = body;
    c.exists = false;

    int num_contacts = 0;

    mat2x2 rotation;
    mat_rot_z(body->state.o, &rotation);

    for (int i = 0; i < body->shape->num_points; i++) {
        vec2 local_pos, world_pos;
        vec_mat_mul(&body->shape->points[i], &rotation , &local_pos);
        vec_add    (&body->state.x         , &local_pos, &world_pos);

        bool coll = false;
        if (world_pos.x < -2.0f) { coll = true; c.normal.x += 1.0f; }
        if (world_pos.x >  2.0f) { coll = true; c.normal.x -= 1.0f; }
        if (world_pos.y < -1.0f) { coll = true; c.normal.y += 1.0f; }
        if (world_pos.y >  1.0f) { coll = true; c.normal.y -= 1.0f; }

        if (coll) {
            num_contacts++;
            vec_add(&local_pos, &c.contact, &c.contact);
        }
    }

    if (num_contacts > 0) {
        vec_scale    (&c.contact, 1.0f/num_contacts, &c.contact);
        vec_normalize(&c.normal                    , &c.normal);

        c.exists = true;
    }

    return (c);
}

static int findCollisions(worldT* world) {
    arrayClear(world->collisions);

    bodyT* body = world->bodies;
    while (body) {
        collisionT c = findBodyWorldCollision(world, body);
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
            if (c.exists)
                arrayAdd(world->collisions, &c);

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
        //printf("before coll %4.2f %4.2f\n", c->a->state.v.x, c->a->state.v.y);
        bodyApplyImpulse(c->a, impulse, c->contact);
        //printf("after coll %4.2f %4.2f\n", c->a->state.v.x, c->a->state.v.y);
    }

}

static void derivativeFn(const float* state, float* derivs) {
    // state[0] = x.x
    // state[1] = x.y
    // state[2] = o
    // state[3] = v.x
    // state[4] = v.y
    // state[5] = w

    // derivs[0] = v.x
    // derivs[1] = v.y
    // derivs[2] = w
    // derivs[0] = a.x
    // derivs[1] = a.y
    // derivs[2] = t

    derivs[0] = state[3];
    derivs[1] = state[4];
    derivs[2] = state[5];

    derivs[3] =  0.0f;
    derivs[4] = -3.0f;
    derivs[5] =  0.0f;
}

void worldStep(worldT* world, float dt) {
    bodyT* body = world->bodies;
    while (body) {
        body->prev_state = body->state;
        body = body->next;
    }

    int a = 1;
    int b = 1;

    // Don't test against zero because of rounding errors!
    while (dt > (1.0/1000000.0f)) {
        float x = (float)a/(float)b;

        body = world->bodies;
        while (body) {
            body->state = body->prev_state;

            bodyStateT* s = &body->state;
            IntegrateFn(&s->x, &s->v, 6, dt*x, derivativeFn);

            body = body->next;
        }

        if (findCollisions(world) > 0) {
            // Collisions were found, so we double the value of b. If we've
            // reached the max value for b, we try to resolve them. Otherwise,
            // we go back and try to simulate half the time we did last
            // iteration.
            if ((b<<=1) == (1<<MaxSubdivs)) {
                resolveCollisions(world);

                body = world->bodies;
                while (body) {
                    body->prev_state.v = body->state.v;
                    body->prev_state.w = body->state.w;

                    body = body->next;
                }

                // Reset a and b to make sure we simulate the remainder of dt
                // with our next attempt.
                a = b = 1;
            }
        }
        else {
            body = world->bodies;
            while (body) {
                body->prev_state = body->state;
                body = body->next;
            }

            dt -= x*dt;
            a <<= 1;
        }
    }
}
