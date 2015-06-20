/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "game.h"

#include "core/time.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

gameT *game = NULL;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

void initGame(void) {
    initGraphics("Game Window", 640, 640);

    loadFontFromFile("resources/fonts/sector_034.ttf");

    game = malloc(sizeof(gameT));
}

void exitGame(void) {
    exitGraphics();

    free(game);
    game = NULL;
}

void gameMain(void) {
    initGame();

    float dt = 0.0f;

    timeT time = getTime();
    while (windowIsOpen()) {
        dt += elapsedSecsSince(time);

        // Pause if we lose focus.
        while (!windowIsFocused()) {
            sleep(10);
            updateDisplay();
        }

        time = getTime();

        clearDisplay(0.0f, 0.0f, 0.0f);

        if (dt < 10.0f) {
            int secs = 10 - (int)dt;
            char lol[100];
            sprintf(lol, "%d", secs);
            drawText(lol, 72);
        }

        updateDisplay();
    }

    exitGame();
}
