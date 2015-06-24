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

gameT* game = NULL;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static void queryInputDevices() {
    getKeyboardState(&game->keyboard);
    getMouseState(&game->mouse);
}

static void updateObjects() {
    for (int i = 0; i < arrayLength(game->objects); i++) {
        gameObjectT* o = (gameObjectT*)arrayGet(game->objects, i);

        if (o->updateFunc)
            o->updateFunc(o);
    }
}

static void renderEverything() {
    clearDisplay(0.0f, 0.0f, 0.5f);

    for (int i = 0; i < arrayLength(game->objects); i++) {
        gameObjectT* o = (gameObjectT*)arrayGet(game->objects, i);

        setShaderParam("Model", &o->transform);

        if (o->model)
            drawMesh(o->model);
    }

    updateDisplay();
}

static void initGame(void) {
    initGraphics("Game Window", 1280, 720);

    loadFontFromFile("resources/fonts/sector_034.ttf");

    game = malloc(sizeof(gameT));

    game->objects = newArray(sizeof(gameObjectT));
    game->world   = worldNew();
}

static void cleanupGame(void) {
    exitGraphics();

    for (int i = 0; i < arrayLength(game->objects); i++) {
        gameObjectT* o = (gameObjectT*)arrayGet(game->objects, i);

        if (o->cleanupFunc)
            o->cleanupFunc(o);

        if (o->model) freeMesh(o->model);
        if (o->body)  bodyFree(o->model);
    }

    freeArray(game->objects);
    worldFree(game->world);

    free(game);
    game = NULL;
}

void gameMain(void) {
    initGame();

    shaderT *s = createShader();
    compileVertexShader(s, readFile("resources/shaders/test_shader.vert"));
    compileFragmentShader(s, readFile("resources/shaders/test_shader.frag"));

    useShader(s);

    mat4x4 view;
    mat4x4_look_at(&(vec3) { 0.0f, 0.5f, 2.0f },
                   &(vec3) { 0.0f, 0.0f, 0.0f },
                   &(vec3) { 0.0f, 1.0f, 0.0f }, &view);

    mat4x4 proj;
    mat4x4_perspective(-1.0f, 1.0f, -1.0f, 1.0f, -3.0f, -0.1f, &proj);

    gameObjectT *lol = arrayAdd(game->objects, &(gameObjectT) { 0 });
    lol->game = game;

    createPlayerShip(lol);
    bodySetType(lol->body, DynamicBody);
    bodyApplyForce(lol->body, (vec3) { 10.0f, 20.0f, -20.0f });

    worldAddBody(game->world, lol->body);

    mat_identity(&lol->transform);

    float dt = 0.0f;
    timeT time = getTime();
    float qq = 0.0f;
    while (windowIsOpen()) {
        dt += elapsedSecsSince(time);

        qq += elapsedSecsSince(time);
        time = getTime();

        mat_rot_y(qq, &lol->transform);

        while (dt >= TimeStep) {
            worldStep(game->world, TimeStep);
            dt -= TimeStep;
        }

        mat4x4 kewk;
        vec3 posy = bodyGetPosition(lol->body);
        mat_transl_xyz(posy.x, posy.y, posy.z, &kewk);
        mat_mul(&kewk, &lol->transform, &lol->transform);
        queryInputDevices();

        updateObjects();

        setShaderParam("Proj", &proj);
        setShaderParam("View", &view);

        renderEverything();

        // Pause if we lose focus.
        while (!windowIsFocused()) {
            sleep(10);
            updateDisplay();
        }
    }

    cleanupGame();
}
