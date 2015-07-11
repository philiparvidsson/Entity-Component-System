#ifndef graphicssubsystem_h_
#define graphicssubsystem_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "engine/subsystem.h"

#include "graphics/shader.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

typedef struct {
    shaderT* default_shader;
} graphicsSubsystemDataT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

gameSubsystemT* newGraphicsSubsystem(void);

#endif // graphicssubsystem_h_
