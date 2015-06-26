/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "game_private.h"
#include "game.h"

#include "core/common.h"
#include "core/debug.h"
#include "core/time.h"

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

static gameT* game = NULL;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static void queryInputDevices() {
    keyboardUpdate(game->keyboard);
    mouseUpdate(game->mouse);
}

static void updateObjects() {
    entityT* e = game->entities;
    while (e) {
        if (e->update_func)
            e->update_func(e);

        e = e->next;
    }
}

static void drawAll() {
    clearDisplay(0.0f, 0.0f, 0.6f);

    entityT* e = game->entities;
    while (e) {
        if (e->draw_func)
            e->draw_func(e);

        e = e->next;
    }

    updateDisplay();
}

void initGame(void) {
    initGraphics("Game Window", 720, 720);

    game = calloc(1, sizeof(gameT));

    game->keyboard = keyboardNew();
    game->mouse = mouseNew();

    game->world = worldNew();
}

void exitGame() {
    keyboardFree(game->keyboard);
    mouseFree(game->mouse);

    entityT* e = game->entities;
    while (e) {
        if (e->cleanup_func)
            e->cleanup_func(e);

        entityT* old_e = e;
        e = e->next;
        free(old_e);
    }

    worldFree(game->world);

    free(game);
    game = NULL;

    exitGraphics();
}

void gameMain(void) {
    float dt = 0.0f;
    timeT time = getTime();
    while (windowIsOpen()) {
        time = getTime();

        if (game->frame_func)
            game->frame_func(dt);

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

        dt += elapsedSecsSince(time);
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

gameFrameFuncT gameGetFrameFunc(void) {
    return (game->frame_func);
}

void gameSetFrameFunc(gameFrameFuncT frame_func) {
    game->frame_func = frame_func;
}

keyboardT* gameGetKeyboard(void) {
    return (game->keyboard);
}

mouseT* gameGetMouse(void) {
    return (game->mouse);
}
