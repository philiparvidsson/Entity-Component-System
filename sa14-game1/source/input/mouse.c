#include "mouse.h"

#include <windows.h>

mouseStateT getMouseState(void) {
    POINT cursor_pos;
    GetCursorPos(&cursor_pos);

    mouseStateT mouse_state = { 0 };

    //ScreenToClient(0, &cursor_pos);

    mouse_state.x = cursor_pos.x;
    mouse_state.y = cursor_pos.y;
}
