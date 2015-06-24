#include "camera.h"

#include "game/game.h"

typedef struct {
    mat4x4 proj;
    mat4x4 view;
    shaderT* shader;
} cameraT;

static void cameraCleanup(gameObjectT* o) {
    if (!o->data)
        return;

    cameraT* camera = (cameraT*)o->data;

    deleteShader(camera->shader);
    camera->shader = NULL;

    free(o->data);
    o->data = NULL;
}

static void cameraUpdate(gameObjectT* o) {
    cameraT* camera = (cameraT*)o->data;

    useShader(camera->shader);
    setShaderParam("Proj", &camera->proj);
    setShaderParam("View", &camera->view);
}

gameObjectT* createCamera() {
    gameObjectT* o = calloc(1, sizeof(gameObjectT));

    o->cleanupFunc = cameraCleanup;
    o->updateFunc  = cameraUpdate;

    o->data = malloc(sizeof(cameraT));
    cameraT* camera = (cameraT*)o->data;

    mat4x4_perspective(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -0.1f, &camera->proj);

    mat4x4_look_at(&(vec3) { 0.0f, 0.0f, 1.0f },
                   &(vec3) { 0.0f, 0.0f, 0.0f },
                   &(vec3) { 0.0f, 1.0f, 0.0f }, &camera->view);

    camera->shader = createShader();
    compileVertexShader(camera->shader, readFile("resources/shaders/test_shader.vert"));
    compileFragmentShader(camera->shader, readFile("resources/shaders/test_shader.frag"));

    return (o);
}
