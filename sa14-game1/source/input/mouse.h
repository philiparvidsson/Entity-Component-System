#ifndef mouse_h_
#define mouse_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

typedef struct {
    int  x, y;
    bool left_button, right_button;
} mouseStateT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

void getMouseState(mouseStateT* mouse_state);

#endif // mouse_h_
