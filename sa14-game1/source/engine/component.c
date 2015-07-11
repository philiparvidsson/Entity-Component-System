/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "component.h"

#include "base/common.h"

#include <stdlib.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

gameComponentT* newComponent(const string* subsystem_name) {
    gameComponentT* component = calloc(1, sizeof(gameComponentT));

    component->subsystem_name = subsystem_name;

    return (component);
}

void freeComponent(gameComponentT* component) {
    free(component);
}
