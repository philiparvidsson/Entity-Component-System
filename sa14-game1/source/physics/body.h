#ifndef body_h_
#define body_h_

#include "core/common.h"

#include "math/vector.h"

#include "physics/physics.h"

typedef enum {
    DynamicBody,
    StaticBody
} bodyTypeT;

bodyT* bodyAlloc(void);
void bodyInit(bodyT* body, float mass);
bodyT* bodyNew(float mass);
void bodyFree(bodyT* body);
float bodyGetMass(const bodyT* body);
vec3 bodyGetPosition(const bodyT* body);
void bodySetPosition(bodyT* body, vec3 pos);
vec3 bodyGetVelocity(const bodyT* body);
void bodySetVelocity(bodyT* body, vec3 vel);
void bodySetMass(bodyT* body, float mass);
bodyTypeT bodyGetType(const bodyT* body);
void bodySetType(bodyT* body, bodyTypeT type);
void bodyApplyForce(bodyT* body, vec3 force);

#endif
