/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "keyboard.h"

#include "core/common.h"
#include "core/debug.h"

#include <stdint.h>

#include <windows.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

void getKeyboardState(keyboardStateT* keyboard_state) {
    BYTE keys[256];
    assert(GetKeyboardState(keys));

    uint8_t map_from[256] = {
        VK_UP,
        VK_DOWN,
        VK_LEFT,
        VK_RIGHT
    };

    uint8_t map_to[256] = {
        ArrowUp,
        ArrowDown,
        ArrowLeft,
        ArrowRight
    };

    for (int i = 0; i < 256; i++)
        keyboard_state->key_states[i] = false;

    for (int i = 0; i < 256; i++) {
        if (!(keys[i] & 0x80))
            continue;

        for (int j = 0; j < 256; j++) {
            if (map_from[j] == i) {
                keyboard_state->key_states[map_to[j]] = 1;
                break;
            }
        }
    }
}

bool keyIsPressed(const keyboardStateT* keyboard_state, keyboardKeyT key) {
    return (keyboard_state->key_states[key & 255]);
}
