#ifdef __linux__

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/common.h"
#include "base/debug.h"
#include "input/mouse.h"

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

static mouseStateT mouse_state;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

void updateMouseState(void) {
}

mouseStateT getMouseState(void) {
    return (mouse_state);
}

#endif // __linux__
