/*------------------------------------------------------------------------------
 * File: vector.h
 * Created: June 17, 2015
 * Last changed: June 18, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Vector types, macros, fucntions etc. Define VECTOR_RGB before including
 *   this file to enable the r, g, b and a coordinate aliases.
 *----------------------------------------------------------------------------*/

#ifndef vector_h_
#define vector_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/common.h"

#include <limits.h> // CHAR_BIT
#include <math.h>

/*------------------------------------------------
 * MACROS
 *----------------------------------------------*/

/*--------------------------------------
 * Macro: vec_n(v)
 * Parameters:
 *   v  The vector to figure out the length of.
 *
 * Returns:
 *   The length of the vector, in number of coordinates.
 *
 * Description:
 *   Figures out the length of the specified vector, in number of coordinates.
 *
 * Usage:
 *   int num_coords = vec_n(v);
 *------------------------------------*/
#define vec_n(v) (sizeof(v)/sizeof(float))

/*--------------------------------------
 * Macro: vec_add(a, b, r)
 * Parameters:
 *   a  Pointer to first vector.
 *   b  Pointer to second vector.
 *   r  Pointer to result vector.
 *
 * Description:
 *   Adds each coordinate in b to the respective coordinate in a and stores the
 *   result in r: r_i=a_i+b_i.
 *
 * Usage:
 *   vec_add(&a, &b, &r);
 *------------------------------------*/
#define vec_add(a, b, r) vec_add_((float*)a, (float*)b, (float*)r, \
                                  min(min(vec_n(*a), vec_n(*b)), vec_n(*r)))

/*--------------------------------------
 * Macro: vec_sub(a, b, r)
 * Parameters:
 *   a  Pointer to first vector.
 *   b  Pointer to second vector.
 *   r  Pointer to result vector.
 *
 * Description:
 *   Subtracts each coordinate in b from the respective coordinate in a and
 *   stores the result in r: r_i=a_i-b_i.
 *
 * Usage:
 *   vec_sub(&a, &b, &r);
 *------------------------------------*/
#define vec_sub(a, b, r) vec_sub_((float*)a, (float*)b, (float*)r, \
                                  min(min(vec_n(*a), vec_n(*b)), vec_n(*r)))

/*--------------------------------------
 * Macro: vec_mul(a, b, r)
 * Parameters:
 *   a  Pointer to first vector.
 *   b  Pointer to second vector.
 *   r  Pointer to result vector.
 *
 * Description:
 *   Multiplies each coordinate in b with the respective coordinate in a and
 *   stores the result in r: r_i=a_i*b_i.
 *
 * Usage:
 *   vec_mul(&a, &b, &r);
 *------------------------------------*/
#define vec_mul(a, b, r) vec_mul_((float*)a, (float*)b, (float*)r, \
                                  min(min(vec_n(*a), vec_n(*b)), vec_n(*r)))

/*--------------------------------------
 * Macro: vec_div(a, b, r)
 * Parameters:
 *   a  Pointer to first vector.
 *   b  Pointer to second vector.
 *   r  Pointer to result vector.
 *
 * Description:
 *   Divides each coordinate in b with the respective coordinate in a and stores
 *   the result in r: r_i=a_i/b_i.
 *
 * Usage:
 *   vec_div(&a, &b, &r);
 *------------------------------------*/
#define vec_div(a, b, r) vec_div_((float*)a, (float*)b, (float*)r, \
                                  min(min(vec_n(*a), vec_n(*b)), vec_n(*r)))

/*--------------------------------------
 * Macro: vec_scale(v, f, r)
 * Parameters:
 *   v  Pointer to first vector.
 *   f  Scaling factor.
 *   r  Pointer to result vector.
 *
 * Description:
 *   Multiplies each coordinate in the specified vector with the scaling factor
 *   and store the result in r: r_i=v_i*f
 *
 * Usage:
 *   vec_scale_(&a, 2.0f, &r);
 *------------------------------------*/
#define vec_scale(v, f, r) vec_scale_((float*)v, f, (float*)r, \
                                      min(vec_n(*v), vec_n(*r)))

