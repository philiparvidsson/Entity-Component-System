/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "game_private.h"
#include "game.h"

#include "core/common.h"
#include "core/debug.h"
#include "core/time.h"

#include "input/keyboard.h"
#include "input/mouse.h"

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

#define TimeStep (1.0f/120.0f)

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

static gameT* game = NULL;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static void queryInputDevices() {
    getKeyboardState(&game->keyboard);
    getMouseState(&game->mouse);
}

static void updateObjects() {
    entityT* e = game->entities;
    while (e) {
        if (e->updateFunc)
            e->updateFunc(e);

        e = e->next;
    }
}

static void drawAll() {
    clearDisplay(0.0f, 0.0f, 0.6f);

    entityT* e = game->entities;
    while (e) {
        if (e->drawFunc)
            e->drawFunc(e);

        e = e->next;
    }

    updateDisplay();
}

void initGame(void) {
    initGraphics("Game Window", 720, 720);

    game = calloc(1, sizeof(gameT));
    game->world = worldNew();
}

void exitGame() {
    exitGraphics();

    entityT* e = game->entities;
    while (e) {
        if (e->cleanupFunc)
            e->cleanupFunc(e);

        entityT* old_e = e;
        e = e->next;
        free(old_e);
    }

    worldFree(game->world);

    free(game);
    game = NULL;
}

void gameMain(void (*frameFunc(float))) {
    float dt = 0.0f;
    timeT time = getTime();
    while (windowIsOpen()) {
        dt += elapsedSecsSince(time);
        time = getTime();

        if (frameFunc)
            frameFunc(dt);

        queryInputDevices();

        updateObjects();

        while (dt >= TimeStep) {
            worldStep(game->world, TimeStep);
            dt -= TimeStep;
        }

        drawAll();

        // Pause if we lose focus.
        while (!windowIsFocused()) {
            sleep(10);
            updateDisplay();
        }
    }
}

void gameAddEntity(entityT* e) {
    assert(e->game == NULL);

    e->game = game;

    if (game->entities)
        game->entities->prev = e;

    e->next = game->entities;
    game->entities = e;
}

gameT* gameGetInst(void) {
    return (game);
}
