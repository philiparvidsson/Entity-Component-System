/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "game.h"

#include "core/time.h"

#include "input/keyboard.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "gfx/text.h"

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static void queryInputDevices(gameT* game) {
    getKeyboardState(&game->keyboard);
    getMouseState(&game->mouse);
}

static void updateObjects(gameT* game) {
    gameObjectT* o = game->object_list;
    while (o) {
        if (o->updateFunc)
            o->updateFunc(o);

        o = o->next;
    }
}

static void drawAll(gameT* game) {
    clearDisplay(0.0f, 0.0f, 0.6f);

    gameObjectT* o = game->object_list;
    while (o) {
        if (o->drawFunc)
            o->drawFunc(o);

        o = o->next;
    }

    updateDisplay();
}

gameT* createGame(void) {
    initGraphics("Game Window", 720, 720);

    gameT* game = calloc(1, sizeof(gameT));

    game->world = worldNew();

    return (game);
}

void exitGame(gameT* game) {
    exitGraphics();

    gameObjectT* o = game->object_list;
    while (o) {
        if (o->cleanupFunc)
            o->cleanupFunc(o);

        gameObjectT* old_o = o;
        o = o->next;
        free(old_o);
    }

    worldFree(game->world);

    free(game);
    game = NULL;
}

void gameMain(gameT* game, void (*frameFunc(gameT*, float))) {
    float dt = 0.0f;
    timeT time = getTime();
    while (windowIsOpen()) {
        dt += elapsedSecsSince(time);
        time = getTime();

        if (frameFunc)
            frameFunc(game, dt);

        queryInputDevices(game);

        updateObjects(game);

        while (dt >= TimeStep) {
            worldStep(game->world, TimeStep);
            dt -= TimeStep;
        }

        drawAll(game);

        // Pause if we lose focus.
        while (!windowIsFocused()) {
            sleep(10);
            updateDisplay();
        }
    }
}

void gameAddObject(gameT* game, gameObjectT* o) {
    assert(o->game == NULL);

    o->game = game;

    if (game->object_list)
        game->object_list->prev = o;

    o->next = game->object_list;
    game->object_list = o;
}