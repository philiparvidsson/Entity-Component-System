#include "physics_p.h"
#include "body.h"

#include "base/common.h"
#include "math/vector.h"

#include <stdlib.h>
#include <string.h>

static bodyT* bodyAlloc(void) {
    bodyT* body = malloc(sizeof(bodyT));

    return (body);
}

static void bodyInit(bodyT* body, float mass) {
    memset(body, 0, sizeof(bodyT));

    bodySetMass(body, mass);
    bodySetType(body, DynamicBody);
}

bodyT* bodyNew(float mass) {
    bodyT* body = bodyAlloc();
    bodyInit(body, mass);

    return (body);
}

void bodyFree(bodyT* body) {
    if (body)
        free(body);
}

float bodyGetAngle(const bodyT* body)              { return (body->state.o); }
void  bodySetAngle(      bodyT* body, float angle) { body->state.o = angle;  }

float bodyGetMass(const bodyT* body)             { return (1.0f/body->inv_mass); }
void  bodySetMass(      bodyT* body, float mass) { body->inv_mass = 1.0f/mass;   }

vec2 bodyGetPosition(const bodyT* body)           { return (body->state.x); }
void bodySetPosition(      bodyT* body, vec2 pos) { body->state.x = pos;    }

bodyTypeT bodyGetType(const bodyT* body)           { return (body->type); }
void      bodySetType(      bodyT* body, int type) { body->type = type;   }

vec2 bodyGetVelocity(const bodyT* body)           { return (body->state.v); }
void bodySetVelocity(      bodyT* body, vec2 vel) { body->state.v = vel;    }

void bodyApplyForce(bodyT* body, vec2 f, vec2 r) {
    vec_scale(&f, body->inv_mass, &f);
    vec_add  (&body->state.a, &f, &body->state.a);
}
