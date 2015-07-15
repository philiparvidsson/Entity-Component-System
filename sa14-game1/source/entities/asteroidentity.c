#include "asteroidentity.h"

#include "base/common.h"
#include "components/graphicscomponent.h"
#include "components/physicscomponent.h"
#include "engine/entity.h"
#include "graphics/trimesh.h"

#include <stdlib.h>

static vec3 randomVector(void) {
    vec3 v;

    v.x = rand()/(float)RAND_MAX - 0.5f;
    v.y = rand()/(float)RAND_MAX - 0.5f;
    v.z = rand()/(float)RAND_MAX - 0.5f;

    vec_normalize(&v, &v);

    return (v);
}

static gameComponentT* createGraphicsComponent(void) {
    triMeshT* mesh = createBox(0.03f, 0.03f, 0.03f);
    gameComponentT* component = newGraphicsComponent(mesh);

    return (component);
}

static gameComponentT* createPhysicsComponent(void) {
    gameComponentT* component = newPhysicsComponent(1.0f * Kilogram);

    physicsComponentDataT* phys = component->data;
    
    vec3 pos = { 0.0f, 0.0f, 0.0f };
    bodySetPosition(((physicsComponentDataT*)phys)->body, pos);

    vec3 vel = randomVector();
    vel.z = 0.0f;
    vec_scale(&vel, 0.1f, &vel);
    bodySetVelocity(((physicsComponentDataT*)phys)->body, vel);

    return (component);
}

static mat4x4 do_it(vec3 u, float a) {
    mat4x4 m = { 0 };

    m.x.x = cosf(a) + u.x*u.x*(1.0f - cosf(a));
    m.x.y = u.x*u.y*(1.0f - cosf(a)) - u.z*sin(a);
    m.x.z = u.x*u.z*(1.0f - cosf(a)) + u.y*sin(a);

    m.y.x = u.y*u.x*(1.0f - cosf(a)) + u.z*sin(a);
    m.y.y = cosf(a) + u.y*u.y*(1.0f - cosf(a));
    m.y.z = u.y*u.z*(1.0f - cosf(a)) - u.x*sin(a);

    m.z.x = u.z*u.x*(1.0f - cosf(a)) - u.y*sin(a);
    m.z.y = u.z*u.y*(1.0f - cosf(a)) + u.x*sin(a);
    m.z.z = cosf(a) + u.z*u.z*(1.0f - cosf(a));

    m.w.w = 1.0f;

    return m;
}

static void rotateAsteroid(gameComponentT* component, float dt) {
    asteroidEntityDataT* asteroid = component->entity->data;

    asteroid->angle += 1.0f * dt;
    float a = asteroid->angle;

    mat4x4 m;
    mat_identity(&m);

    mat4x4 m0 = do_it(asteroid->rot_axis_1, a);
    mat4x4 m1 = do_it(asteroid->rot_axis_2, a);

    mat_mul(&m0, &m, &m);
    mat_mul(&m1, &m, &m);


    graphicsComponentDataT* gfx = getComponent(component->entity, "graphics")->data;
    gfx->transform = m;
}

gameEntityT* newAsteroidEntity(void) {
    gameEntityT* entity = newEntity();

    entity->data = calloc(1, sizeof(asteroidEntityDataT));

    gameComponentT* gfx  = createGraphicsComponent();
    gfx->update_fn = rotateAsteroid;

    gameComponentT* phys = createPhysicsComponent();

    attachComponent(entity, gfx);
    attachComponent(entity, phys);

    asteroidEntityDataT* asteroid = entity->data;

    asteroid->angle      = 0.0f;
    asteroid->scale      = 1.0f;
    asteroid->rot_axis_1 = randomVector();
    asteroid->rot_axis_2 = randomVector();

    return (entity);
}
