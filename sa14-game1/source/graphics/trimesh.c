/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "trimesh.h"

#include "base/common.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h> // memset()

#include <GL/glew.h>

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: triMeshT
 *
 * Description:
 *   Represents a piece of geometry with a vertex mesh etc.
 *------------------------------------*/
struct triMeshT {
    int      num_verts; // Number of vertices.
    vertexT* verts;     // The vertices.

    int   num_tris;   // Number of triangles.
    triT* tris;       // The triangles (faces).

    GLuint vbo, // Vertex buffer object.
           ibo; // Index buffer object.
};

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static vec3 calcTriNormal(vertexT* verts, triT* tri) {
    vertexT* v0 = &verts[tri->v0];
    vertexT* v1 = &verts[tri->v1];
    vertexT* v2 = &verts[tri->v2];

    vec3 edge0, edge1, normal;
    vec_sub(&v1->p, &v0->p, &edge0);
    vec_sub(&v2->p, &v0->p, &edge1);
    vec3_cross(&edge0, &edge1, &normal);

    return (normal);
}

/*--------------------------------------
 * Function: newMesh()
 * Parameters:
 *   num_verts  Number of vertices.
 *   num_tris   Number of triangles (faces).
 *
 * Returns:
 *   A pointer to the mesh.
 *
 * Description:
 *   Creates a new mesh with the specified number of vertices and triangles. The
 *   returned pointer is actually a pointer to the internal type triMeshT_.
 *
 * Usage:
 *   triMeshT* mesh = newMesh(8, 6);
 *------------------------------------*/
triMeshT* newMesh(int num_verts, int num_tris) {
    triMeshT* mesh = malloc(sizeof(triMeshT));

    mesh->num_verts  = num_verts;
    mesh->verts      = calloc(mesh->num_verts, sizeof(vertexT));
    mesh->num_tris   = num_tris;
    mesh->tris       = calloc(mesh->num_tris, sizeof(triT));

    size_t vb_size = sizeof(vertexT) * mesh->num_verts;
    size_t ib_size = sizeof(triT)    * mesh->num_tris;

    glGenBuffers(1, &mesh->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, vb_size, mesh->verts, GL_STATIC_DRAW);

    glGenBuffers(1, &mesh->ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ib_size, mesh->tris, GL_STATIC_DRAW);

    return (mesh);
}

/*--------------------------------------
 * Function: freeMesh()
 * Parameters:
 *   mesh  The mesh to free.
 *
 * Description:
 *   Deletes the specified mesh from RAM and VRAM.
 *
 * Usage:
 *   freeMesh(my_mesh);
 *------------------------------------*/
void freeMesh(triMeshT* mesh) {
    glDeleteBuffers(1, &mesh->vbo);
    glDeleteBuffers(1, &mesh->ibo);

    free(mesh->verts);
    free(mesh->tris);
    free(mesh);
}

/*--------------------------------------
 * Function: updateMesh()
 * Parameters:
 *   mesh  The mesh to update.
 *
 * Description:
 *   Updates the specified mesh in VRAM by reuploading it.
 *
 * Usage:
 *   updateMesh(my_mesh);
 *------------------------------------*/
void updateMesh(const triMeshT* mesh) {
    size_t vb_size = sizeof(vertexT) * mesh->num_verts;
    size_t ib_size = sizeof(triT)    * mesh->num_tris;

    glBindBuffer   (GL_ARRAY_BUFFER, mesh->vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vb_size, mesh->verts);

    glBindBuffer   (GL_ARRAY_BUFFER, mesh->ibo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, ib_size, mesh->tris);
}

/*--------------------------------------
 * Function: drawMesh()
 * Parameters:
 *   mesh  The mesh to draw.
 *
 * Description:
 *   Draws the specified mesh. Remember to assign a shader program with the
 *   useShaderProgram() function.
 *
 * Usage:
 *   drawMesh(my_mesh);
 *------------------------------------*/
