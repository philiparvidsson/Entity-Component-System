#ifndef integrate_h_
#define integrate_h_

void integrateEuler(float* x, float* v, float* a, int n, float dt,
                    void (*acc_fn)(float*, float));

void integrateRK4(float* x, float* v, float* a, int n, float dt,
                  void(*acc_fn)(float*, float));

#endif // integrate_h_
