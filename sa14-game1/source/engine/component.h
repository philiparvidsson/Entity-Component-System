#ifndef component_h_
#define component_h_

#include "base/common.h"

typedef struct gameComponentT gameComponentT;
typedef void(*gameComponentUpdateFnT)(gameComponentT*, float);

gameComponentT* newComponent(const string* subsystem_name);
void freeComponent(gameComponentT* component);
void* getComponentDataPtr(gameComponentT* component);
void setComponentDataPtr(gameComponentT* component, void* data);
gameComponentUpdateFnT getComponentUpdateFn(gameComponentT* component);
void setComponentUpdateFn(gameComponentT* component, gameComponentUpdateFnT update_fn);

#endif // component_h_
