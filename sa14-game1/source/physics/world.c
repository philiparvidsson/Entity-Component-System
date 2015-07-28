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

    // @To-do: I'm a lazy bastard and decided to only implement fine collision
    //         resolution for rectangles. Boo-hoo!
    
    // Require exactly 4 points and assume they represent a rectangle.
    assert(a->shape->num_points == 4);
    assert(b->shape->num_points == 4);

    aabbT aabb;

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

typedef struct {
    vec2 dx;
    vec2 dv;
    float d_o;
    float dw;
} derivativeT;

static vec3 accelerationFunc(bodyStateT* state) {
    return (vec3) { 0.0f , -5.0f, 0.0f };
}

static derivativeT rk4Evaluate(bodyStateT* initial, derivativeT* d, float dt) {
    bodyStateT state;

    state.x.x = initial->x.x + d->dx.x * dt;
    state.x.y = initial->x.y + d->dx.y * dt;
    state.v.x = initial->v.x + d->dv.x * dt;
    state.v.y = initial->v.y + d->dv.y * dt;
    state.o   = initial->o   + d->d_o  * dt;
    state.w   = initial->w   + d->dw   * dt;

    derivativeT output;

    vec3 acc = accelerationFunc(&state);

    output.dx  = state.v;
    output.dv  = acc.xy;
    output.d_o = state.w;
    output.dw  = acc.z;

    return (output);
}

static void rk4Integrate(bodyT* body, float dt) {
    // Fourth-order Runge-Kutta integration.

    derivativeT empty = { 0 };

    derivativeT a = rk4Evaluate(&body->state, &empty, 0.0f   );
    derivativeT b = rk4Evaluate(&body->state, &a    , dt*0.5f);
    derivativeT c = rk4Evaluate(&body->state, &b    , dt*0.5f);
    derivativeT d = rk4Evaluate(&body->state, &c    , dt     );

    body->state.x.x += (1.0f/6.0f) * (a.dx.x + 2.0f*(b.dx.x + c.dx.x) + d.dx.x) * dt;
    body->state.x.y += (1.0f/6.0f) * (a.dx.y + 2.0f*(b.dx.y + c.dx.y) + d.dx.y) * dt;
    body->state.v.x += (1.0f/6.0f) * (a.dv.x + 2.0f*(b.dv.x + c.dv.x) + d.dv.x) * dt;
    body->state.v.y += (1.0f/6.0f) * (a.dv.y + 2.0f*(b.dv.y + c.dv.y) + d.dv.y) * dt;
    body->state.o   += (1.0f/6.0f) * (a.d_o  + 2.0f*(b.d_o  + c.d_o ) + d.d_o ) * dt;
    body->state.w   += (1.0f/6.0f) * (a.dw   + 2.0f*(b.dw   + c.dw  ) + d.dw  ) * dt;
}

static inline float lerp(float a, float b, float x) {
    return (a*x + b*(1.0f-x));
}

void worldStep(worldT* world, float dt) {
    bodyT* b = world->bodies;
    while (b) {
        bodyT* body = b;
        b = b->next;

        body->prev_state = body->state;
    }

    int counter = 16;

    float safe_x = 0.0f;
    float step   = 1.0f;

    while (dt > (1.0/1000000.0f)) {
        float x = safe_x + step;

        bodyT* b = world->bodies;
        while (b) {
            bodyT* body = b;
            b = b->next;

            bodyT* b = world->bodies;
            while (b) {
                bodyT* body = b;
                b = b->next;

                body->state = body->prev_state;

                rk4Integrate(body, dt*x);
            }
        }

        if (findCollisions(world) > 0) {
            x = safe_x;
            step *= 0.5f;

            counter--;
            if (counter < 0) {
                counter = 0;
                warn("couldn't resolve collisions");
            }

            if (counter == 0) {
                resolveCollisions(world);

                bodyT* b = world->bodies;
                while (b) {
                    bodyT* body = b;
                    b = b->next;

                    body->prev_state.v = body->state.v;
                    body->prev_state.w = body->state.w;
                }
            }
        }
        else {
            safe_x = x;

            bodyT* b = world->bodies;
            while (b) {
                bodyT* body = b;
                b = b->next;
        
                body->prev_state = body->state;
            }

            dt -= safe_x * dt;
        }
    }
}
