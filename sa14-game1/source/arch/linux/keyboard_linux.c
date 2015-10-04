#ifdef __linux__

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/common.h"
#include "base/debug.h"
#include "input/keyboard.h"

#include <stdint.h>

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

static keyboardStateT keyboard_state;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

void updateKeyboardState(void) {
}

keyboardStateT getKeyboardState(void) {
    return (keyboard_state);
}

bool keyIsPressed(keyboardKeyT key) {
    return (keyboard_state.keys[key % KeyboardNumKeys]);
}

#endif // __linux__
