#ifndef keyboard_h_
#define keyboard_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

#define KeyboardNumKeys 256

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

typedef enum {
    ArrowLeft,
    ArrowRight,
    ArrowUp,
    ArrowDown
} keyboardKeyT;

typedef struct {
    bool alt, ctrl, shift;
    bool keys[KeyboardNumKeys];
} keyboardStateT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

void getKeyboardState(keyboardStateT* keyboard_state);

static inline bool keyIsPressed(const keyboardStateT* keyboard_state,
                                      keyboardKeyT key)
{
    return (keyboard_state->keys[key % KeyboardNumKeys]);
}


#endif // keyboard_h_
