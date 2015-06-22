#include "world.h"

#include "core/array.h"
#include "core/common.h"

void stepWorld(worldT *world, float dt) {
    int num_objects = arrayLength(world->objects);

    for (int i = 0; i < num_objects; i++) {
        void *object = arrayGet(world->objects, i);
    }
}
