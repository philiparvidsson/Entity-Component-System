/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "game.h"

#include "base/common.h"
#include "base/debug.h"
#include "base/time.h"

#include "graphics/graphics.h"
#include "graphics/text.h"

#include "input/keyboard.h"
#include "input/mouse.h"

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
