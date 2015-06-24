#ifndef keyboard_h_
#define keyboard_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

typedef struct {
    bool key_states[256];
} keyboardStateT;

typedef enum {
    ArrowLeft,
    ArrowRight,
    ArrowUp,
    ArrowDown
} keyboardKeyT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

void getKeyboardState(keyboardStateT* keyboard_state);

bool keyIsPressed(const keyboardStateT* keyboard_state, keyboardKeyT key);

#endif // keyboard_h_
