#ifndef trimesh_h_
#define trimesh_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"

#include "math/vector.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: vertexT
 *
 * Description:
 *   Represents a single vertex in a triangle mesh.
 *------------------------------------*/
typedef struct {
    union {
        float x, y, z;
        vec3 p;
    };

    union {
        float nx, ny, nz;
        vec3 n;
    };

    union {
        float u, v;
        vec2 uv;
    };
} vertexT;

/*--------------------------------------
 * Type: triT
 *
 * Description:
 *   Represents a single triangle (face) with vertex indices in a triangle mesh.
 *------------------------------------*/
typedef struct {
    int v0, v1, v2;
} triT;

typedef struct triMeshT triMeshT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

triMeshT *createMesh(int num_verts, int num_tris);
void deleteMesh(triMeshT *mesh);
void updateMesh(triMeshT const *mesh);
void drawMesh(triMeshT const *mesh);

int meshNumTris(triMeshT const *mesh);
int meshNumVerts(triMeshT const *mesh);

triT *meshGetTris(triMeshT *mesh);
vertexT *meshGetVerts(triMeshT *mesh);

triMeshT *createBox(float width, float height, float length);
triMeshT *createCone(float radius, float height, int num_sides);
triMeshT *createQuad(float width, float height);

#endif // trimesh_h_
