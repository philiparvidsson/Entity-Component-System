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

struct gameT {
    arrayT* paks;
    arrayT* entities;
    arrayT* subsystems;
};

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

static bool done = false;
static gameT* game_inst = NULL;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static void gameCleanup() {
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

    game_inst->paks = arrayNew(sizeof(pakArchiveT*));
    game_inst->entities = arrayNew(sizeof(gameEntityT*));
    game_inst->subsystems = arrayNew(sizeof(gameSubsystemT*));
}

void exitGame(void) {
    done = true;
}

void gameMain(void) {
    done = false;

    timeT time = getTime();
    while (!done && windowIsOpen()) {
        float dt = elapsedSecsSince(time);

        // Pause if we lose focus. The time we pause should not be taken into
        // account, so we put this between elapsedSecsSince() and getTime().
        while (!windowIsFocused()) {
            sleep(100);
            
            // Doing this twice prevents flickering.
            updateDisplay();
            updateDisplay();
        }

        time = getTime();

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

char* readGamePakFile(const string* file_name) {
    for (int i = 0; i < arrayLength(game_inst->paks); i++) {
        pakArchiveT* pak = *(pakArchiveT**)arrayGet(game_inst->paks, i);

        char* data = pakReadFile(pak, file_name);
        if (data)
            return (data);
    }

    return (NULL);
}