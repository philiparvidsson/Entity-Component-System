/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "game.h"

#include "core/time.h"

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
    cube.mesh = createBox(0.5f, 0.5f, 0.5f);

    shaderT *test_shader = createShader();
    compileVertexShader(test_shader, readFile("resources/shaders/test_shader.vert"));
    compileFragmentShader(test_shader, readFile("resources/shaders/test_shader.frag"));

    shaderT *postfx_shader = createShader();
    compileVertexShader(postfx_shader, readFile("resources/shaders/postfx.vert"));
    compileFragmentShader(postfx_shader, readFile("resources/shaders/postfx.frag"));


    mat4x4 view;
    mat4x4_look_at(
        &(vec3) { 0.0f, 0.0f, 1.0f },
        &(vec3) { 0.0f, 0.0f, 0.0f },
        &(vec3) { 0.0f, 1.0f, 0.0f },
        &view
    );

    //mat_identity(&view);



    timeT time = getTime();
    int size = 0;
    while (windowIsOpen()) {
        dt += elapsedSecsSince(time);

        // Pause if we lose focus.
        while (!windowIsFocused()) {
            sleep(10);
            updateDisplay();
        }
        time = getTime();

        mat4x4 proj;
        mat4x4_perspective(-1.0f, 1.0f, -1.0f, 1.0f, -2.0f, -0.1f, &proj);
        //mat4x4_ortho      (-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, &proj);
        //mat_identity(&proj);

        clearDisplay(0.0f, 0.0f, 0.5f);

        mouseStateT ms;
        getMouseState(&ms);
        float lol = ms.x / 200.0f;
        float lol2 = ms.y / 200.0f;

        mat4x4 a, b;
        mat_rot_y(lol, &a);
        mat_rot_x(lol2, &b);

        mat_identity(&cube.transform);
        mat_mul(&a, &cube.transform, &cube.transform);
        mat_mul(&b, &cube.transform, &cube.transform);

        useShader(test_shader);

        setShaderParam("Model", &cube.transform);
        setShaderParam("Proj",  &proj);
        setShaderParam("View",  &view);

        drawMesh(cube.mesh);

        size += 1;
        if (size > 1000)
            size = 1000;

        if (dt < 10.0f) {
            int secs = 10 - (int)dt;
            char lol[100];
            sprintf(lol, "%d", secs);
            drawText(lol, (int)(size / 10.0f)+10);
        }

        shaderPostProcess(postfx_shader);

        updateDisplay();
    }

    exitGame();
}
