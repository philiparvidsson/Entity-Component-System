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
 * GLOBALS
 *----------------------------------------------*/

gameT *game = NULL;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static void queryInputDevices() {
    getKeyboardState(&game->keyboard);
    getMouseState(&game->mouse);
}

static void renderEverything() {
    clearDisplay(0.0f, 0.0f, 0.5f);

    for (int i = 0; i < arrayLength(game->objects); i++) {
        gameObjectT *o = (gameObjectT *)arrayGet(game->objects, i);

        setShaderParam("Model", &o->transform);

        if (o->mesh)
            drawMesh(o->mesh);
    }

    updateDisplay();
}

static void initGame(void) {
    initGraphics("Game Window", 1280, 720);

    loadFontFromFile("resources/fonts/sector_034.ttf");

    game = malloc(sizeof(gameT));

    game->objects = newArray(sizeof(gameObjectT));
}

static void cleanupGame(void) {
    exitGraphics();

    for (int i = 0; i < arrayLength(game->objects); i++) {
        gameObjectT *o = (gameObjectT *)arrayGet(game->objects, i);

        if (o->mesh)
            freeMesh(o->mesh);
    }

    freeArray(game->objects);

    free(game);
    game = NULL;
}

void gameMain(void) {
    initGame();

    float dt = 0.0f;
    timeT time = getTime();
    while (windowIsOpen()) {
        dt += elapsedSecsSince(time);

        queryInputDevices();

        renderEverything();

        // Pause if we lose focus.
        while (!windowIsFocused()) {
            sleep(10);
            updateDisplay();
        }
        time = getTime();
    }

    cleanupGame();
}
