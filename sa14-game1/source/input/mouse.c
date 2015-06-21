/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "mouse.h"

#include "core/common.h"
#include "core/debug.h"

#include <windows.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

void getMouseState(mouseStateT *mouse_state) {
    POINT cursor_pos;
    assert(GetCursorPos(&cursor_pos));

    //ScreenToClient(0, &cursor_pos);

    mouse_state->x = cursor_pos.x;
    mouse_state->y = cursor_pos.y;

    mouse_state->left_button  = (GetAsyncKeyState(VK_LBUTTON) & 0x8000);
    mouse_state->right_button = (GetAsyncKeyState(VK_RBUTTON) & 0x8000);
}
