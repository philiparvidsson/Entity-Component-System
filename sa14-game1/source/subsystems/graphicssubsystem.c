/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "graphicssubsystem.h"

#include "base/common.h"
#include "components/graphicscomponent.h"
#include "components/physicscomponent.h"
#include "engine/game.h"
#include "engine/subsystem.h"
#include "graphics/graphics.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "math/matrix.h"

#include <stdlib.h>

#include <GL/glew.h>

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

typedef struct {
    float aspect_ratio;

    shaderT*  default_shader;
    textureT* default_texture;

#ifdef DRAW_TRI_NORMALS
    shaderT* normal_shader;
#endif // DRAW_TRI_NORMALS

    shaderT* noise_shader;
    float    noise_intensity;
    int      noise_seed;
} graphicsDataT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static void loadDefaultShader(graphicsDataT* gfx) {
    gfx->default_shader = createShader();

    string* vert_src = readGamePakFile("default.vert");
    string* frag_src = readGamePakFile("default.frag");

    compileVertexShader  (gfx->default_shader, vert_src);
    compileFragmentShader(gfx->default_shader, frag_src);
    
    free(vert_src);
    free(frag_src);
}

static void loadNormalShader(graphicsDataT* gfx) {
    gfx->normal_shader = createShader();

    string* vert_src = readGamePakFile("default.vert");
    string* geom_src = readGamePakFile("normals.geom");
    string* frag_src = readGamePakFile("normals.frag");

    compileVertexShader  (gfx->normal_shader, vert_src);
    compileGeometryShader(gfx->normal_shader, geom_src);
    compileFragmentShader(gfx->normal_shader, frag_src);
    
    free(vert_src);
    free(geom_src);
    free(frag_src);
}

static void initPostFX(graphicsDataT* gfx) {
    gfx->noise_intensity = 0.07f;
    gfx->noise_seed = 0;

    string* vert_src = readGamePakFile("discard_z.vert");
    string* frag_src = readGamePakFile("noise.frag");

    gfx->noise_shader = createShader();

    compileVertexShader  (gfx->noise_shader, vert_src);
    compileFragmentShader(gfx->noise_shader, frag_src);

    free(vert_src);
    free(frag_src);
}

static void applyPostFX(gameSubsystemT* subsystem) {
    graphicsDataT* gfx = subsystem->data;

    // Noise -------------------------------------

    useShader(gfx->noise_shader);
    setShaderParam("Intensity", &gfx->noise_intensity);
    setShaderParam("Seed", &gfx->noise_seed);
    shaderPostProcess();

    gfx->noise_seed++;

    //--------------------------------------------
}

static void setupCamera(graphicsDataT* gfx) {
    mat4x4 proj, view;

    // @To-do: Camera logic should be here.
    mat4x4_look_at(&(vec3) { 0.0f, 0.0f, 1.0f },
        &(vec3) { 0.0f, 0.0f, 0.0f },
        &(vec3) { 0.0f, 1.0f, 0.0f }, &view);

    float r = gfx->aspect_ratio;
    mat4x4_persp(-0.5f*r, 0.5f*r, -0.5f, 0.5f, -1.5f, -0.01f, &proj);

    setShaderParam("Proj", &proj);
    setShaderParam("View", &view);
}

static void drawComponent(gameComponentT* component) {
    gameComponentT* phys_c = getComponent(component->entity, "physics");

    // The graphics component needs a physics component to pull the position
    // from.
    assert(phys_c != NULL);

    graphicsComponentDataT* gfx  = component->data;
    physicsComponentDataT*  phys = phys_c->data;

    // If there's no mesh to render, we exit the function here.
    if (!gfx->mesh)
        return;

    mat4x4 model;
    mat_identity(&model);

    mat4x4 translation;
    vec3 pos = bodyGetPosition(phys->body);
    mat_transl_xyz(pos.x, pos.y, pos.z, &translation);

    mat_mul(&gfx->transform, &model, &model);
    mat_mul(&translation   , &model, &model);

    setShaderParam("ModelTransform" , &model);
    setShaderParam("NormalTransform", &gfx->normal_transform);
    
    drawMesh(gfx->mesh);
}

static void drawComponents(arrayT* components) {
    for (int i = 0; i < arrayLength(components); i++) {
        gameComponentT* component = *(gameComponentT**)arrayGet(components, i);
        drawComponent(component);
    }
}

static void drawEverything(gameSubsystemT* subsystem, float dt) {
    graphicsDataT* gfx = subsystem->data;

    clearDisplay(0.0f, 0.0f, 0.5f);

    useShader (gfx->default_shader);
    useTexture(gfx->default_texture, 0);

    setupCamera   (gfx);
    drawComponents(subsystem->components);

#ifdef DRAW_TRI_NORMALS
    useShader (gfx->normal_shader);
    useTexture(gfx->normal_shader, 0);

    setupCamera   (gfx);
    drawComponents(subsystem->components);
#endif // DRAW_TRI_NORMALS

    applyPostFX(subsystem);
}

gameSubsystemT* newGraphicsSubsystem(void) {
    gameSubsystemT* subsystem = newSubsystem("graphics");
    graphicsDataT* gfx = calloc(1, sizeof(graphicsDataT));

    gfx->aspect_ratio    = screenWidth() / (float)screenHeight();
    gfx->default_texture = createWhiteTexture();

    loadDefaultShader(gfx);

#ifdef DRAW_TRI_NORMALS
    loadNormalShader(gfx);
#endif // DRAW_TRI_NORMALS

    subsystem->data = gfx;
    subsystem->after_update_fn = drawEverything;

    initPostFX(gfx);

    return (subsystem);
}
