#ifndef gameobject_h_
#define gameobject_h_

#include "game/game.h"

struct gameObjectT {
    gameT* game;
    void*  data;

    void (*cleanupFunc)(gameObjectT*);
    void (*drawFunc)(const gameObjectT*);
    void (*updateFunc)(gameObjectT*);

    gameObjectT* prev;
    gameObjectT* next;
};

#endif // gameobject_h_
