/*------------------------------------------------------------------------------
 * File: meshgen.h
 * Created: June 16, 2015
 * Last changed: June 16, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Functions for generating meshes for geometric objects.
 *----------------------------------------------------------------------------*/

#ifndef meshgen_h_
#define meshgen_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "graphics.h"

#include "core/common.h"

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
 *   The six sides of the box do not share vertices.
 *
 * Usage:
 *   triMeshT *cube = createBox(1.0f, 1.0f, 1.0f);
 *------------------------------------*/
triMeshT *createBox(float width, float height, float length);

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
 *   vertex.
 *
 * Usage:
 *   triMeshT *cone = createCone(0.1, 0.2f, 10);
 *------------------------------------*/
triMeshT *createCone(float radius, float height, int num_sides);

#endif // meshgen_h_
