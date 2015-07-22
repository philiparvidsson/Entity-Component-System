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

vec3 calcTriNormal(vertexT* verts, triT* tri) {
    vertexT* v0 = &verts[tri->v0];
    vertexT* v1 = &verts[tri->v1];
    vertexT* v2 = &verts[tri->v2];

    vec3 edge0, edge1, normal;
    vec_sub(&v1->p, &v0->p, &edge0);
    vec_sub(&v2->p, &v0->p, &edge1);
    vec3_cross(&edge0, &edge1, &normal);

    return (normal);
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

    triMeshT* box = newMesh(6*4, 6*2);

    // Alias pointers for less code clutter.
    vertexT* v = box->verts;
    triT*    t = box->tris;

    v[ 0].p = (vec3) {  width,  height,  length };
    v[ 1].p = (vec3) { -width,  height,  length };
    v[ 2].p = (vec3) { -width, -height,  length };
    v[ 3].p = (vec3) {  width, -height,  length };
    v[ 4].p = (vec3) {  width,  height, -length };
    v[ 5].p = (vec3) {  width,  height,  length };
    v[ 6].p = (vec3) {  width, -height,  length };
    v[ 7].p = (vec3) {  width, -height, -length };
    v[ 8].p = (vec3) { -width,  height, -length };
    v[ 9].p = (vec3) {  width,  height, -length };
    v[10].p = (vec3) {  width, -height, -length };
    v[11].p = (vec3) { -width, -height, -length };
    v[12].p = (vec3) { -width,  height,  length };
    v[13].p = (vec3) { -width,  height, -length };
    v[14].p = (vec3) { -width, -height, -length };
    v[15].p = (vec3) { -width, -height,  length };
    v[16].p = (vec3) {  width,  height, -length };
    v[17].p = (vec3) { -width,  height, -length };
    v[18].p = (vec3) { -width,  height,  length };
    v[19].p = (vec3) {  width,  height,  length };
    v[20].p = (vec3) {  width, -height,  length };
    v[21].p = (vec3) { -width, -height,  length };
    v[22].p = (vec3) { -width, -height, -length };
    v[23].p = (vec3) {  width, -height, -length };

    v[ 0].k = v[1].k = v[2].k = v[3].k = 0;
    v[ 4].k = v[5].k = v[6].k = v[7].k = 1;
    v[ 8].k = v[9].k = v[10].k = v[11].k = 2;
    v[12].k = v[13].k = v[14].k = v[15].k = 3;
    v[16].k = v[17].k = v[18].k = v[19].k = 4;
    v[20].k = v[21].k = v[22].k = v[23].k = 5;

    t[ 0] = (triT) {  0,  1,  2 };
    t[ 1] = (triT) {  2,  3,  0 };
    t[ 2] = (triT) {  4,  5,  6 };
    t[ 3] = (triT) {  6,  7,  4 };
    t[ 4] = (triT) {  8,  9, 10 };
    t[ 5] = (triT) { 10, 11,  8 };
    t[ 6] = (triT) { 12, 13, 14 };
    t[ 7] = (triT) { 14, 15, 12 };
    t[ 8] = (triT) { 16, 17, 18 };
    t[ 9] = (triT) { 18, 19, 16 };
    t[10] = (triT) { 20, 21, 22 };
    t[11] = (triT) { 22, 23, 20 };

    for (int i = 0; i < 24; i += 4) {
        v[i  ].uv = (vec2) { 1.0f, 1.0f };
        v[i+1].uv = (vec2) { 0.0f, 1.0f };
        v[i+2].uv = (vec2) { 0.0f, 0.0f };
        v[i+3].uv = (vec2) { 1.0f, 0.0f };
    }

    calcNormals(box);
    updateMesh(box);

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

        t[i*2  ] = (triT) {j  , j+1, j+2 };
        t[i*2+1] = (triT) {j+3, j+5, j+4 };
    }

    calcSmoothNormals(cone);
    updateMesh(cone);

    return (cone);
}

