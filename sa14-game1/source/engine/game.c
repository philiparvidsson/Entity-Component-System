/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "engine_private.h"

#include "game.h"

#include "base/common.h"
#include "base/debug.h"
#include "base/time.h"

#include "graphics/graphics.h"
#include "graphics/text.h"

#include "input/keyboard.h"
#include "input/mouse.h"

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

#define TimeStep (1.0f/120.0f)

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

static gameT* game_inst = NULL;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

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
            subsystem->before_update_fn(subsystem);

        updateComponents(subsystem, dt);

        if (subsystem->after_update_fn)
            subsystem->after_update_fn(subsystem);
    }
}

void initGame(const string* title, int screen_width, int screen_height) {
    assert(game_inst == NULL);

    initGraphics(title, screen_width, screen_height);

    game_inst = malloc(sizeof(gameT));

    game_inst->entities = arrayNew(sizeof(gameEntityT*));
    game_inst->subsystems = arrayNew(sizeof(gameSubsystemT*));
}

void exitGame(void) {
    if (!game_inst)
        return;

    free(game_inst);
    game_inst = NULL;

    exitGraphics();
}

void gameMain(void) {
    timeT time = getTime();
    while (windowIsOpen()) {
        float dt = elapsedSecsSince(time);
        time = getTime();

        queryInputDevices();
        updateSubsystems(dt);
        updateDisplay();

        // Pause if we lose focus.
        while (!windowIsFocused()) {
            sleep(10);
            updateDisplay();
        }

    }
}

void addSubsystemToGame(gameSubsystemT* subsystem) {
    arrayAdd(game_inst->subsystems, &subsystem);
}

void addEntityToGame(gameEntityT* entity) {
    assert(entity->game == NULL);

    entity->game = game_inst;

    int num_components = arrayLength(entity->components);
    int num_subsystems = arrayLength(game_inst->subsystems);

    for (int i = 0; i < num_components; i++) {
        gameComponentT* component = *(gameComponentT**)arrayGet(entity->components, i);

        for (int j = 0; j < num_subsystems; j++) {
            gameSubsystemT* subsystem = *(gameSubsystemT**)arrayGet(game_inst->subsystems, j);

            if ((component->subsystem_name == subsystem->name) ||
                strcmp(component->subsystem_name, subsystem->name)==0)
            {
                assert(component->subsystem == NULL);

                component->subsystem = subsystem;
                arrayAdd(subsystem->components, &component);
                break;
            }
        }
    }
}

void removeEntityFromGame(gameEntityT* entity) {
    if (!entity->game)
        return;

    entity->game = NULL;
}
