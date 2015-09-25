/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "game.h"

#include "base/common.h"
#include "base/debug.h"
#include "base/pak.h"
#include "base/time.h"
#include "graphics/graphics.h"
#include "graphics/text.h"
#include "input/keyboard.h"
#include "input/mouse.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

typedef struct gameResourceT {
    string* name;
    int     type;
    void*   data;

    struct gameResourceT* next;
} gameResourceT;

struct gameT {
    gameResourceT* resources;

    arrayT* entities;
    arrayT* subsystems;

    bool done;
};

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

static gameT* game_inst = NULL;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static void gameCleanup(void) {
    // @To-do: Cleanup components here.

    for (int i = 0; i < arrayLength(game_inst->subsystems); i++) {
        gameSubsystemT* subsystem = *(gameSubsystemT**)arrayGet(game_inst->subsystems, i);
        freeSubsystem(subsystem);
    }

    arrayFree(game_inst->subsystems);

    free(game_inst);
    game_inst = NULL;

    exitGraphics();
}

static void queryInputDevices(void) {
    updateKeyboardState();
    updateMouseState();
}

static void updateComponents(gameSubsystemT* subsystem, float dt) {
    int num_components = arrayLength(subsystem->components);
    for (int i = 0; i < num_components; i++) {
        gameComponentT* component = *(gameComponentT**)arrayGet(subsystem->components, i);

        if (component->update_fn)
            component->update_fn(component, dt);
    }
}

static void updateSubsystems(float dt) {
    int num_subsystems = arrayLength(game_inst->subsystems);
    for (int i = 0; i < num_subsystems; i++) {
        gameSubsystemT* subsystem = *(gameSubsystemT**)arrayGet(game_inst->subsystems, i);

        if (subsystem->before_update_fn)
            subsystem->before_update_fn(subsystem, dt);

        updateComponents(subsystem, dt);

        if (subsystem->after_update_fn)
            subsystem->after_update_fn(subsystem, dt);
    }
}

void initGame(const string* title, int screen_width, int screen_height) {
    assert(game_inst == NULL);

    initGraphics(title, screen_width, screen_height);

    game_inst = malloc(sizeof(gameT));

    game_inst->resources  = NULL;
    game_inst->entities   = arrayNew(sizeof(gameEntityT*));
    game_inst->subsystems = arrayNew(sizeof(gameSubsystemT*));
}

void exitGame(void) {
    if (game_inst)
        game_inst->done = true;
}

void gameMain(void (*frame_func)(float dt)) {
    game_inst->done = false;

    timeT time = getTime();
    while (!game_inst->done && windowIsOpen()) {
        float dt = elapsedSecsSince(time);
        
        // Pause if we lose focus. The time we pause should not be taken into
        // account, so we put this between elapsedSecsSince() and getTime().
        while (!windowIsFocused()) {
            sleep(100);
            
            updateWindow();
        }

        // @To-do: Don't hardcode FPS measurements here.
#ifndef _DEBUG
        if ((dt > (1.0f/59.0f)) || (dt < (1.0f/61.0f)))
            warn("frame time %6.4f ms", dt*1000.0f);
#endif // !_DEBUG

        time = getTime();

        if (frame_func)
            frame_func(dt);

        queryInputDevices();
        updateSubsystems(dt);
        updateDisplay();
    }

    gameCleanup();
}

void addSubsystemToGame(gameSubsystemT* subsystem) {
    arrayAdd(game_inst->subsystems, &subsystem);
}

gameSubsystemT* getGameSubsystem(const string* name) {
    int num_subsystems = arrayLength(game_inst->subsystems);
    for (int i = 0; i < num_subsystems; i++) {
        gameSubsystemT* subsystem = *(gameSubsystemT**)arrayGet(game_inst->subsystems, i);

        if (strcmp(subsystem->name, name)==0)
            return (subsystem);
    }
    
    return (NULL);
}

void addEntityToGame(gameEntityT* entity) {
    assert(entity->game == NULL);
    
    entity->game = game_inst;
    
    int num_components = arrayLength(entity->components);
    for (int i = 0; i < num_components; i++) {
        gameComponentT* component = *(gameComponentT**)arrayGet(entity->components, i);

        gameSubsystemT* subsystem = getGameSubsystem(component->subsystem_name);
        assert(subsystem != NULL);

        addComponentToSubsystem(component, subsystem);
    }
}

void removeEntityFromGame(gameEntityT* entity) {
    if (!entity->game)
        return;

    entity->game = NULL;
}

void gameAddResource(const string* name, void* data, int type) {
    if (gameResource(name, -1))
        error("attempted to add duplicate resource");

    gameResourceT* res = malloc(sizeof(gameResourceT));

    res->data = data;
    res->name = strdup(name);
    res->type = type;
    res->next = game_inst->resources;

    game_inst->resources = res;
}

const void* gameResource(const string* name, int type) {
    assert((name != NULL) && (strlen(name) > 0));

    gameResourceT* res = game_inst->resources;
    while (res) {
        if (((res->type == type) || (res->type == -1))
         && (strcmpi2(name, res->name) == 0))
        {
            return (res->data);
        }

        res = res->next;
    }

    if (type != -1)
        warn("requested resource not found (could be wrong type): %s", name);
    return (NULL);
}
