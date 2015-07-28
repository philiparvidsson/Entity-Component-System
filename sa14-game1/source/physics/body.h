#ifndef body_h_
#define body_h_

#include "base/common.h"
#include "math/aabb.h"
#include "math/shape.h"
#include "math/vector.h"

typedef struct bodyT bodyT;

typedef enum {
    DynamicBody,
    StaticBody
} bodyTypeT;


bodyT* bodyNew      (shapeT* shape, float mass);
bodyT* bodyNewSquare(float width, float height, float mass);
void   bodyFree     (bodyT* body);

aabbT bodyAABB(const bodyT* body);

float bodyOrientation   (const bodyT* body             );
void  bodySetOrientation(      bodyT* body, float angle);

float bodyMass   (const bodyT* body            );
void  bodySetMass(      bodyT* body, float mass);

vec2 bodyPosition   (const bodyT* body          );
void bodySetPosition(      bodyT* body, vec2 pos);

bodyTypeT bodyType   (const bodyT* body          );
void      bodySetType(      bodyT* body, int type);

vec2 bodyVelocity   (const bodyT* body          );
void bodySetVelocity(      bodyT* body, vec2 vel);

void bodyApplyForce  (bodyT* body, vec2 f, vec2 r);
void bodyApplyImpulse(bodyT* body, vec2 i, vec2 r);

#endif
