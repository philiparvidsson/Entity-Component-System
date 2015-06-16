/*------------------------------------------------------------------------------
 * File: meshgen.c
 * Created: June 16, 2015
 * Last changed: June 16, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Functions for generating meshes for geometric objects.
 *----------------------------------------------------------------------------*/

#define VectorSwizzle

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "graphics.h"

#include "core/common.h"
#include "core/linmath.h"

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

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
 *   triMeshT *cube = createBox(1.0f, 1.0f, 1.0f);
 *------------------------------------*/
triMeshT *createBox(float width, float height, float length) {
    // Divide the measurements in half since we span the box in both ways per
    // dimension.
    width  /= 2.0f;
    height /= 2.0f;
    length /= 2.0f;

    // A box has eight corners, but every side needs their own normals for flat-
    // shading to work properly (we don't want interpolated normals). Since each
    // vertex has its own normal, we need six sides times four vertices. Also,
    // each of the six sides is made up of two triangles.
    triMeshT *box = createMesh(6*4, 6*2);

    // Alias pointers for less code clutter.
    vertexT *v = box->verts;
    triT    *t = box->tris;

    // Front.
    v[0].p = (vec3) {  width,  height, length };
    v[1].p = (vec3) { -width,  height, length };
    v[2].p = (vec3) { -width, -height, length };
    v[3].p = (vec3) {  width, -height, length };

    v[0].n = v[1].n = v[2].n = v[3].n = (vec3) { 0.0f, 0.0f, 1.0f };

    t[0] = (triT) { 0, 1, 2 };
    t[1] = (triT) { 2, 3, 0 };

    // Right.
    v[4].p = (vec3) { width,  height, -length };
    v[5].p = (vec3) { width,  height,  length };
    v[6].p = (vec3) { width, -height,  length };
    v[7].p = (vec3) { width, -height, -length };

    v[4].n = v[5].n = v[6].n = v[7].n = (vec3) { 1.0f, 0.0f, 0.0f };

    t[2] = (triT) { 4, 5, 6 };
    t[3] = (triT) { 6, 7, 4 };

    // Back.
    v[ 8].p = (vec3) { -width,  height, -length };
    v[ 9].p = (vec3) {  width,  height, -length };
    v[10].p = (vec3) {  width, -height, -length };
    v[11].p = (vec3) { -width, -height, -length };

    v[8].n = v[9].n = v[10].n = v[11].n = (vec3) { 0.0f, 0.0f, -1.0f };

    t[4] = (triT) { 8,  9, 10 };
    t[5] = (triT) { 10, 11, 8 };

    // Left.
    v[12].p = (vec3) { -width,  height,  length };
    v[13].p = (vec3) { -width,  height, -length };
    v[14].p = (vec3) { -width, -height, -length };
    v[15].p = (vec3) { -width, -height,  length };

    v[12].n = v[13].n = v[14].n = v[15].n = (vec3) { -1.0f, 0.0f, 0.0f };

    t[6] = (triT) { 12, 13, 14 };
    t[7] = (triT) { 14, 15, 12 };

    // Top.
    v[16].p = (vec3) {  width, height, -length };
    v[17].p = (vec3) { -width, height, -length };
    v[18].p = (vec3) { -width, height,  length };
    v[19].p = (vec3) {  width, height,  length };

    v[16].n = v[17].n = v[18].n = v[19].n = (vec3) { 0.0f, 1.0f, 0.0f };

    t[8] = (triT) { 16, 17, 18 };
    t[9] = (triT) { 18, 19, 16 };

    // Bottom
    v[20].p = (vec3) {  width, -height,  length };
    v[21].p = (vec3) { -width, -height,  length };
    v[22].p = (vec3) { -width, -height, -length };
    v[23].p = (vec3) {  width, -height, -length };

    v[20].n = v[21].n = v[22].n = v[23].n = (vec3) { 0.0f, -1.0f, 0.0f };

    t[10] = (triT) { 20, 21, 22 };
    t[11] = (triT) { 22, 23, 20 };

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
 *   triMeshT *cone = createCone(0.1, 0.2f, 10);
 *------------------------------------*/
triMeshT *createCone(float radius, float height, int num_sides) {
    // One center bottom vertex, two vertices for each point around the radius
    // (because they need separate normals for shading to work properly) and
    // one separate top point for each face (again, because they need separate
    // normals).
    triMeshT *cone = createMesh(1+num_sides*3, num_sides*2);

    // Alias pointers for less code clutter.
    vertexT *v = cone->verts;
    triT    *t = cone->tris;

    v[0].p = (vec3) { 0.0f,  0.0f, 0.0f };
    v[0].n = (vec3) { 0.0f, -1.0f, 0.0f };

    // Skip the first vertex.
    v++;
    
    float pi = acosf(-1.0f);
    for (int i = 0; i < num_sides; i++) {
        float theta = 360.0f*(i/(float)num_sides)*(2.0f*pi/360.0f);

        // Bottom.
        v[i].p = (vec3) { radius*cosf(theta),  0.0f, radius*sinf(theta) };
        v[i].n = (vec3) { 0.0f, -1.0f, 0.0f };

        t[i] = (triT) { 0, 1+i, 1+((i+1) % num_sides) };

        // Sides.
        v[num_sides  +i].p = v[i].p;
        v[num_sides*2+i].p = (vec3) { 0.0f, height, 0.0f };

        // First, we create a line from the top of the cone to the current
        // vertex...
        vec3 diff = vec3_sub(v[i].p, v[num_sides*2+i].p);

        // ...then we figure out its perp-vector on the xz-plane.
        vec3 perp = vec3_zyx(v[i].p);
        perp.x = -perp.x;

        // The normal is then the cross product of the perp-vector and the
        // line between the current vertex and the top of the cone.
        vec3 normal = vec3_cross(perp, diff);

        v[num_sides  +i].n = normal;
        v[num_sides*2+i].n = normal;

        t[num_sides+i] = (triT) { 1+  num_sides+  i,
                                  1+2*num_sides+  i,
                                  1+  num_sides+((i+1) % num_sides) };
    }

    updateMesh(cone);
    return (cone);
}
