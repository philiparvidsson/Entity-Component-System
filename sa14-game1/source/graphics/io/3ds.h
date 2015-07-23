/*------------------------------------------------------------------------------
 * File: 3ds.h
 * Created: July 23, 2015
 * Last changed: July 24, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Provides functionality for loading .3ds-file data.
 *----------------------------------------------------------------------------*/

#ifndef x3ds_h_
#define x3ds_h_

/*------------------------------------------------
 * INCLUDE
 *----------------------------------------------*/

#include "graphics/trimesh.h"

#include <stdint.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

triMeshT* load3DS(const uint8_t* ptr);

#endif // x3ds_h_
