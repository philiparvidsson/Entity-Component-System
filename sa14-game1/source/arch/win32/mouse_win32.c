#ifdef WIN32

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/common.h"
#include "base/debug.h"
#include "input/mouse.h"

#include <windows.h>

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

static mouseStateT mouse_state;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

void updateMouseState(void) {
    POINT cursor_pos;
    assert(GetCursorPos(&cursor_pos));

    //ScreenToClient(0, &cursor_pos);

    mouse_state.x = cursor_pos.x;
    mouse_state.y = cursor_pos.y;

    mouse_state.left_button  = (GetAsyncKeyState(VK_LBUTTON) & 0x8000);
    mouse_state.right_button = (GetAsyncKeyState(VK_RBUTTON) & 0x8000);
}

mouseStateT getMouseState(void) {
    return (mouse_state);
}

#endif // WIN32
