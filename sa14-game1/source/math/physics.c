#include "physics.h"

void step(particleT *p, float dt) {
    p->vel.x += p->acc.x*dt;
    p->vel.y += p->acc.y*dt;
    p->pos.x += p->vel.x*dt;
    p->pos.y += p->vel.y*dt;
    p->acc.x = p->acc.y = 0.0f;
}
