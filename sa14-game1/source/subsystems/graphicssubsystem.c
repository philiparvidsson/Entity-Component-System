/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "graphicssubsystem.h"

#include "base/common.h"

#include "engine/game.h"
#include "engine/subsystem.h"

#include "graphics/graphics.h"
#include "graphics/shader.h"

#include "math/matrix.h"

#include <stdlib.h>

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

static shaderT* noise_shader = NULL;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static void initPostFX(void) {
    string* vert_src = readGamePakFile("shaders/discard_z.vert");
    string* frag_src = readGamePakFile("shaders/noise.frag");

    noise_shader = createShader();
    compileVertexShader  (noise_shader, vert_src);
    compileFragmentShader(noise_shader, frag_src);

    free(vert_src);
    free(frag_src);
}

static void beginFrame(gameSubsystemT* subsystem, float dt) {
    graphicsSubsystemDataT* data = subsystem->data;

    // We begin by clearing the frame buffer to some color...
    clearDisplay(1.0f, 1.0f, 1.0f);

    // ...then we activate the default shader and texture.
    useShader (data->default_shader);
    useTexture(data->default_texture, 0);

    // We also need to setup the shader by providing it with the projection and
    // view matrices.

    mat4x4 proj, view;

    // @To-do: Camera logic should be here.
    mat4x4_look_at(&(vec3) { 0.0f, 0.0f, 1.0f },
                   &(vec3) { 0.0f, 0.0f, 0.0f },
                   &(vec3) { 0.0f, 1.0f, 0.0f }, &view);

    float r = data->aspect_ratio;
    mat4x4_perspective(-0.5f*r, 0.5f*r, -0.5f, 0.5f, -1.0f, -0.01f, &proj);

    setShaderParam("Proj", &proj);
    setShaderParam("View", &view);
}

static void applyPostFX(gameSubsystemT* subsystem, float dt) {
    // Noise -------------------------------------

    static float noise_intensity = 0.07f;
    static int   noise_seed      = 0; noise_seed++;

    useShader(noise_shader);
    setShaderParam("Intensity", &noise_intensity);
    setShaderParam("Seed"     , &noise_seed);
    shaderPostProcess();

    //--------------------------------------------
}

gameSubsystemT* newGraphicsSubsystem(void) {
    gameSubsystemT* subsystem = newSubsystem("graphics");
    graphicsSubsystemDataT* data = calloc(1, sizeof(graphicsSubsystemDataT));

    data->aspect_ratio    = screenWidth() / (float)screenHeight();
    data->default_shader  = createShader();
    data->default_texture = createWhiteTexture();

    string* vert_src = readGamePakFile("shaders/default.vert");
    string* frag_src = readGamePakFile("shaders/default.frag");

    compileVertexShader  (data->default_shader, vert_src);
    compileFragmentShader(data->default_shader, frag_src);

    free(vert_src);
    free(frag_src);

    initPostFX();

    subsystem->data = data;
    subsystem->before_update_fn = beginFrame;
    subsystem->after_update_fn = applyPostFX;

    return (subsystem);
}
