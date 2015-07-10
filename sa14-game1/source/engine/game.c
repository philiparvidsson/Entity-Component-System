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

static void updateEntities(const string* subsystem_name) {

}

static void updateSubsystems(void) {
}

void initGame(const string* title, int screen_width, int screen_height) {
    assert(game_inst == NULL);

    initGraphics(title, screen_width, screen_height);

    game_inst = malloc(sizeof(gameT));
}

void exitGame(void) {
    if (!game_inst)
        return;

    free(game_inst);
    game_inst = NULL;

    exitGraphics();
}

void gameMain(void) {
    float dt = 0.0f;
    timeT time = getTime();
    while (windowIsOpen()) {
        time = getTime();

        queryInputDevices();

        updateSubsystems();

        // Pause if we lose focus.
        while (!windowIsFocused()) {
            sleep(10);
            updateDisplay();
        }

        dt += elapsedSecsSince(time);
    }
}

void addEntityToGame(gameEntityT* entity) {
    assert(entity->game == NULL);

    entity->game = game_inst;

    int num_components = arrayLength(entity->components);
    int num_subsystems = arrayLength(game_inst->subsystems);

    for (int i = 0; i < num_components; i++) {
        gameComponentT* component = *(gameComponentT**)arrayGet(entity->components, i);

        for (int j = 0; j < num_subsystems; j++) {
            gameSubsystemT* subsystem = arrayGet(game_inst->subsystems, j);

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
