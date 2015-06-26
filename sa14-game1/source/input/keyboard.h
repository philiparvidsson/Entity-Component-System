#ifndef keyboard_h_
#define keyboard_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

typedef struct keyboardT keyboardT;

typedef enum {
    ArrowLeft,
    ArrowRight,
    ArrowUp,
    ArrowDown
} keyboardKeyT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

keyboardT* keyboardAlloc(void);
void keyboardInit(keyboardT* keyboard);
keyboardT* keyboardNew(void);
void keyboardFree(keyboardT* keyboard);
void keyboardUpdate(keyboardT* keyboard);
bool keyIsPressed(const keyboardT* keyboard, keyboardKeyT key);

#endif // keyboard_h_
