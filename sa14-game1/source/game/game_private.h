#ifndef game_private_h_
#define game_private_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "game.h"

#include "input/keyboard.h"
#include "input/mouse.h"

#include "physics/physics.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

struct entityT {
    gameT* game;
    void*  data;

    entityCleanupFuncT cleanup_func;
    entityDrawFuncT    draw_func;
    entityUpdateFuncT  update_func;

    entityT* prev;
    entityT* next;
};

struct gameT {
    gameFrameFuncT frame_func;

    // Below are the input devices. They will be queried at the start of every
    // single frame.
    keyboardStateT keyboard;
    mouseStateT    mouse;

    // The list of game entities.
    entityT* entities;

    // The world used for physical simulation of all game entities.
    worldT* world;
};

#endif
