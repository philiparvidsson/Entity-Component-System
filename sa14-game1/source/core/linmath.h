/*------------------------------------------------------------------------------
 * File: linmath.h
 * Created: June 14, 2015
 * Last changed: June 15, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Math related types and functions, specifically for linear algebra.
 *----------------------------------------------------------------------------*/

#ifndef linmath_h_
#define linmath_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"

#include "math/matrix.h"
#include "math/vector.h"

#include <limits.h>
#include <math.h>



/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*------------------------------------------------------------------------------
 * Unit vector macros.
 *----------------------------------------------------------------------------*/

#define vec2_unitX() (vec2) { 1.0f, 0.0f }
#define vec2_unitY() (vec2) { 0.0f, 1.0f }

#define vec3_unitX() (vec3) { 1.0f, 0.0f, 0.0f }
#define vec3_unitY() (vec3) { 0.0f, 1.0f, 0.0f }
#define vec3_unitZ() (vec3) { 0.0f, 0.0f, 1.0f }

#define vec4_unitX() (vec4) { 1.0f, 0.0f, 0.0f, 0.0f }
#define vec4_unitY() (vec4) { 0.0f, 1.0f, 0.0f, 0.0f }
#define vec4_unitZ() (vec4) { 0.0f, 0.0f, 1.0f, 0.0f }
#define vec4_unitW() (vec4) { 0.0f, 0.0f, 0.0f, 1.0f }

/*------------------------------------------------------------------------------
 * Vector-vector operations.
 *----------------------------------------------------------------------------*/

#define vec2_add(a, b) ((vec2) { a.x+b.x, a.y+b.y })
#define vec2_mul(a, b) ((vec2) { a.x*b.x, a.y*b.y })
#define vec2_sub(a, b) ((vec2) { a.x-b.x, a.y-b.y })

#define vec3_add(a, b) ((vec3) { a.x+b.x, a.y+b.y, a.z+b.z })
#define vec3_mul(a, b) ((vec3) { a.x*b.x, a.y*b.y, a.z*b.z })
#define vec3_sub(a, b) ((vec3) { a.x-b.x, a.y-b.y, a.z-b.z })

#define vec4_add(a, b) ((vec4) { a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w })
#define vec4_mul(a, b) ((vec4) { a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w })
#define vec4_sub(a, b) ((vec4) { a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w })

#define vec2_dot(a, b) (a.x*b.x+a.y*b.y)
#define vec3_dot(a, b) (a.x*b.x+a.y*b.y+a.z*b.z)
#define vec4_dot(a, b) (a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w)


/*------------------------------------------------------------------------------
 * Vector-matrix operations.
 *----------------------------------------------------------------------------*/

#define vec2_mat2x2_mul(v, m) ((vec2) { \
            v.x*m.x.x+v.y*m.x.y,        \
            v.x*m.y.x+v.y*m.y.y         \
        })

#define vec3_mat3x3_mul(v, m) ((vec3) {    \
            v.x*m.x.x+v.y*m.x.y+v.z*m.x.z, \
            v.x*m.y.x+v.y*m.y.y+v.z*m.y.z, \
            v.x*m.z.x+v.y*m.z.y+v.z*m.z.z  \
        })

#define vec4_mat4x4_mul(v, m) ((vec4) {             \
            v.x*m.x.x+v.y*m.x.y+vz*m.x.z+v.w*m.x.w, \
            v.x*m.y.x+v.y*m.y.y+vz*m.y.z+v.w*m.y.w, \
            v.x*m.z.x+v.y*m.z.y+vz*m.z.z+v.w*m.z.w, \
            v.x*m.w.x+v.y*m.w.y+vz*m.w.z+v.w*m.w.w  \
        })

/*------------------------------------------------------------------------------
 * Matrix macros.
 *----------------------------------------------------------------------------*/

#define mat2x2_identity() ((mat2x2) { 1.0f, 0.0f, \
                                      0.0f, 1.0f })

#define mat3x3_identity() ((mat3x3) { 1.0f, 0.0f, 0.0f,   \
                                      0.0f, 1.0f, 0.0f,   \
                                      0.0f, 0.0f, 1.0f })

#define mat4x4_identity() ((mat4x4) { 1.0f, 0.0f, 0.0f, 0.0f,   \
                                      0.0f, 1.0f, 0.0f, 0.0f,   \
                                      0.0f, 0.0f, 1.0f, 0.0f,   \
                                      0.0f, 0.0f, 0.0f, 1.0f })

#define mat2x2_rotateZ(a) ((mat2x2) { cos(a), -sin(a),   \
                                      sin(a),  cos(a) })

#define mat3x3_rotateX(a) ((mat3x3) { 1.0f, 0.0f,    0.0f,     \
                                      0.0f, cos(a), -sin(a),   \
                                      0.0f, sin(a),  cos(a) })

