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
    arrayT* paks;

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

    for (int i = 0; i < arrayLength(game_inst->paks); i++) {
        pakArchiveT* pak = *(pakArchiveT**)arrayGet(game_inst->paks, i);
        pakCloseArchive(pak);
    }

    arrayFree(game_inst->paks);

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

    game_inst->resources = NULL;

    game_inst->paks       = arrayNew(sizeof(pakArchiveT*));
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
        if ((dt > (1.0f/59.0f)) || (dt < (1.0f/61.0f)))
            warn("frame time %6.4f ms", dt*1000.0f);

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

void addGamePak(pakArchiveT* pak) {
    assert(pak != NULL);

    arrayAdd(game_inst->paks, &pak);
}

uint8_t* readGamePakFile(const string* file_name) {
    for (int i = arrayLength(game_inst->paks)-1; i >= 0; i--) {
        pakArchiveT* pak = *(pakArchiveT**)arrayGet(game_inst->paks, i);

        uint8_t* data = pakReadFile(pak, file_name);
        if (data)
            return (data);
    }

    error("couldn't find file '%s' in any game pak", file_name);
    return (NULL);
}

int gamePakFileSize(const string* file_name) {
    for (int i = arrayLength(game_inst->paks)-1; i >= 0; i--) {
        pakArchiveT* pak = *(pakArchiveT**)arrayGet(game_inst->paks, i);

        pakFileT* pf = pakOpenFile(pak, file_name);
        if (pf) {
            int file_size = pakFileSize(pf);
            pakCloseFile(pf);
            return (file_size);
        }
    }

    error("couldn't find file '%s' in any game pak", file_name);
    return (NULL);
}

void gameAddResource(const string* name, void* data, int type) {
    gameResourceT* res = malloc(sizeof(gameResourceT));

    res->data = data;
    res->name = strdup(name);
    res->type = type;
    res->next = game_inst->resources;

    game_inst->resources = res;
}

void* gameResource(const string* name, int type) {
    assert((name != NULL) && (strlen(name) > 0));

    gameResourceT* res = game_inst->resources;
    while (res) {
        if ((res->type == type) && (strcmp(name, res->name) == 0))
            return (res->data);

        res = res->next;
    }

    return (NULL);
}

/*void* delayedFree(void* mem) {
    arrayAdd(game_inst->delayed_free, &mem);
    return (mem);
}*/
