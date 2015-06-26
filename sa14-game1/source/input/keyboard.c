/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "keyboard.h"

#include "core/common.h"
#include "core/debug.h"

#include <stdint.h>

#include <windows.h>

struct keyboardT {
    bool state[256];
};

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

keyboardT* keyboardAlloc(void) {
    keyboardT* keyboard = malloc(sizeof(keyboardT));

    return (keyboard);
}

void keyboardInit(keyboardT* keyboard) {
    memset(keyboard, 0, sizeof(keyboardT));
}

keyboardT* keyboardNew(void) {
    keyboardT* keyboard = keyboardAlloc();
    keyboardInit(keyboard);

    return (keyboard);
}

void keyboardFree(keyboardT* keyboard) {
    free(keyboard);
}

void keyboardUpdate(keyboardT* kb) {
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
        kb->state[i] = false;

    for (int i = 0; i < 256; i++) {
        if (!(keys[i] & 0x80))
            continue;

        for (int j = 0; j < 256; j++) {
            if (map_from[j] == i) {
                kb->state[map_to[j]] = true;
                break;
            }
        }
    }
}

bool keyIsPressed(const keyboardT* keyboard, keyboardKeyT key) {
    return (keyboard->state[key & 255]);
}