/*--------------------------------------
 * Macro: vec_dot(a, b)
 * Parameters:
 *   a  Pointer to first vector.
 *   b  Pointer to second vector.
 *
 * Returns:
 *   The dot product of the two specified vectors.
 *
 * Description:
 *   Calculates the dot product of the two specified vectors and returns it:
 *   d = a_0*b_0+...+a_n*b_n.
 *
 * Usage:
 *   float d = vec_dot(&a, &b);
 *------------------------------------*/
#define vec_dot(a, b) vec_dot_((float*)a, (float*)b, \
                               min(vec_n(*a), vec_n(*b)))

/*--------------------------------------
 * Macro: vec_normalize(a, r)
 * Parameters:
 *   a  Pointer to the vector to normalize.
 *   r  Pointer to result vector.
 *
 * Description:
 *   Normalizes the vector v and stores the result in r: r_i=a_i/sqrt(dot(a,a)).
 *
 * Usage:
 *   vec_normalize(&a, &r);
 *------------------------------------*/
#define vec_normalize(v, r) \
    vec_normalize_((float*)v, (float*)r, min(vec_n(*v), vec_n(*r)))

/*--------------------------------------
 * Macro: vec_float_padding
 *
 * Description:
 *   Skips a single vector coordinate in a struct or union..
 *
 * Usage:
 *   vec_float_padding;
 *------------------------------------*/
#define vec_float_padding int:sizeof(float) * CHAR_BIT

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: vec2
 *
 * Description:
 *   Vector type with two components (x and y).
 *------------------------------------*/
typedef union {
    float coord[2];
    struct { float x, y; };
} vec2;

/*--------------------------------------
 * Type: vec3
 *
 * Description:
 *   Vector type with three components (x, y and z or r, g and b if the
 *   VECTOR_RGB symbol is defined).
 *------------------------------------*/
typedef union {
    float coord[3];
    struct { float x, y, z; };

    vec2 xy;
    struct { vec_float_padding; vec2 yz; };

#ifdef VECTOR_RGB
    struct { float r, g, b; };

    vec2 rg;
    struct { vec_float_padding; vec2 gb; };
#endif // VECTOR_RGB
} vec3;

/*--------------------------------------
 * Type: vec4
 *
 * Description:
 *   Vector type with three components (x, y, z and w or r, g, b and a if the
 *   VECTOR_RGB symbol is defined).
 *------------------------------------*/
typedef union {
    float coord[4];
    struct { float x, y, z, w; };

    vec2 xy;
    struct { vec_float_padding; vec2 yz; };
    struct { vec_float_padding; vec_float_padding; vec2 zw; };

    vec3 xyz;
    struct { vec_float_padding; vec3 yzw; };

#ifdef VECTOR_RGB
    struct { float r, g, b, a; };

    vec2 rg;
    struct { vec_float_padding; vec2 gb; };
    struct { vec_float_padding; vec_float_padding; vec2 ba; };

    vec3 rgb;
    struct { vec_float_padding; vec3 gba; };
#endif // VECTOR_RGB
} vec4;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: vec_add_(a, b, r, n)
 * Parameters:
 *   a  Pointer to first vector.
 *   b  Pointer to second vector.
 *   r  Pointer to result vector.
 *   n  The minimum number of coordinates in any of the specified vectors.
 *
 * Description:
 *   Adds each coordinate in b to the respective coordinate in a and stores the
 *   result in r: r_i=a_i+b_i.
 *
 * Usage:
 *   vec_add_(&a, &b, &r, 3);
 *------------------------------------*/
static inline void vec_add_(const float* a, const float* b, float *r, int n) {
    for (int i = 0; i < n; i++)
        r[i] = a[i] + b[i];
}

/*--------------------------------------
 * Function: vec_sub_(a, b, r, n)
 * Parameters:
 *   a  Pointer to first vector.
 *   b  Pointer to second vector.
 *   r  Pointer to result vector.
 *   n  The minimum number of coordinates in any of the specified vectors.
 *
 * Description:
 *   Subtracts each coordinate in b from the respective coordinate in a and
 *   stores the result in r: r_i=a_i-b_i.
 *
 * Usage:
 *   vec_sub_(&a, &b, &r, 3);
 *------------------------------------*/
static inline void vec_sub_(const float* a, const float* b, float *r, int n) {
    for (int i = 0; i < n; i++)
        r[i] = a[i] - b[i];
}

