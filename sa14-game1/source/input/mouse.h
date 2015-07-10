#ifndef mouse_h_
#define mouse_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/common.h"

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

void updateMouseState(void);

mouseStateT getMouseState(void);

#endif // mouse_h_
