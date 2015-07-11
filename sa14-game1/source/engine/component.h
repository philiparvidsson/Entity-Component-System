#ifndef component_h_
#define component_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/common.h"

typedef struct gameComponentT gameComponentT;

#include "engine/entity.h"
#include "engine/subsystem.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

struct gameComponentT {
    gameEntityT* entity;
    gameSubsystemT* subsystem;
    string* subsystem_name;

    void* data;
    void (*init_fn)(gameComponentT*);
    void (*update_fn)(gameComponentT*, float);
    void (*cleanup_fn)(gameComponentT*);
};

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

gameComponentT* newComponent(const string* subsystem_name);
void freeComponent(gameComponentT* component);

#endif // component_h_
