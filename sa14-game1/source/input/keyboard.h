#ifndef keyboard_h_
#define keyboard_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/common.h"

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

void updateKeyboardState(void);

keyboardStateT getKeyboardState(void);

bool keyIsPressed(keyboardKeyT key);

#endif // keyboard_h_
