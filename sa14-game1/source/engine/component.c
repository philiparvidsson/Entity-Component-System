#include "engine_private.h"

#include "base/common.h"

#include <stdlib.h>

gameComponentT* newComponent(const string* subsystem_name) {
    gameComponentT* component = calloc(1, sizeof(gameComponentT));

    component->subsystem_name = subsystem_name;

    return (component);
}

void freeComponent(gameComponentT* component) {
    free(component);
}

void* getComponentDataPtr(gameComponentT* component) {
    return (component->data_ptr);
}

void setComponentDataPtr(gameComponentT* component, void* data) {
    component->data_ptr = data;
}

gameComponentUpdateFnT getComponentUpdateFunction(gameComponentT* component) {
    return (component->update_fn);
}

void setComponentUpdateFunction(gameComponentT* component, gameComponentUpdateFnT update_fn) {
    component->update_fn = update_fn;
}
