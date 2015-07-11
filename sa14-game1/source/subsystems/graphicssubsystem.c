/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "graphicssubsystem.h"

#include "engine/game.h"
#include "engine/subsystem.h"

#include "graphics/graphics.h"
#include "graphics/shader.h"

#include "math/matrix.h"

#include <stdlib.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static void beginFrame(gameSubsystemT* subsystem, float dt) {
    graphicsSubsystemDataT* data = subsystem->data;

    // We begin by clearing the frame buffer to some color...
    clearDisplay(0.0f, 0.0f, 0.4f);

    // ...then we activate the default shader.
    useShader(data->default_shader);

    // We also need to setup the shader by providing it with view and projection
    // matrices.

    mat4x4 proj, view;

    // @To-do: Camera logic should be here.
    mat4x4_look_at(&(vec3) { 0.0f, 0.0f, 0.5f },
                   &(vec3) { 0.0f, 0.0f, 0.0f },
                   &(vec3) { 0.0f, 1.0f, 0.0f }, &view);

    mat4x4_perspective(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -0.01f, &proj);

    setShaderParam("Proj", &proj);
    setShaderParam("View", &view);
}

gameSubsystemT* newGraphicsSubsystem(void) {
    gameSubsystemT* subsystem = newSubsystem("graphics");
    graphicsSubsystemDataT* data = calloc(1, sizeof(graphicsSubsystemDataT));

    data->default_shader = createShader();

    compileVertexShader  (data->default_shader, readFile("resources/shaders/test_shader.vert"));
    compileFragmentShader(data->default_shader, readFile("resources/shaders/test_shader.frag"));

    subsystem->data = data;
    subsystem->before_update_fn = beginFrame;

    return (subsystem);
}
