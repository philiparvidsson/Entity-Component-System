#include "game.h"

#include "core/common.h"
#include "core/file_io.h"

#include "graphics/meshgen.h"

#include "math/matrix.h"
#include "math/vector.h"

#include "graphics.h"

#include <stdlib.h>

typedef struct {
    triMeshT *box1;

    mat4x4 view, proj;

    shaderProgramADT shader_prog;
} gameT;

gameT *game = NULL;


void initGame(void) {
    initGraphics("Game Window", 640, 640);
    setFrameRate(60.0f);

    game = malloc(sizeof(gameT));

    game->shader_prog = createShaderProgram();

    compileVertexShader(game->shader_prog, readFile("shaders/test_shader.vert"));
    compileFragmentShader(game->shader_prog, readFile("shaders/test_shader.frag"));

    game->box1 = createCone(0.1f, 0.2f, 8);

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


void gameMain(void) {
    initGame();

    while (windowIsOpen()) {
        mat4x4 rot_x, rot_y;

        mat_rot_x((mouseY() - 240) / 200.0f, &rot_x);
        mat_rot_y((mouseX() - 320) / 300.0f, &rot_y);

        mat4x4 t;

        mat_identity(&t);
        mat_mul(&rot_x, &t, &t);
        mat_mul(&rot_y, &t, &t);

        clearDisplay(0.0f, 0.0f, 0.2f);

        useShaderProgram(game->shader_prog);
        setShaderUniform("View", Matrix4Uniform, &game->view);
        setShaderUniform("Proj", Matrix4Uniform, &game->proj);

        setShaderUniform("Model", Matrix4Uniform, &t);
        drawMesh(game->box1);


        updateDisplay();
    }

    exitGame();
}
