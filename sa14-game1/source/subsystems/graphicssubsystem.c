/*------------------------------------------------
 * DEFINES
 *----------------------------------------------*/

#define VECTOR_RGB

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
#include "graphics/material.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "math/matrix.h"
#include "math/vector.h"

#include <stdlib.h>

#include <GL/glew.h>

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

typedef struct {
    vec3 clear_color;
    float aspect_ratio;

    shaderT*  default_shader;
    textureT* default_texture;

#ifdef DRAW_TRI_NORMALS
    shaderT* normal_shader;
#endif // DRAW_TRI_NORMALS

    shaderT* noise_shader;
    float    noise_intensity;
    int      noise_seed;
} graphicsSubsystemDataT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static void loadDefaultShader(graphicsSubsystemDataT* gfx_data) {
    gfx_data->default_shader = createShader();

    string* vert_src = readGamePakFile("default.vert");
    string* frag_src = readGamePakFile("default.frag");

    compileVertexShader  (gfx_data->default_shader, vert_src);
    compileFragmentShader(gfx_data->default_shader, frag_src);
    
    free(vert_src);
    free(frag_src);
}

#ifdef DRAW_TRI_NORMALS
static void loadNormalShader(graphicsSubsystemDataT* gfx_data) {
    gfx_data->normal_shader = createShader();

    string* vert_src = readGamePakFile("normals.vert");
    string* geom_src = readGamePakFile("normals.geom");
    string* frag_src = readGamePakFile("normals.frag");

    compileVertexShader  (gfx_data->normal_shader, vert_src);
    compileGeometryShader(gfx_data->normal_shader, geom_src);
    compileFragmentShader(gfx_data->normal_shader, frag_src);
    
    free(vert_src);
    free(geom_src);
    free(frag_src);
}
#endif // DRAW_TRI_NORMALS

static void initPostFX(graphicsSubsystemDataT* gfx_data) {
    gfx_data->noise_intensity = 0.14f;
    gfx_data->noise_seed = 0;
    
    string* vert_src = readGamePakFile("discard_z.vert");
    string* frag_src = readGamePakFile("noise.frag");

    gfx_data->noise_shader = createShader();

    compileVertexShader  (gfx_data->noise_shader, vert_src);
    compileFragmentShader(gfx_data->noise_shader, frag_src);

    free(vert_src);
    free(frag_src);
}

static void applyPostFX(gameSubsystemT* subsystem) {
    graphicsSubsystemDataT* gfx_data = subsystem->data;

    // Noise -------------------------------------

    useShader(gfx_data->noise_shader);
    setShaderParam("Intensity", &gfx_data->noise_intensity);
    setShaderParam("Seed"     , &gfx_data->noise_seed);
    shaderPostProcess();

    gfx_data->noise_seed++;

    //--------------------------------------------
}

static void setupCamera(graphicsSubsystemDataT* gfx_data) {
    mat4x4 proj, view;

    // @To-do: Camera logic should be here.
    mat4x4_look_at(&(vec3) { 0.0f, 0.0f, 1.0f },
                   &(vec3) { 0.0f, 0.0f, 0.0f },
                   &(vec3) { 0.0f, 1.0f, 0.0f }, &view);

    float r = gfx_data->aspect_ratio;
    mat4x4_persp(-0.5f*r, 0.5f*r, -0.5f, 0.5f, -1.5f, -0.01f, &proj);

    setShaderParam("Proj", &proj);
    setShaderParam("View", &view);
}

