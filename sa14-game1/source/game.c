#include "game.h"

#include <stdio.h>
#include <stdlib.h>

gameT *game = NULL;

void initGame(void) {
    initGraphics("Game Window", 640, 640);

    game = malloc(sizeof(gameT));

    game->objects = newArray(sizeof(gameObjectT));

    game->shader_prog = createShader();
    game->text_prog = createShader();

    compileVertexShader(game->shader_prog, readFile("resources/shaders/test_shader.vert"));
    compileFragmentShader(game->shader_prog, readFile("resources/shaders/test_shader.frag"));

    compileVertexShader(game->text_prog, readFile("resources/shaders/text.vert"));
    compileFragmentShader(game->text_prog, readFile("resources/shaders/text.frag"));

    mat4x4_perspective(-1.0f, 1.0f, -1.0f, 1.0f, -0.1f, -3.0f, &game->proj);
    
    mat4x4_look_at(
        &(vec3) { 0.0f, 0.5f, 1.0f },
        &(vec3) { 0.0f, 0.0f, 0.0f },
        &(vec3) { 0.0f, 1.0f, 0.0f },
        &game->view
    );
}

void exitGame(void) {
    exitGraphics();

    free(game);
    game = NULL;
}

void createObj() {
    gameObjectT o;
    createLolCone(&o);
    arrayAdd(game->objects, &o);
}

void gameMain(void) {
    initGame();
    createObj();

    useShader(game->shader_prog);

    int lol = 0;
    float dt = 0.0f;
    while (windowIsOpen()) {
        while (dt >= TimeStep) {
            // physics shit here

            dt -= TimeStep;
        }

        // We need to step all objects forwards in time by integrating their
        // accelerations, velocities etc.
        for (int i = 0; i < arrayLength(game->objects); i++) {
            gameObjectT *o = (gameObjectT *)arrayGet(game->objects, i);

            // Not all objects need to be updated.
            if (o->update)
                o->update(o);
        }

        clearDisplay(1.0f, 1.0f, 0.5f);

        setShaderUniform("View", Matrix4Uniform, &game->view);
        setShaderUniform("Proj", Matrix4Uniform, &game->proj);

        useShader(game->shader_prog);

        // Time to render all objects.
        for (int i = 0; i < arrayLength(game->objects); i++) {
            gameObjectT *o = (gameObjectT *)arrayGet(game->objects, i);

            // All objects might not have meshes.
            if (!o->mesh)
                continue;

            setShaderUniform("Model", Matrix4Uniform, &o->transform);
            drawMesh(o->mesh);
        }

        useShader(game->text_prog);
        char buf[1024];
        sprintf(buf, "hej %d", lol++);
        drawText(buf, 24);

        updateDisplay();
    }

    exitGame();
}
