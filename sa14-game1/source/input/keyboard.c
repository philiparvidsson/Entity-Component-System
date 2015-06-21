/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "keyboard.h"

#include "core/common.h"
#include "core/debug.h"

#include <windows.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

void getKeyboardState(keyboardStateT *keyboard_state) {
    BYTE keys[256];
    assert(GetKeyboardState(keys));
}

bool isKeyPressed(int key, keyboardStateT const *keyboard_state) {
    return (false);
}
