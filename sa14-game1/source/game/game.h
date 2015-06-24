#ifndef game_h_
#define game_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

typedef struct gameObjectT gameObjectT;
typedef struct gameT gameT;

#include "core/array.h"
#include "core/common.h"
#include "core/file_io.h"

#include "game/gameobject.h"

#include "gfx/graphics.h"
#include "gfx/shader.h"
#include "gfx/trimesh.h"

#include "input/keyboard.h"
#include "input/mouse.h"

#include "math/matrix.h"
#include "math/vector.h"

#include "physics/physics.h"

#include <stdlib.h>

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/


struct gameT {
    // Below are the input devices. They will be queried at the start of every
    // single frame.
    keyboardStateT keyboard;
    mouseStateT    mouse;

    // The list of game objects.
    gameObjectT* object_list;

    // The game object that represents the player.
    gameObjectT* player;

    // The world used for physical simulation of all game object bodies.
    worldT* world;
};



/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

#define TimeStep (1.0f/120.0f)

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

void gameMain(void);
void gameAddObject(gameT* game, gameObjectT* o);

#endif // game_h_
