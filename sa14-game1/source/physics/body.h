#ifndef body_h_
#define body_h_

#include "base/common.h"
#include "math/vector.h"
#include "physics/physics.h"

typedef enum {
    DynamicBody,
    StaticBody
} bodyTypeT;

bodyT* bodyNew(float mass);
void bodyFree(bodyT* body);

float bodyGetAngle(const bodyT* body);
void  bodySetAngle(      bodyT* body, float angle);

float bodyGetMass(const bodyT* body            );
void  bodySetMass(      bodyT* body, float mass);

vec2 bodyGetPosition(const bodyT* body          );
void bodySetPosition(      bodyT* body, vec2 pos);

bodyTypeT bodyGetType(const bodyT* body          );
void      bodySetType(      bodyT* body, int type);

vec2 bodyGetVelocity(const bodyT* body          );
void bodySetVelocity(      bodyT* body, vec2 vel);

void bodyApplyForce(bodyT* body, vec2 f, vec2 r);

#endif
