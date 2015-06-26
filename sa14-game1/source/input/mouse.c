/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "mouse.h"

#include "core/common.h"
#include "core/debug.h"

#include <windows.h>

struct mouseT {
    int  x, y;
    bool left_button, right_button;
};

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

mouseT* mouseAlloc(void) {
    mouseT* mouse = malloc(sizeof(mouseT));

    return (mouse);
}

void mouseInit(mouseT* mouse) {
    memset(mouse, 0, sizeof(mouseT));
}

mouseT* mouseNew(void) {
    mouseT* mouse = mouseAlloc();
    mouseInit(mouse);

    return (mouse);
}

void mouseFree(mouseT* mouse) {
    free(mouse);
}

void mouseUpdate(mouseT* mouse) {
    POINT cursor_pos;
    assert(GetCursorPos(&cursor_pos));

    //ScreenToClient(0, &cursor_pos);

    mouse->x = cursor_pos.x;
    mouse->y = cursor_pos.y;

    mouse->left_button = (GetAsyncKeyState(VK_LBUTTON) & 0x8000);
    mouse->right_button = (GetAsyncKeyState(VK_RBUTTON) & 0x8000);
}