void drawMesh(const triMeshT* mesh) {
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertexT), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertexT), &((vertexT*)NULL)->n);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertexT), &((vertexT*)NULL)->uv);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);
    glDrawElements(GL_TRIANGLES, mesh->num_tris*3, GL_UNSIGNED_INT, (void*)0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

int meshNumTris(const triMeshT* mesh) {
    return (mesh->num_verts);
}

int meshNumVerts(const triMeshT* mesh) {
    return (mesh->num_tris);
}

triT* meshTrisPtr(triMeshT* mesh) {
    return (mesh->tris);
}

vertexT* meshVertsPtr(triMeshT* mesh) {
    return (mesh->verts);
}

void calcNormals(triMeshT* mesh) {
    for (int i = 0; i < mesh->num_tris; i++) {
        triT* tri = &mesh->tris[i];

        vertexT* v0 = &mesh->verts[tri->v0];
        vertexT* v1 = &mesh->verts[tri->v1];
        vertexT* v2 = &mesh->verts[tri->v2];

        vec3 normal = calcTriNormal(mesh->verts, tri);
        vec_normalize(&normal, &normal);
        v0->n = v1->n = v2->n = normal;
    }

    updateMesh(mesh);
}

void calcSmoothNormals(triMeshT* mesh) {
    for (int i = 0; i < mesh->num_verts; i++)
        mesh->verts[i].n = (vec3) { 0.0f, 0.0f, 0.0f };

    for (int i = 0; i < mesh->num_tris; i++) {
        triT* tri = &mesh->tris[i];

        vertexT* v0 = &mesh->verts[tri->v0];
        vertexT* v1 = &mesh->verts[tri->v1];
        vertexT* v2 = &mesh->verts[tri->v2];

        vec3 normal = calcTriNormal(mesh->verts, tri);

        for (int j = 0; j < mesh->num_verts; j++) {
            vertexT* v = &mesh->verts[j];

            if ((v->smoothing_group==v0->smoothing_group)
             && (v->x==v0->x) && (v->y==v0->y) && (v->z==v0->z))
            {
                vec_add(&normal, &v->n, &v->n);
            }
            else if ((v->smoothing_group==v1->smoothing_group)
             && (v->x==v1->x) && (v->y==v1->y) && (v->z==v1->z))
            {
                vec_add(&normal, &v->n, &v->n);
            }
            else if ((v->smoothing_group==v2->smoothing_group)
             && (v->x==v2->x) && (v->y==v2->y) && (v->z==v2->z))
            {
                vec_add(&normal, &v->n, &v->n);
            }
        }
    }

    for (int i = 0; i < mesh->num_verts; i++) {
        vec3* n = &mesh->verts[i].n;
        vec_normalize(n, n);
    }

    updateMesh(mesh);
}


/*--------------------------------------
 * Function: createBox()
 * Parameters:
 *   width   The width of the box (along the x-axis).
 *   height  The height of the box (y-axis).
 *   length  The length of the box (z-axis).
 *
 * Returns:
 *   A pointer to the generated mesh.
 *
 * Description:
 *   Creates a box mesh by laying out the vertices and attaching the triangles.
 *   The six sides of the box do not share vertices, because if they are shared,
 *   OpenGL interpolates the normals, effectively resulting in a six sided
 *   sphere, which is not what we want.
 *
 * Usage:
 *   triMeshT* cube = createBox(1.0f, 1.0f, 1.0f);
 *------------------------------------*/
triMeshT* createBox(float width, float height, float length) {
    // Divide the measurements in half since we span the box in both ways per
    // dimension.
    width  /= 2.0f;
    height /= 2.0f;
    length /= 2.0f;

    // A box has eight corners, but every side needs their own normals for flat-
    // shading to work properly (we don't want interpolated normals). Since each
    // vertex has its own normal, we need six sides times four vertices. Also,
    // each of the six sides is made up of two triangles.
    triMeshT* box = newMesh(6*4, 6*2);

    // Alias pointers for less code clutter.
    vertexT* v = box->verts;
    triT*    t = box->tris;

    // Front.
    v[0].p = (vec3) {  width,  height, length };
    v[1].p = (vec3) { -width,  height, length };
    v[2].p = (vec3) { -width, -height, length };
    v[3].p = (vec3) {  width, -height, length };

    v[0].k = v[1].k = v[2].k = v[3].k = 0;

    t[0] = (triT) { 0, 1, 2 };
    t[1] = (triT) { 2, 3, 0 };

    // Right.
    v[4].p = (vec3) { width,  height, -length };
    v[5].p = (vec3) { width,  height,  length };
    v[6].p = (vec3) { width, -height,  length };
    v[7].p = (vec3) { width, -height, -length };

    v[4].k = v[5].k = v[6].k = v[7].k = 1;

    t[2] = (triT) { 4, 5, 6 };
    t[3] = (triT) { 6, 7, 4 };

    // Back.
    v[ 8].p = (vec3) { -width,  height, -length };
    v[ 9].p = (vec3) {  width,  height, -length };
    v[10].p = (vec3) {  width, -height, -length };
    v[11].p = (vec3) { -width, -height, -length };

    v[8].k = v[9].k = v[10].k = v[11].k = 2;

    t[4] = (triT) { 8,  9, 10 };
    t[5] = (triT) { 10, 11, 8 };

    // Left.
    v[12].p = (vec3) { -width,  height,  length };
    v[13].p = (vec3) { -width,  height, -length };
    v[14].p = (vec3) { -width, -height, -length };
    v[15].p = (vec3) { -width, -height,  length };

    v[12].k = v[13].k = v[14].k = v[15].k = 3;

    t[6] = (triT) { 12, 13, 14 };
    t[7] = (triT) { 14, 15, 12 };

    // Top.
    v[16].p = (vec3) {  width, height, -length };
    v[17].p = (vec3) { -width, height, -length };
    v[18].p = (vec3) { -width, height,  length };
    v[19].p = (vec3) {  width, height,  length };

    v[16].k = v[17].k = v[18].k = v[19].k = 4;

    t[8] = (triT) { 16, 17, 18 };
    t[9] = (triT) { 18, 19, 16 };

    // Bottom.
    v[20].p = (vec3) {  width, -height,  length };
    v[21].p = (vec3) { -width, -height,  length };
    v[22].p = (vec3) { -width, -height, -length };
    v[23].p = (vec3) {  width, -height, -length };

    v[20].k = v[21].k = v[22].k = v[23].k = 5;

    t[10] = (triT) { 20, 21, 22 };
    t[11] = (triT) { 22, 23, 20 };

    for (int i = 0; i < 24; i += 4) {
        v[i  ].uv = (vec2) { 1.0f, 1.0f };
        v[i+1].uv = (vec2) { 0.0f, 1.0f };
        v[i+2].uv = (vec2) { 0.0f, 0.0f };
        v[i+3].uv = (vec2) { 1.0f, 0.0f };
    }

    calcSmoothNormals(box);

    return (box);
}

/*--------------------------------------
 * Function: createCone()
 * Parameters:
 *   radius     The cone radius (on the xz-plane).
 *   height     The height of the cone (y-axis).
 *   num_sides  Number of sides.
 *
 * Returns:
 *   A pointer to the generated mesh.
 *
 * Description:
 *   Creates a cone mesh. No vertices are shared except for the bottom center
 *   vertex. We get 2*num_sides "wasted" vertices because we need to separate
 *   the normals.
 *
 * Usage:
 *   triMeshT* cone = createCone(0.1, 0.2f, 10);
 *------------------------------------*/
triMeshT* createCone(float radius, float height, int num_sides) {
    // One center bottom vertex, two vertices for each point around the radius
    // (because they need separate normals for shading to work properly) and
    // one separate top point for each face (again, because they need separate
    // normals).
    triMeshT* cone = newMesh(num_sides*6, num_sides*2);

    // Alias pointers for less code clutter.
    vertexT* v = cone->verts;
    triT    *t = cone->tris;

    float r = radius;
    float f = (2.0f * 3.141592653f) / num_sides;
    for (int i = 0; i < num_sides; i++) {
        int j = i*6;

        float a = f*i;
        float b = f*((i+1) % num_sides);

        v[j  ].p = (vec3) { 0.0f, 0.0f, 0.0f };
        v[j+1].p = (vec3) { r*cosf(a), 0.0f, r*sinf(a) };
        v[j+2].p = (vec3) { r*cosf(b), 0.0f, r*sinf(b) };
        v[j+3].p = v[i*6+1].p;
        v[j+4].p = v[i*6+2].p;
        v[j+5].p = (vec3) { 0.0f, height, 0.0f };

        v[j  ].k = v[i*6+1].k = v[i*6+2].k = 0;
        v[j+3].k = v[i*6+4].k = 1;
        v[j+5].k = 2+i;

        t[i*2  ] = (triT) {i*6  , i*6+1, i*6+2 };
        t[i*2+1] = (triT) {i*6+3, i*6+5, i*6+4 };
    }

    calcSmoothNormals(cone);

    return (cone);
}

triMeshT* createGeodesicSphere(float radius, int num_subdivs) {
    error("Not implemented");
}

triMeshT* createQuad(float width, float height) {
    float half_width = width * 0.5f, half_height = height * 0.5f;

    triMeshT* quad = newMesh(4, 2);

    // Alias pointers for less code clutter.
    vertexT* v = quad->verts;
    triT    *t = quad->tris;

    v[0].p = (vec3) {  half_width,  half_height, 0.0f };
    v[1].p = (vec3) { -half_width,  half_height, 0.0f };
    v[2].p = (vec3) { -half_width, -half_height, 0.0f };
    v[3].p = (vec3) {  half_width, -half_height, 0.0f };

    v[0].n = v[1].n = v[2].n = v[3].n = (vec3) { 0.0f, 0.0f, 1.0f };

    v[0].uv = (vec2) { 1.0f, 1.0f };
    v[1].uv = (vec2) { 0.0f, 1.0f };
    v[2].uv = (vec2) { 0.0f, 0.0f };
    v[3].uv = (vec2) { 1.0f, 0.0f };

    t[0] = (triT) { 0, 1, 2 };
    t[1] = (triT) { 2, 3, 0 };

    updateMesh(quad);
    return (quad);
}
