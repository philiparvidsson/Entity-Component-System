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
    int lol;
} keyboardStateT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

void getKeyboardState(keyboardStateT *keyboard_state);

bool isKeyPressed(int key, keyboardStateT const *keyboard_state);

#endif // keyboard_h_
