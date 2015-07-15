#ifndef graphicssubsystem_h_
#define graphicssubsystem_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/common.h"
#include "engine/component.h"
#include "engine/subsystem.h"
#include "graphics/trimesh.h"

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

gameComponentT* newGraphicsComponent(triMeshT* mesh);
gameSubsystemT* newGraphicsSubsystem(void);

#endif // graphicssubsystem_h_
