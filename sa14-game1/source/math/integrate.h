#ifndef integrate_h_
#define integrate_h_

#include <string.h>

typedef void (*derivativeFnT)(const float*, float *);

static inline void eulerIntegrate(float* state, float* derivs, int n, float dt,
                                  derivativeFnT deriv_fn)
{
    // This is actually symplectic Euler integration since we go backwards
    // through the state variables.
    //     See https://en.wikipedia.org/wiki/Semi-implicit_Euler_method for more
    // information.

    deriv_fn(state, derivs);

    for (int i = (n-1); i >= 0; i--)
        state[i] += derivs[i] * dt;

}

static inline void rk4Integrate(float* state, float* derivs, int n, float dt,
                                derivativeFnT deriv_fn)
{
    // Fourth-order Runge-Kutta integration.
    //     See http://mathworld.wolfram.com/Runge-KuttaMethod.html for more
    // information.

#ifdef _DEBUG
    // We need to do some buffering so make sure we have room for it.
    assert(n <= 8);
#endif // _DEBUG

    float tmp[8], k1[8], k2[8], k3[8], k4[8];

    deriv_fn(state, k1);
    for (int i = 0; i < n; i++) tmp[i] = state[i]+k1[i]*dt*0.5f;
    deriv_fn(tmp, k2);
    for (int i = 0; i < n; i++) tmp[i] = state[i]+k2[i]*dt*0.5f;
    deriv_fn(tmp, k3);
    for (int i = 0; i < n; i++) tmp[i] = state[i]+k3[i]*dt;
    deriv_fn(tmp, k4);

    for (int i = 0; i < n; i++)
        state[i] += (1.0f/6.0f)*(k1[i]+2.0f*(k2[i]+k3[i])+k4[i])*dt;
}

#endif // integrate_h_