triMeshT* createCylinder(float radius, float height, int num_sides) {
    height /= 2.0f;

    triMeshT* cylinder = newMesh(num_sides*12, num_sides*4);

    // Alias pointers for less code clutter.
    vertexT* v = cylinder->verts;
    triT    *t = cylinder->tris;

    float r = radius;
    float f = (2.0f * 3.141592653f) / num_sides;
    for (int i = 0; i < num_sides; i++) {
        int j = i*12;

        float a = f*i;
        float b = f*((i+1) % num_sides);

        float cos_a = cosf(a), sin_a = sinf(a),
              cos_b = cosf(b), sin_b = sinf(b);

        // Bottom triangle.
        v[j   ].p = (vec3) {    0.0f, -height,    0.0f };
        v[j+ 1].p = (vec3) { r*cos_a, -height, r*sin_a };
        v[j+ 2].p = (vec3) { r*cos_b, -height, r*sin_b };

        // First side triangle.
        v[j+ 3].p = v[j+1].p;
        v[j+ 4].p = (vec3) { r*cos_b,  height, r*sin_b };
        v[j+ 5].p = v[j+2].p;

        // Second side triangle.
        v[j+ 6].p = v[j+1].p;
        v[j+ 7].p = (vec3) { r*cos_a,  height, r*sin_a };
        v[j+ 8].p = v[j+4].p;

        // Top triangle.
        v[j+ 9].p = v[j+7].p;
        v[j+10].p = (vec3) { 0.0f,  height, 0.0f };
        v[j+11].p = v[j+4].p;

        v[j  ].k = v[j+ 1].k = v[j+ 2].k = 0;
        v[j+3].k = v[j+ 4].k = v[j+ 5].k = 1;
        v[j+6].k = v[j+ 7].k = v[j+ 8].k = 1;
        v[j+9].k = v[j+10].k = v[j+11].k = 2;

        t[i*4  ] = (triT) { j  , j+ 1, j+ 2 };
        t[i*4+1] = (triT) { j+3, j+ 4, j+ 5 };
        t[i*4+2] = (triT) { j+6, j+ 7, j+ 8 };
        t[i*4+3] = (triT) { j+9, j+10, j+11 };
    }

    calcSmoothNormals(cylinder);
    updateMesh(cylinder);

    return (cylinder);
}