#define mat3x3_rotateY(a) ((mat3x3) {  cos(a), 0.0f, sin(a),   \
                                       0.0f,   1.0f, 0.0f,     \
                                      -sin(a), 0.0f, cos(a) })

#define mat3x3_rotateZ(a) ((mat3x3) { cos(a), -sin(a), 0.0f,   \
                                      sin(a),  cos(a), 0.0f,   \
                                      0.0f,    0.0f,   1.0f })

#define mat4x4_rotateX(a) ((mat4x4) { 1.0f, 0.0f,    0.0f,   0.0f,   \
                                      0.0f, cos(a), -sin(a), 0.0f,   \
                                      0.0f, sin(a),  cos(a), 0.0f,   \
                                      0.0f, 0.0f,    0.0f,   1.0f })

#define mat4x4_rotateY(a) ((mat4x4) {  cos(a), 0.0f, sin(a), 0.0f,   \
                                       0.0f,   1.0f, 0.0f,   0.0f,   \
                                      -sin(a), 0.0f, cos(a), 0.0f,   \
                                       0.0f,   0.0f, 0.0f,   1.0f })

#define mat4x4_rotateZ(a) ((mat4x4) { cos(a), -sin(a), 0.0f, 0.0f,   \
                                      sin(a),  cos(a), 0.0f, 0.0f,   \
                                      0.0f,    0.0f,   1.0f, 0.0f,   \
                                      0.0f,    0.0f,   0.0f, 1.0f })

#define mat4x4_translateX(d) ((mat4x4) { 1.0f, 0.0f, 0.0f, d,      \
                                         0.0f, 1.0f, 0.0f, 0.0f,   \
                                         0.0f, 0.0f, 1.0f, 0.0f,   \
                                         0.0f, 0.0f, 0.0f, 1.0f })

#define mat4x4_translateY(d) ((mat4x4) { 1.0f, 0.0f, 0.0f, 0.0f,   \
                                         0.0f, 1.0f, 0.0f, d,      \
                                         0.0f, 0.0f, 1.0f, 0.0f,   \
                                         0.0f, 0.0f, 0.0f, 1.0f })

#define mat4x4_translateZ(d) ((mat4x4) { 1.0f, 0.0f, 0.0f, 0.0f,   \
                                         0.0f, 1.0f, 0.0f, 0.0f,   \
                                         0.0f, 0.0f, 1.0f, d,      \
                                         0.0f, 0.0f, 0.0f, 1.0f })

#define mat2x2_toMat3x3(m) ((mat3x3) { m.x.x, m.x.y, 0.0f,   \
                                       m.y.x, m.y.y, 0.0f,   \
                                       0.0f,  0.0f,  0.0f })

#define mat2x2_toMat4x4(m) ((mat4x4) { m.x.x, m.x.y, 0.0f, 0.0f,   \
                                       m.y.x, m.y.y, 0.0f, 0.0f,   \
                                       0.0f,  0.0f,  0.0f, 0.0f,   \
                                       0.0f,  0.0f,  0.0f, 0.0f })

#define mat3x3_toMat2x2(m) ((mat2x2) { m.x.x, m.x.y,   \
                                       m.y.x, m.y.y })

#define mat3x3_toMat4x4(m) ((mat4x4) { m.x.x, m.x.y, m.x.z, 0.0f,   \
                                       m.y.x, m.y.y, m.y.z, 0.0f,   \
                                       m.z.x, m.z.y, m.z.z, 0.0f,   \
                                       0.0f,  0.0f,  0.0f,  0.0f })

#define mat4x4_toMat2x2(m) ((mat2x2) { m.x.x, m.x.y,   \
                                       m.y.x, m.y.y })

#define mat4x4_toMat3x3(m) ((mat3x3) { m.x.x, m.x.y, m.x.z,   \
                                       m.y.x, m.y.y, m.y.z,   \
                                       m.z.x, m.z.y, m.z.z })

#define mat2x2_transpose(m) ((mat2x2) { m.x.x, m.y.x,   \
                                        m.x.y, m.y.y })

#define mat3x3_transpose(m) ((mat3x3) { m.x.x, m.y.x, m.z.x,   \
                                        m.x.y, m.y.y, m.z.y,   \
                                        m.x.z, m.y.z, m.z.z })

#define mat4x4_transpose(m) ((mat4x4) { m.x.x, m.y.x, m.z.x, m.w.x,   \
                                        m.x.y, m.y.y, m.z.y, m.w.y,   \
                                        m.x.z, m.y.z, m.z.z, m.w.z,   \
                                        m.x.w, m.y.w, m.z.w, m.w.w })

/*------------------------------------------------------------------------------
 * Matrix-matrix operations.
 *----------------------------------------------------------------------------*/

static inline mat2x2 mat2x2_add(mat2x2 a, mat2x2 b) {
    return ((mat2x2) {
        a.x.x + b.x.x, a.x.y + b.x.y,
        a.y.x + b.y.x, a.y.y + b.y.y
    });
}

