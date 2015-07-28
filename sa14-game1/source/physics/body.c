#include "physics_p.h"
#include "body.h"

#include "base/common.h"
#include "math/aabb.h"
#include "math/matrix.h"
#include "math/shape.h"
#include "math/vector.h"

#include <stdlib.h>
#include <string.h>

static bodyT* bodyAlloc(void) {
    bodyT* body = malloc(sizeof(bodyT));

    return (body);
}

static void bodyInit(bodyT* body, shapeT* shape, float mass) {
    memset(body, 0, sizeof(bodyT));

    body->shape       = shape;
    body->inv_mass    = 1.0f/mass;
    body->inv_inertia = 1.0f / 0.015f;
    body->restitution = 1.0f;
    body->type        = DynamicBody;
}

bodyT* bodyNew(shapeT* shape, float mass) {
    bodyT* body = bodyAlloc();
    bodyInit(body, shape, mass);

    return (body);
}

bodyT* bodyNewSquare(float width, float height, float mass) {
    shapeT* shape = shapeNewSquare(width, height);

    return (bodyNew(shape, mass));
}

void bodyFree(bodyT* body) {
    if (body)
        free(body);
}

aabbT bodyAABB(const bodyT* body) {
    aabbT aabb = { 0 };

    mat2x2 r;
    mat_rot_z(body->state.o, &r);

    for (int i = 0; i < body->shape->num_points; i++) {
        vec2 p;
        vec_mat_mul(&body->shape->points[i], &r, &p);

        aabb.min.x = min(aabb.min.x, p.x);
        aabb.min.y = min(aabb.min.y, p.y);
        aabb.max.x = max(aabb.max.x, p.x);
        aabb.max.y = max(aabb.max.y, p.y);
    }

    vec_add(&body->state.x, &aabb.min, &aabb.min);
    vec_add(&body->state.x, &aabb.max, &aabb.max);

    return (aabb);
}

float bodyOrientation   (const bodyT* body)              { return (body->state.o); }
void  bodySetOrientation(      bodyT* body, float angle) { body->state.o = angle;  }

float bodyMass   (const bodyT* body)             { return (1.0f/body->inv_mass); }
void  bodySetMass(      bodyT* body, float mass) { body->inv_mass = 1.0f/mass;   }

vec2 bodyPosition   (const bodyT* body)           { return (body->state.x); }
void bodySetPosition(      bodyT* body, vec2 pos) { body->state.x = pos;    }

bodyTypeT bodyType   (const bodyT* body)           { return (body->type); }
void      bodySetType(      bodyT* body, int type) { body->type = type;   }

vec2 bodyVelocity   (const bodyT* body)           { return (body->state.v); }
void bodySetVelocity(      bodyT* body, vec2 vel) { body->state.v = vel;    }

void bodyApplyForce(bodyT* body, vec2 f, vec2 r) {
    vec_scale(&f, body->inv_mass, &f);
    vec_add  (&body->state.a, &f, &body->state.a);
}

void bodyApplyImpulse(bodyT* body, vec2 i, vec2 r) {
    vec2 a = i;
    vec_scale(&a, body->inv_mass, &a);
    vec_add  (&a, &body->state.v, &body->state.v);

    float b = vec_perp_dot(&r, &i);
    body->state.w += b * body->inv_inertia;
}
