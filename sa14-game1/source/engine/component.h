#ifndef component_h_
#define component_h_

#include "base/common.h"

#include "engine/subsystem.h"

typedef struct gameComponentT gameComponentT;

struct gameComponentT {
    gameSubsystemT* subsystem;
    string* subsystem_name;

    void* data;
    void (*update_fn)(gameComponentT*, float);
};


gameComponentT* newComponent(const string* subsystem_name);
void freeComponent(gameComponentT* component);

#endif // component_h_
