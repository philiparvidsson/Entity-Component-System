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

    gameObjectT cube = { 0 };
    cube.mesh = createBox(0.3f, 0.3f, 0.3f);

    shaderT *test_shader = createShader();
    compileVertexShader(test_shader, readFile("resources/shaders/test_shader.vert"));
    compileFragmentShader(test_shader, readFile("resources/shaders/test_shader.frag"));

    useShader(test_shader);

    mat4x4 view;
    mat4x4_look_at(
        &(vec3) { 0.0f, 0.5f, 0.5f },
        &(vec3) { 0.0f, 0.0f, 0.0f },
        &(vec3) { 0.0f, 1.0f, 0.9f },
        &view
    );

    mat4x4 proj;
    mat4x4_perspective(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, -1.0f, &proj);

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

        mat_rot_x(dt, &cube.transform);

        setShaderParam("Model", &cube.transform);
        setShaderParam("Proj",  &proj);
        setShaderParam("View",  &view);

        drawMesh(cube.mesh);

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