static void setupLights(graphicsSubsystemDataT* gfx_data) {
    int one = 2;
    setShaderParam("NumLights", &one);

    vec3 light_pos      = (vec3) { 0.0f, 1.0f, 0.0f };
    vec3 light_ambient  = (vec3) { 0.0f, 0.0f, 0.0f };
    vec3 light_diffuse  = (vec3) { 1.0f, 1.0f, 1.0f };
    vec3 light_specular = (vec3) { 1.0f, 1.0f, 1.0f };

    setShaderParam("Lights[0].pos"     , &light_pos);
    setShaderParam("Lights[0].ambient" , &light_ambient);
    setShaderParam("Lights[0].diffuse" , &light_specular);
    setShaderParam("Lights[0].specular", &light_diffuse);

    vec_scale(&light_diffuse, 0.3f, &light_diffuse);
    light_pos.y = -1.0f;

    setShaderParam("Lights[1].pos"     , &light_pos);
    setShaderParam("Lights[1].ambient" , &light_ambient);
    setShaderParam("Lights[1].diffuse" , &light_specular);
    setShaderParam("Lights[1].specular", &light_diffuse);
}

static void drawComponent(gameComponentT* component) {
    gameComponentT* phys_c = getComponent(component->entity, "physics");
    
    // The graphics component needs a physics component to pull the position
    // from.
    assert(phys_c != NULL);
    
    graphicsComponentDataT* gfx_component  = component->data;
    physicsComponentDataT*  phys_component = phys_c->data;

    // If there's no mesh to render, we exit the function here.
    if (!gfx_component->mesh)
        return;

    mat4x4 model;
    mat_identity(&model);

    mat4x4 translation;
    vec3 pos = bodyGetPosition(phys_component->body);
    mat_transl_xyz(pos.x, pos.y, pos.z, &translation);

    mat_mul(&gfx_component->transform, &model, &model);
    mat_mul(&translation             , &model, &model);

    setShaderParam("ModelTransform" , &model);
    setShaderParam("NormalTransform", &gfx_component->normal_transform);

    materialT* material = gfx_component->material;
    setShaderParam("Material.ambient"  , &material->ambient  );
    setShaderParam("Material.diffuse"  , &material->diffuse  );
    setShaderParam("Material.specular" , &material->specular );
    setShaderParam("Material.shininess", &material->shininess);
    
    textureT* old_texture = NULL;
    if (gfx_component->texture)
        old_texture = useTexture(gfx_component->texture, 0);
    
    drawMesh(gfx_component->mesh);

    if (old_texture)
        useTexture(old_texture, 0);
}

static void drawComponents(arrayT* components) {
    for (int i = 0; i < arrayLength(components); i++) {
        gameComponentT* component = *(gameComponentT**)arrayGet(components, i);
        drawComponent(component);
    }
}

static void drawEverything(gameSubsystemT* subsystem, float dt) {
    graphicsSubsystemDataT* gfx_data = subsystem->data;

    vec3* clear_color = &gfx_data->clear_color;
    clearDisplay(clear_color->r, clear_color->g, clear_color->b);

    useShader (gfx_data->default_shader);
    useTexture(gfx_data->default_texture, 0);

    setupCamera   (gfx_data);
    setupLights   (gfx_data);
    drawComponents(subsystem->components);

#ifdef DRAW_TRI_NORMALS
    useShader (gfx_data->normal_shader);
    useTexture(gfx_data->normal_shader, 0);

    setupCamera   (gfx_data);
    drawComponents(subsystem->components);
#endif // DRAW_TRI_NORMALS

    applyPostFX(subsystem);
}

gameSubsystemT* newGraphicsSubsystem(void) {
    gameSubsystemT* subsystem = newSubsystem("graphics");
    graphicsSubsystemDataT* gfx_data = calloc(1, sizeof(graphicsSubsystemDataT));

    gfx_data->clear_color     = (vec3) { 1.0f, 1.0f, 1.0f };
    gfx_data->aspect_ratio    = screenWidth() / (float)screenHeight();
    gfx_data->default_texture = createWhiteTexture();

    loadDefaultShader(gfx_data);

#ifdef DRAW_TRI_NORMALS
    loadNormalShader(gfx_data);
#endif // DRAW_TRI_NORMALS

    subsystem->data = gfx_data;
    subsystem->after_update_fn = drawEverything;

    initPostFX(gfx_data);

    return (subsystem);
}
