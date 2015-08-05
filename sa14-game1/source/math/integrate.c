/*#include "integrate.h"

#include "base/common.h"

typedef struct {
    float v[16];
    float a[16];
} derivativeT;*/

/*static derivativeT rk4Evaluate(bodyStateT* initial, derivativeT* d, float dt) {
    bodyStateT state;

    state.x.x = initial->x.x + d->dx.x * dt;
    state.x.y = initial->x.y + d->dx.y * dt;
    state.v.x = initial->v.x + d->dv.x * dt;
    state.v.y = initial->v.y + d->dv.y * dt;
    state.o = initial->o + d->d_o  * dt;
    state.w = initial->w + d->dw   * dt;

    derivativeT output;

    vec3 acc = accelerationFunc2(&state);

    output.dx = state.v;
    output.dv = acc.xy;
    output.d_o = state.w;
    output.dw = acc.z;

    return (output);
}*/
/**/
/*
void rk4Integrate(float* x, float* v, float* a, int n, float dt,
                  void(*acc_fn)(float*, float))
{
    // Fourth-order Runge-Kutta integration.

#ifdef _DEBUG
    // We need to do some buffering so make sure we have room for it.
    assert(n < 16);
#endif // _DEBUG

    float zeroes[16] = { 0 };

    //derivativeT a = rk4Evaluate(&body->state, &empty, 0.0f);
    //derivativeT b = rk4Evaluate(&body->state, &a, dt*0.5f);
    //derivativeT c = rk4Evaluate(&body->state, &b, dt*0.5f);
    //derivativeT d = rk4Evaluate(&body->state, &c, dt);

    for (int i = 0; i < n; i++) {
        x[i] += (1.0f/6.0f) * (0 + 2.0f*(0 + 0) + 0) * dt;
        v[i] += (1.0f/6.0f) * (0 + 2.0f*(0 + 0) + 0) * dt;
    }
    /*body->state.x.x += (1.0f / 6.0f) * (a.dx.x + 2.0f*(b.dx.x + c.dx.x) + d.dx.x) * dt;
    body->state.x.y += (1.0f / 6.0f) * (a.dx.y + 2.0f*(b.dx.y + c.dx.y) + d.dx.y) * dt;
    body->state.v.x += (1.0f / 6.0f) * (a.dv.x + 2.0f*(b.dv.x + c.dv.x) + d.dv.x) * dt;
    body->state.v.y += (1.0f / 6.0f) * (a.dv.y + 2.0f*(b.dv.y + c.dv.y) + d.dv.y) * dt;
    body->state.o += (1.0f / 6.0f) * (a.d_o + 2.0f*(b.d_o + c.d_o) + d.d_o) * dt;
    body->state.w += (1.0f / 6.0f) * (a.dw + 2.0f*(b.dw + c.dw) + d.dw) * dt;*/
//}
//*/