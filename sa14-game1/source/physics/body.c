#include "physics_p.h"
#include "body.h"

#include "core/common.h"

#include "math/vector.h"

#include <stdlib.h>
#include <string.h>

bodyT* bodyAlloc(void) {
    bodyT* body = malloc(sizeof(bodyT));

    return (body);
}

void bodyInit(bodyT* body, float mass) {
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

float bodyGetMass(const bodyT* body) {
    return (body->mass);
}

void bodySetMass(bodyT* body, float mass) {
    body->mass = mass;
}

vec3 bodyGetPosition(const bodyT* body) {
    return (body->pos);
}

void bodySetPosition(bodyT* body, vec3 pos) {
    body->pos = pos;
}

vec3 bodyGetVelocity(const bodyT* body) {
    return (body->vel);
}

void bodySetVelocity(bodyT* body, vec3 vel) {
    body->vel = vel;
}

bodyTypeT bodyGetType(const bodyT* body) {
    return (body->type);
}

void bodySetType(bodyT* body, bodyTypeT type) {
    body->type = type;
}

void bodyApplyForce(bodyT* body, vec3 force) {
    vec_scale(&force, 1.0f / body->mass, &force);
    vec_add(&body->acc, &force, &body->acc);
}
