#include "cameraentity.h"

#include "game/game.h"

#include "graphics/shader.h"

#include "math/matrix.h"
#include "math/vector.h"

#include <stdlib.h>

typedef struct {
    mat4x4 proj;
    mat4x4 view;
    shaderT* shader;
} cameraT;

static void cameraCleanup(entityT* entity) {
    cameraT* camera = (cameraT*)entityGetData(entity);

    deleteShader(camera->shader);
    camera->shader = NULL;

    free(camera);
    entitySetData(entity, NULL);
}

static void cameraUpdate(entityT* entity) {
    cameraT* camera = (cameraT*)entityGetData(entity);

    useShader(camera->shader);
    setShaderParam("Proj", &camera->proj);
    setShaderParam("View", &camera->view);
}

entityT* createCamera() {
    entityT* entity = entityNew();
    cameraT* camera = malloc(sizeof(cameraT));

    entitySetData       (entity, camera);
    entitySetCleanupFunc(entity, cameraCleanup);
    entitySetUpdateFunc (entity, cameraUpdate);

    mat4x4_perspective(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -0.1f, &camera->proj);

    mat4x4_look_at(&(vec3) { 0.0f, 0.0f, 1.0f },
                   &(vec3) { 0.0f, 0.0f, 0.0f },
                   &(vec3) { 0.0f, 1.0f, 0.0f }, &camera->view);

    camera->shader = createShader();
    compileVertexShader(camera->shader, readFile("resources/shaders/test_shader.vert"));
    compileFragmentShader(camera->shader, readFile("resources/shaders/test_shader.frag"));

    return (entity);
}
