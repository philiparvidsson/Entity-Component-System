#ifndef subsystem_h_
#define subsystem_h_

#include "base/array.h"
#include "base/common.h"

typedef struct gameSubsystemT gameSubsystemT;

#include "engine/component.h"

struct gameSubsystemT {
    const string* name;
    arrayT* components;

    void (*before_update_fn)(gameSubsystemT*, float);
    void (*after_update_fn)(gameSubsystemT*, float);
    void (*add_component_fn)(gameSubsystemT*, gameComponentT*);
    void (*cleanup_fn)(gameSubsystemT*);

    void* data;
};


gameSubsystemT* newSubsystem(const string* name);
void freeSubsystem(gameSubsystemT* subsystem);

void addComponentToSubsystem(gameComponentT* component, gameSubsystemT* subsystem);

#endif // subsystem_h_