static inline void mat2x2_mul(const mat2x2 *a, const mat2x2 *b, mat2x2 *r) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            r->m[i][j] = a->m[i][0] * b->m[0][j]
                       + a->m[i][1] * b->m[1][j];
        }
    }
}

static inline mat2x2 mat2x2_sub(mat2x2 a, mat2x2 b) {
    return ((mat2x2) {
        a.x.x - b.x.x, a.x.y - b.x.y,
        a.y.x - b.y.x, a.y.y - b.y.y
    });
}

static inline mat3x3 mat3x3_add(mat3x3 a, mat3x3 b) {
    return ((mat3x3) {
        a.x.x + b.x.x, a.x.y + b.x.y, a.x.z + b.x.z,
        a.y.x + b.y.x, a.y.y + b.y.y, a.y.z + b.y.z,
        a.z.x + b.z.x, a.z.y + b.z.y, a.z.z + b.z.z
    });
}

static inline void mat3x3_mul(const mat3x3 *a, const mat3x3 *b, mat3x3 *r) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            r->m[i][j] = a->m[i][0] * b->m[0][j]
                       + a->m[i][1] * b->m[1][j]
                       + a->m[i][2] * b->m[2][j];
        }
    }
}

static inline mat3x3 mat3x3_sub(mat3x3 a, mat3x3 b) {
    return ((mat3x3) {
        a.x.x - b.x.x, a.x.y - b.x.y, a.x.z - b.x.z,
        a.y.x - b.y.x, a.y.y - b.y.y, a.y.z - b.y.z,
        a.z.x - b.z.x, a.z.y - b.z.y, a.z.z - b.z.z
    });
}

static inline mat4x4 mat4x4_add(const mat4x4 *a, const mat4x4 *b, mat4x4 *r) {
    for (int i = 0; i < 4*4; i++)
        r->value[i] = a->value[i] + b->value[i];
}

static inline void mat4x4_mul(const mat4x4 *a, const mat4x4 *b, mat4x4 *r) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            r->m[i][j] = a->m[i][0] * b->m[0][j]
                       + a->m[i][1] * b->m[1][j]
                       + a->m[i][2] * b->m[2][j]
                       + a->m[i][3] * b->m[3][j];
        }
    }
}

static inline mat4x4 mat4x4_sub(const mat4x4 *a, const mat4x4 *b, mat4x4 *r) {
    for (int i = 0; i < 4*4; i++)
        r->value[i] = a->value[i] - b->value[i];
}

static inline mat4x4 mat4x4_lookAt(vec3 pos, vec3 at, vec3 up) {
    vec3 x_axis, y_axis, z_axis;

    vec_sub      (&pos,    &at,     &z_axis);
    vec_normalize(&z_axis, &z_axis         );
    vec3_cross   (&up,     &z_axis, &x_axis);
    vec_normalize(&x_axis, &x_axis          );
    vec3_cross   (&z_axis, &x_axis, &y_axis);

    float x_dot = vec_dot(&x_axis, &pos);
    float y_dot = vec_dot(&y_axis, &pos);
    float z_dot = vec_dot(&z_axis, &pos);

    mat4x4 m = {
         x_axis.x,  x_axis.y,  x_axis.z, -x_dot,
         y_axis.x,  y_axis.y,  y_axis.z, -y_dot,
         z_axis.x,  z_axis.y,  z_axis.z, -z_dot,
         0.0f,     0.0f,      0.0f,    1.0f
    };

    return (m);
}

static inline mat4x4 mat4x4_ortho(float left, float right, float bottom,
                                  float top, float near, float far)
{
    float l=left, r=right, b=bottom, t=top, n=near, f=far;

    mat4x4 m = {
        2.0f/(r-l), 0.0f,        0.0f,       -(r+l)/(r-l),
        0.0f,       2.0f/(t-b),  0.0f,       -(t+b)/(t-b),
        0.0f,       0.0f,        2.0f/(n-f), -(n+f)/(n-f),
        0.0f,       0.0f,        0.0f,        1.0f
    };
    
    return (m);
}

static inline mat4x4 mat4x4_perspective(float left, float right, float bottom,
                                        float top, float near, float far)
{
    float l=left, r=right, b=bottom, t=top, n=near, f=far;

    mat4x4 m = {
        (-2.0f*f)/(r-l),  0.0f,            (r+l)/(r-l), 0.0f,
         0.0f,           -(2.0f*f)/(t-b),  (t+b)/(t-b), 0.0f,
         0.0f,            0.0f,           -(n+f)/(n-f), (2.0f*n*f)/(n-f),
         0.0f,            0.0f,           -1.0f,        0.0f
    };

    return (m);
}

//------------------------------------------------------------------------------




#endif // linalg_h_
