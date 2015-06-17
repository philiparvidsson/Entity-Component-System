#ifndef matrix_h_
#define matrix_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"

#include "math/vector.h"

#include <math.h>

/*------------------------------------------------
 * MACROS
 *----------------------------------------------*/

#define matCheckArgs1() { \
    if (n == (-1)) error("First argument is not a matrix"); \
}

#define matCheckArgs2() { \
    if (a_n == (-1)) error("First argument is not a matrix");  \
    if (r_n == (-1)) error("Second argument is not a matrix"); \
}

#define matCheckArgs3() { \
    if (a_n == (-1)) error("First argument is not a matrix");  \
    if (b_n == (-1)) error("Second argument is not a matrix"); \
    if (r_n == (-1)) error("Third argument is not a matrix");  \
}


#define mat_n(v) (((vec_n(v))==16) ? ( 4) \
               : (((vec_n(v))== 9) ? ( 3) \
               : (((vec_n(v))== 4) ? ( 2) \
               :                     (-1))))

#define mat_identity(r) mat_identity_((float *)r, mat_n(*r))

#define mat_transl_xy(x, y, r) mat_transl_xy_(x, y, (float *)r, mat_n(*r))
#define mat_transl_xyz(x, y, z, r) mat_transl_xyz_(x, y, z, (float *)r, mat_n(*r))

#define mat_rot_x(a, r) mat_rot_x_(a, (float *)r, mat_n(*r))
#define mat_rot_y(a, r) mat_rot_y_(a, (float *)r, mat_n(*r))
#define mat_rot_z(a, r) mat_rot_z_(a, (float *)r, mat_n(*r))

#define mat_add(a, b, r) \
    mat_add_((float *)a, (float *)b, (float *)r, mat_n(*a), mat_n(*b), mat_n(*r))

#define mat_sub(a, b, r) \
    mat_sub_((float *)a, (float *)b, (float *)r, mat_n(*a), mat_n(*b), mat_n(*r))

#define mat_mul(a, b, r) \
    mat_mul_((float *)a, (float *)b, (float *)r, mat_n(*a), mat_n(*b), mat_n(*r))

#define mat_div(a, b, r) \
    mat_div_((float *)a, (float *)b, (float *)r, mat_n(*a), mat_n(*b), mat_n(*r))

#define mat_transpose(a, r) \
    mat_transpose_((float *)a, (float *)r, mat_n(*a), mat_n(*r))


/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

typedef union {
    float value[4];
    float m[2][2];
    vec2 row[2];
    struct { vec2 x; vec2 y; };
} mat2x2;

typedef union {
    float value[9];
    float m[3][3];
    vec3 row[3];
    struct { vec3 x; vec3 y; vec3 z; };
} mat3x3;

typedef union {
    float value[16];
    float m[4][4];
    vec4 row[4];
    struct { vec4 x; vec4 y; vec4 z; vec4 w; };
} mat4x4;


/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

static mat4x4 const mat_id2entity = { 1.0f, 0.0f, 0.0f, 0.0f,
                                     0.0f, 1.0f, 0.0f, 0.0f,
                                     0.0f, 0.0f, 1.0f, 0.0f,
                                     0.0f, 0.0f, 0.0f, 1.0f };

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static inline void mat_identity_(float *r, int n) {
    matCheckArgs1();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            r[i*n+j] = (i==j) ? (1.0f) : (0.0f);
    }
}

static inline void mat_transl_xy_(float x, float y, float *m, int n) {
    matCheckArgs1();

    if (n < 3) error("Matrix is too small");

    mat_identity_(m, n);

    m[  n-1] = x;
    m[2*n-1] = y;
}

static inline void mat_transl_xyz_(float x, float y, float z, float *m, int n) {
    matCheckArgs1();

    if (n < 4) error("Matrix is too small");

    mat_identity_(m, n);

    m[  n-1] = x;
    m[2*n-1] = y;
    m[3*n-1] = z;
}

static inline void mat_rot_x_(float a, float *m, int n) {
    matCheckArgs1();

    if (n < 3) error("Matrix is too small");

    mat_identity_(m, n);

    m[n  +1] =  cosf(a);
    m[n  +2] = -sinf(a);
    m[2*n+1] =  sinf(a);
    m[2*n+2] =  cosf(a);
}

static inline void mat_rot_y_(float a, float *m, int n) {
    matCheckArgs1();

    if (n < 3) error("Matrix is too small");

    mat_identity_(m, n);

    m[    0] =  cosf(a);
    m[    2] =  sinf(a);
    m[2*n  ] = -sinf(a);
    m[2*n+2] =  cosf(a);
}

static inline void mat_rot_z_(float a, float *m, int n) {
    matCheckArgs1();

    mat_identity_(m, n);

    m[  0] =  cosf(a);
    m[  1] = -sinf(a);
    m[n  ] =  sinf(a);
    m[n+1] =  cosf(a);
}

static inline void mat_add_(float const *a, float const *b, float *r, int a_n, int b_n, int r_n) {
    matCheckArgs3();

    for (int i = 0; i < min(a_n, min(b_n, r_n)); i++) {
        for (int j = 0; j < min(a_n, min(b_n, r_n)); j++)
            r[i*r_n+j] = a[i*a_n+j] + b[i*b_n+j];
    }
}

static inline void mat_sub_(float const *a, float const *b, float *r, int a_n, int b_n, int r_n) {
    matCheckArgs3();

    for (int i = 0; i < min(a_n, min(b_n, r_n)); i++) {
        for (int j = 0; j < min(a_n, min(b_n, r_n)); j++)
            r[i*r_n+j] = a[i*a_n+j] - b[i*b_n+j];
    }
}

static inline void mat_mul_(float const *a, float const *b, float *r, int a_n, int b_n, int r_n) {
    matCheckArgs3();

    int const  n = min(a_n, min(b_n, r_n));
    mat4x4     t;
    float     *m = (r==a || r==b) ? (float *)&t : r;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            float d = 0.0f;

            for (int k = 0; k < n; k++)
                d += a[i*a_n+k] * b[j+k*b_n];

            m[i*r_n+j] = d;
        }
    }

    if (m == &t) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                r[i*r_n+j] = m[i*r_n+j];
        }
    }
}

static inline void mat_div_(float const *a, float const *b, float *r, int a_n, int b_n, int r_n) {
    matCheckArgs3();

    for (int i = 0; i < min(a_n, min(b_n, r_n)); i++) {
        for (int j = 0; j < min(a_n, min(b_n, r_n)); j++)
            r[i*r_n+j] = a[i*a_n+j] / b[i*b_n+j];
    }
}

static inline void mat_transpose_(float const *a, float *r, int a_n, int r_n) {
    matCheckArgs2();

    for (int i = 0; i < min(a_n, r_n); i++) {
        for (int j = 0; j < min(a_n, r_n); j++)
            r[i*r_n+j] = a[j*a_n+i];
    }
}

#endif // matrix_h_
