#ifndef subsystem_h_
#define subsystem_h_

#include "base/array.h"
#include "base/common.h"

typedef struct gameSubsystemT gameSubsystemT;
struct gameSubsystemT {
    string* name;
    arrayT* components;

    void (*before_update_fn)(gameSubsystemT*);
    void (*after_update_fn)(gameSubsystemT*);

    void* data;
};


gameSubsystemT* newSubsystem(const string* name);
void freeSubsystem(gameSubsystemT* subsystem);

#endif // subsystem_h_
