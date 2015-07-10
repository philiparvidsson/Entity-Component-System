#include "engine_private.h"

#include <stdlib.h>

gameSubsystemT* newSubsystem(const string* name) {
    gameSubsystemT* subsystem = calloc(1, sizeof(gameSubsystemT));

    subsystem->name       = name;
    subsystem->components = arrayNew(sizeof(gameComponentT*));

    return (subsystem);
}

void freeSubsystem(gameSubsystemT* subsystem) {
    subsystem->name = NULL;

    if (subsystem->components) {
        arrayFree(subsystem->components);
        subsystem->components = NULL;
    }

    free(subsystem);
}

void addComponentToSubsystem(gameComponentT* component, gameSubsystemT* subsystem) {

}
