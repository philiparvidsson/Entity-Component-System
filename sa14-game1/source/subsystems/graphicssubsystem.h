#ifndef graphicssubsystem_h_
#define graphicssubsystem_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "engine/subsystem.h"

#include "graphics/shader.h"
#include "graphics/texture.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

typedef struct {
    float aspect_ratio;
    shaderT* default_shader;
    textureT* default_texture;
} graphicsSubsystemDataT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

gameSubsystemT* newGraphicsSubsystem(void);

#endif // graphicssubsystem_h_