/*--------------------------------------
 * Function: vec_mul_(a, b, r, n)
 * Parameters:
 *   a  Pointer to first vector.
 *   b  Pointer to second vector.
 *   r  Pointer to result vector.
 *   n  The minimum number of coordinates in any of the specified vectors.
 *
 * Description:
 *   Multiplies each coordinate in b with the respective coordinate in a and
 *   stores the result in r: r_i=a_i*b_i.
 *
 * Usage:
 *   vec_mul_(&a, &b, &r, 3);
 *------------------------------------*/
static inline void vec_mul_(const float* a, const float* b, float *r, int n) {
    for (int i = 0; i < n; i++)
        r[i] = a[i] * b[i];
}

/*--------------------------------------
 * Function: vec_div_(a, b, r, n)
 * Parameters:
 *   a  Pointer to first vector.
 *   b  Pointer to second vector.
 *   r  Pointer to result vector.
 *   n  The minimum number of coordinates in any of the specified vectors.
 *
 * Description:
 *   Divides each coordinate in b with the respective coordinate in a and stores
 *   the result in r: r_i=a_i/b_i.
 *
 * Usage:
 *   vec_div_(&a, &b, &r, 3);
 *------------------------------------*/
static inline void vec_div_(const float* a, const float* b, float *r, int n) {
    for (int i = 0; i < n; i++)
        r[i] = a[i] / b[i];
}

/*--------------------------------------
 * Function: vec_scale_(v, f, r, n)
 * Parameters:
 *   v  Pointer to first vector.
 *   f  Scaling factor.
 *   r  Pointer to result vector.
 *   n  The minimum number of coordinates in any of the specified vectors.
 *
 * Description:
 *   Multiplies each coordinate in the specified vector with the scaling factor
 *   and store the result in r: r_i=v_i*f
 *
 * Usage:
 *   vec_scale_(&a, 2.0f, &r, 3);
 *------------------------------------*/
static inline void vec_scale_(const float* v, float f, float *r, int n) {
    for (int i = 0; i < n; i++)
        r[i] = v[i] * f;
}

/*--------------------------------------
 * Function: vec3_cross(a, b, r)
 * Parameters:
 *   a  Pointer to first vector.
 *   b  Pointer to second vector.
 *   r  Pointer to result vector.
 *
 * Description:
 *   Calculates the cross product of the two specifiec vectors and stores the
 *   result in r.
 *
 * Usage:
 *   vec3_cross(&a, &b, &r);
 *------------------------------------*/
static inline void vec3_cross(const vec3* a, const vec3* b, vec3 *r) {
    r->x = a->y*b->z - a->z*b->y;
    r->y = a->z*b->x - a->x*b->z;
    r->z = a->x*b->y - a->y*b->x;
}

/*--------------------------------------
 * Function: vec_dot_(a, b, n)
 * Parameters:
 *   a  Pointer to first vector.
 *   b  Pointer to second vector.
 *   n  The minimum number of coordinates in any of the specified vectors.
 *
 * Returns:
 *   The dot product of the two specified vectors.
 *
 * Description:
 *   Calculates the dot product of the two specified vectors and returns it:
 *   d = a_0*b_0+...+a_n*b_n.
 *
 * Usage:
 *   float d = vec_dot_(&a, &b, 3);
 *------------------------------------*/
static inline float vec_dot_(const float* a, const float* b, int n) {
    float d = 0.0f;

    for (int i = 0; i < n; i++)
        d += a[i]*b[i];

    return d;
}

/*--------------------------------------
 * Function: vec_normalize(a, r, n)
 * Parameters:
 *   a  Pointer to the vector to normalize.
 *   r  Pointer to result vector.
 *   n  The minimum number of coordinates in any of the specified vectors.
 *
 * Description:
 *   Normalizes the vector v and stores the result in r: r_i=a_i/sqrt(dot(a,a)).
 *
 * Usage:
 *   vec_normalize(&a, &r, 3);
 *------------------------------------*/
static inline void vec_normalize_(const float* v, float *r, int n) {
    float d = vec_dot_(v, v, n);

    if (d > 0.0f) d = sqrtf(d);
    else          d = 1.0f;

    for (int i = 0; i < n; i++)
        r[i] = v[i] / d;
}

#endif // vector_h_
