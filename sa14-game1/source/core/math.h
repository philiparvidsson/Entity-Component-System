#ifndef math_h_
#define math_h_

typedef struct {
    float x, y;
} vector2T;

typedef struct {
    float x, y, z;
} vector3T;

typedef struct {
    float x, y, z, w;
} vector4T;

typedef struct {
    vector2T x, y;
} matrix2T;

typedef struct {
    vector3T x, y, z;
} matrix3T;

typedef struct {
    vector4T x, y, z, w;
} matrix4T;

#endif /* math_h_ */
