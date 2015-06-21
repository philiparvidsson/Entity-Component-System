#ifndef mouse_h_
#define mouse_h_

#include "core/common.h"

typedef struct {
    int  x, y;
    bool left_button, right_button;
} mouseStateT;

void getMouseState(mouseStateT *mouse_state);

#endif // mouse_h_
