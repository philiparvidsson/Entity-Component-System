#include "rendersubsystem.h"

#include "engine/game.h"
#include "engine/subsystem.h"

#include "graphics/graphics.h"
#include "graphics/shader.h"

#include "math/matrix.h"

#include <stdlib.h>

typedef struct {
    shaderT* default_shader;
} renderSubsystemDataT;

static void beforeRender(gameSubsystemT* subsystem) {
    clearDisplay(0.0f, 0.0f, 0.4f);

    renderSubsystemDataT *data = subsystem->data;
    useShader(data->default_shader);

    mat4x4 proj, view;

    mat4x4_look_at(&(vec3) { 0.0f, 0.5f, 0.5f },
                   &(vec3) { 0.0f, 0.0f, 0.0f },
                   &(vec3) { 0.0f, 1.0f, 0.0f }, &view);

    mat4x4_perspective(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -0.01f, &proj);

    setShaderParam("Proj", &proj);
    setShaderParam("View", &view);
}

gameSubsystemT* createRenderSubsystem(void) {
    gameSubsystemT* render_subsystem = newSubsystem("render");

    renderSubsystemDataT *data = malloc(sizeof(renderSubsystemDataT));

    data->default_shader = createShader();

    compileVertexShader(data->default_shader, readFile("resources/shaders/test_shader.vert"));
    compileFragmentShader(data->default_shader, readFile("resources/shaders/test_shader.frag"));

    render_subsystem->before_update_fn = beforeRender;
    render_subsystem->data = data;

    return (render_subsystem);
}
