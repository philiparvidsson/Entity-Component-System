#include "asteroidentity.h"

#include "base/common.h"
#include "components/graphicscomponent.h"
#include "components/physicscomponent.h"
#include "engine/entity.h"
#include "graphics/material.h"
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
    static triMeshT* mesh;
    
    if (!mesh) {
        mesh = createGeodesicSphere(0.02f, 0);
    }


    gameComponentT* component = newGraphicsComponent(mesh);
    graphicsComponentDataT* gfx_data = component->data;

    gfx_data->material = getNamedMaterial("blue crystal");

    if ((rand() % 2) == 1) {
        gfx_data->material = getNamedMaterial("shiny black");
    }

    return (component);
}

static gameComponentT* createPhysicsComponent(void) {
    gameComponentT* component = newPhysicsComponent(1.0f * Kilogram);

    physicsComponentDataT* phys = component->data;
    
    vec3 pos;
    vec3 vel;

    switch (rand() % 4) {
        
    case 0: {
        // Spawn above screen.
        pos = (vec3) { -0.5f + 1.0f * rand()/(float)RAND_MAX, 0.5f, 0.0f };
        vel = (vec3) { -1.0f + 2.0f * rand()/(float)RAND_MAX,
                       -0.2f - 1.0f * rand()/(float)RAND_MAX,
                        0.0f };
        break;
    };

    case 1: {
        // Spawn to the right of screen.
        pos = (vec3) {  0.9f, -0.5f + 1.0f * rand()/(float)RAND_MAX, 0.0f };
        vel = (vec3) { -0.2f - 1.0f * rand() / (float)RAND_MAX,
                       -1.0f + 2.0f * rand() / (float)RAND_MAX,
                        0.0f
        };
        break;
    };

    case 2: {
        // Spawn below screen.
        pos = (vec3) { -0.5f + 1.0f * rand()/(float)RAND_MAX, -0.5f, 0.0f };
        vel = (vec3) { -1.0f + 2.0f * rand()/(float)RAND_MAX,
                        0.2f + 1.0f * rand()/(float)RAND_MAX,
                        0.0f };
        break;
    };

    case 3: {
        // Spawn to the left of screen.
        pos = (vec3) { -0.9f, -0.5f + 1.0f * rand()/(float)RAND_MAX, 0.0f };
        vel = (vec3) {  0.2f + 1.0f * rand() / (float)RAND_MAX,
                       -1.0f + 2.0f * rand() / (float)RAND_MAX,
                        0.0f
        };
        break;
    };

    }

    //vec_scale(&pos, 0.1f, &pos);
    bodySetPosition(((physicsComponentDataT*)phys)->body, pos);

    vel.z = 0.0f;
    vec_scale(&vel, 0.1f, &vel);
    bodySetVelocity(((physicsComponentDataT*)phys)->body, vel);
    
    return (component);
}

static void rotateAsteroid(gameComponentT* component, float dt) {
    asteroidEntityDataT* asteroid = component->entity->data;

    asteroid->angle1 += asteroid->a1 * dt;
    asteroid->angle2 += asteroid->a2 * dt;

    mat4x4 m;
    mat_identity(&m);

    mat4x4 m0;
    mat4x4 m1;

    mat_rot(&asteroid->rot_axis_1, asteroid->angle1, &m0);
    mat_rot(&asteroid->rot_axis_2, asteroid->angle2, &m1);

    mat_mul(&m0, &m, &m);
    mat_mul(&m1, &m, &m);

    graphicsComponentDataT* gfx = getComponent(component->entity, "graphics")->data;
    
    gfx->normal_transform = m;

    mat4x4 lole;
    mat_scale(asteroid->scale, &lole);
    lole.w.w = 1.0f;
    mat_mul(&lole, &m, &m);

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

    asteroid->a1 = (rand() / (float)RAND_MAX - 0.5f) * 6.0f;
    asteroid->a2 = (rand() / (float)RAND_MAX - 0.5f) * 6.0f;
    asteroid->angle1 = 0.0f;
    asteroid->angle2 = 0.0f;
    asteroid->scale = (0.5f + rand() / (float)RAND_MAX);
    asteroid->rot_axis_1 = randomVector();
    asteroid->rot_axis_2 = randomVector();

    return (entity);
}
