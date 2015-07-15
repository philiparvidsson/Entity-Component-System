#ifndef trimesh_h_
#define trimesh_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/common.h"
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

triMeshT* newMesh(int num_verts, int num_tris);
void freeMesh(triMeshT* mesh);
void updateMesh(const triMeshT* mesh);
void drawMesh(const triMeshT* mesh);

int meshNumTris(const triMeshT* mesh);
int meshNumVerts(const triMeshT* mesh);

triT* meshTrisPtr(triMeshT* mesh);
vertexT* meshVertsPtr(triMeshT* mesh);

triMeshT* createBox(float width, float height, float length);
triMeshT* createCone(float radius, float height, int num_sides);
triMeshT* createQuad(float width, float height);

#endif // trimesh_h_