triMeshT* createGeodesicSphere(float radius, int num_subdivs) {
    assert(0 <= num_subdivs && num_subdivs < 5);

    int kw = (int)pow(5, num_subdivs);
    triMeshT* sphere = newMesh(60*kw, 20*kw);

    // We always start with 60 vertices and 20 triangles, then subdivide.
    sphere->num_verts = 60;
    sphere->num_tris =  20;

    // Alias pointers for less code clutter.
    vertexT* v = sphere->verts;
    triT    *t = sphere->tris;

    float r = (1.0f + sqrtf(5.0)) / 2.0f;

    v[ 0].p = (vec3) { -1.0f,  r, 0.0f };
    v[ 5].p = (vec3) {  1.0f,  r, 0.0f };
    v[10].p = (vec3) { -1.0f, -r, 0.0f };
    v[15].p = (vec3) {  1.0f, -r, 0.0f };
    v[20].p = (vec3) { 0.0f, -1.0f,  r };
    v[25].p = (vec3) { 0.0f,  1.0f,  r };
    v[30].p = (vec3) { 0.0f, -1.0f, -r };
    v[35].p = (vec3) { 0.0f,  1.0f, -r };
    v[40].p = (vec3) {  r, 0.0f, -1.0f };
    v[45].p = (vec3) {  r, 0.0f,  1.0f };
    v[50].p = (vec3) { -r, 0.0f, -1.0f };
    v[55].p = (vec3) { -r, 0.0f,  1.0f };

    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 5; j++)
            v[i*5+j].p = v[i*5].p;
    }

    t[0] = (triT) { 0, 11,  5 };
    t[1] = (triT) { 0,  5,  1 };
    t[2] = (triT) { 0,  1,  7 };
    t[3] = (triT) { 0,  7, 10 };
    t[4] = (triT) { 0, 10, 11 };

    t[5] = (triT) {  1,  5, 9 };
    t[6] = (triT) {  5, 11, 4 };
    t[7] = (triT) { 11, 10, 2 };
    t[8] = (triT) { 10,  7, 6 };
    t[9] = (triT) {  7,  1, 8 };

    t[10] = (triT) { 3, 9, 4 };
    t[11] = (triT) { 3, 4, 2 };
    t[12] = (triT) { 3, 2, 6 };
    t[13] = (triT) { 3, 6, 8 };
    t[14] = (triT) { 3, 8, 9 };

    t[15] = (triT) { 4, 9,  5 };
    t[16] = (triT) { 2, 4, 11 };
    t[17] = (triT) { 6, 2, 10 };
    t[18] = (triT) { 8, 6,  7 };
    t[19] = (triT) { 9, 8,  1 };

    int indices[12] = { 0 };

    for (int i = 0; i < sphere->num_tris; i++) {
        int a = indices[t[i].v0]++;
        int b = indices[t[i].v1]++;
        int c = indices[t[i].v2]++;

        t[i].v0 = t[i].v0*5 + a;
        t[i].v1 = t[i].v1*5 + b;
        t[i].v2 = t[i].v2*5 + c;
    }

    for (int i = 0; i < num_subdivs; i++) {
        int num_tris = sphere->num_tris;
        for (int j = 0; j < num_tris; j++) {
            triT* tri = &sphere->tris[j];

            vertexT* v0 = &v[tri->v0];
            vertexT* v1 = &v[tri->v1];
            vertexT* v2 = &v[tri->v2];

            vec3 a = v0->p;
            vec_add  (&a, &v1->p, &a);
            vec_scale(&a, 0.5f  , &a);

            vec3 b = v1->p;
            vec_add  (&b, &v2->p, &b);
            vec_scale(&b, 0.5f  , &b);

            vec3 c = v2->p;
            vec_add  (&c, &v0->p, &c);
            vec_scale(&c, 0.5f  , &c);

            int k = sphere->num_verts;

            v[k +  0].p = a;
            v[k +  1].p = b;
            v[k +  2].p = c;
            v[k +  3].p = a;
            v[k +  4].p = b;
            v[k +  5].p = c;
            v[k +  6].p = a;
            v[k +  7].p = b;
            v[k +  8].p = c;

            t[sphere->num_tris + 0] = (triT) { tri->v0, k    , k + 2 };
            t[sphere->num_tris + 1] = (triT) { tri->v1, k + 1, k + 3 };
            t[sphere->num_tris + 2] = (triT) { tri->v2, k + 5, k + 4 };
            t[sphere->num_tris + 3] = (triT) { k + 6  , k + 7, k + 8 };

            sphere->num_verts += 9;
            sphere->num_tris  += 4;
        }
    }

    for (int i = 0; i < sphere->num_verts; i++) {
        vec_normalize(&v[i].p, &v[i].p);
        vec_scale(&v[i].p, radius, &v[i].p);
    }

    calcNormals(sphere);
    updateMesh(sphere);

    return (sphere);
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

    v[0].uv = (vec2) { 1.0f, 1.0f };
    v[1].uv = (vec2) { 0.0f, 1.0f };
    v[2].uv = (vec2) { 0.0f, 0.0f };
    v[3].uv = (vec2) { 1.0f, 0.0f };

    t[0] = (triT) { 0, 1, 2 };
    t[1] = (triT) { 2, 3, 0 };

    calcNormals(quad);
    updateMesh(quad);

    return (quad);
}
