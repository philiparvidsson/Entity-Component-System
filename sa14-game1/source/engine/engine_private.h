#ifndef engine_private_h_
#define engine_private_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/array.h"
#include "base/common.h"

#include "component.h"
#include "entity.h"
#include "game.h"
#include "subsystem.h"

#include "input/keyboard.h"
#include "input/mouse.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/


struct gameComponentT {
    subsystemT* subsystem;
    string* subsystem_name;

    void* data_ptr;
    gameComponentUpdateFnT update_fn;
};

struct gameEntityT {
    gameT* game;
    arrayT* components;
};

struct gameSubsystemT {
    string* name;
    arrayT* components;
};

struct gameT {
    gameFrameFuncT frame_func;

    // Below are the input devices. They will be queried at the start of every
    // single frame.
    keyboardStateT keyboard_state;
    mouseStateT    mouse_state;

    gameEntityT* entities;
    gameSubsystemT* subsystems;
};

#endif // engine_private_h_
