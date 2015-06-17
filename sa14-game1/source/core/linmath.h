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
 * Vector-vector operations.
 *----------------------------------------------------------------------------*/



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
 * Matrix-matrix operations.
 *----------------------------------------------------------------------------*/


static inline mat4x4 mat4x4_lookAt(vec3 pos, vec3 at, vec3 up) {
    vec3 z_axis;
    vec_sub(&pos, &at, &z_axis);
    vec_normalize(&z_axis, &z_axis);

    vec3 x_axis;
    vec3_cross(&up, &z_axis, &x_axis);
    vec_normalize(&x_axis, &x_axis);

    vec3 y_axis;
    vec3_cross(&z_axis, &x_axis, &y_axis);

    float x_dot = vec_dot(&x_axis, &pos);
    float y_dot = vec_dot(&y_axis, &pos);
    float z_dot = vec_dot(&z_axis, &pos);

    mat4x4 m = {
         x_axis.x,  y_axis.x,  z_axis.x, 0.0f,
         x_axis.y,  y_axis.y,  z_axis.y, 0.0f,
         x_axis.z,  y_axis.z,  z_axis.z, 0.0f,
        -x_dot,    -y_dot,    -z_dot,    1.0f
    };

    mat_transpose(&m, &m);
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
