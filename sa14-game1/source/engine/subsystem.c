#include "subsystem.h"

#include "base/common.h"

#include "engine/component.h"

#include <stdlib.h>

gameSubsystemT* newSubsystem(const string* name) {
    gameSubsystemT* subsystem = calloc(1, sizeof(gameSubsystemT));

    subsystem->name       = name;
    subsystem->components = arrayNew(sizeof(gameComponentT*));

    return (subsystem);
}

void freeSubsystem(gameSubsystemT* subsystem) {
    if (subsystem->components) {
        arrayFree(subsystem->components);
        subsystem->components = NULL;
    }

    if (subsystem->cleanup_fn)
        subsystem->cleanup_fn(subsystem);

    free(subsystem);
}

void addComponentToSubsystem(gameComponentT* component, gameSubsystemT* subsystem) {
    assert(component->subsystem == NULL);
    component->subsystem = subsystem;
    arrayAdd(subsystem->components, &component);

    if (component->init_fn)
        component->init_fn(component);

    if (subsystem->add_component_fn)
        subsystem->add_component_fn(subsystem, component);
}
