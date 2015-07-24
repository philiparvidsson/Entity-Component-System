/*------------------------------------------------------------------------------
 * File: 3ds.c
 * Created: July 23, 2015
 * Last changed: July 24, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Provides functionality for loading .3ds-file data.
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDE
 *----------------------------------------------*/

#include "3ds.h"

#include "base/common.h"
#include "graphics/material.h"
#include "graphics/trimesh.h"
#include "graphics/materials/adsmaterial.h"
#include "math/vector.h"

#include <stdint.h>
#include <string.h>


/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static void readMeshMaterial(const uint8_t* ptr, a3dsMeshDataT* mesh_data) {
    chunkT* chunk = (chunkT*)ptr;

    assert(chunk->id == 0x4130);

    mesh_data->material = (string*)chunk->data;
}

static void readTexCoords(const uint8_t* ptr, a3dsMeshDataT* mesh_data) {
    chunkT* chunk = (chunkT*)ptr;

    assert(chunk->id == 0x4140);

    int num_verts = *(short*)chunk->data;
    if (mesh_data->num_verts == (-1)) mesh_data->num_verts = num_verts;
    else assert(mesh_data->num_verts == num_verts);

    mesh_data->vert_uv = chunk->data + sizeof(short);
}

static void readTriIndices(const uint8_t* ptr, a3dsMeshDataT* mesh_data) {
    chunkT* chunk = (chunkT*)ptr;

    assert(chunk->id == 0x4120);

    mesh_data->num_tris = *(short*)chunk->data;
    mesh_data->tris = chunk->data + sizeof(short);

    ptr = (uint8_t*)chunk->data;
    ptr += sizeof(short) + sizeof(short)*4*mesh_data->num_tris;

    void* chunk_end = (uint8_t*)chunk + chunk->length;
    while (ptr < chunk_end) {
        chunk = (chunkT*)ptr;

        if (chunk->id == 0x4130) readMeshMaterial(ptr, mesh_data);

        ptr = (uint8_t*)chunk + chunk->length;
    }
}

static void readVertCoords(const uint8_t* ptr, a3dsMeshDataT* mesh_data) {
    chunkT* chunk = (chunkT*)ptr;

    assert(chunk->id == 0x4110);

    int num_verts = *(short*)chunk->data;
    if (mesh_data->num_verts == (-1)) mesh_data->num_verts = num_verts;
    else assert(mesh_data->num_verts == num_verts);

    mesh_data->vert_pos = chunk->data + sizeof(short);
}

static void readTrimesh(const uint8_t* ptr, a3dsObjectDataT* obj_data) {
    chunkT* chunk = (chunkT*)ptr;
    ptr = chunk->data;

    assert(chunk->id == 0x4100);

    obj_data->mesh = calloc(1, sizeof(a3dsMeshDataT));
    obj_data->mesh->num_verts = (-1);

    void* chunk_end = (uint8_t*)chunk + chunk->length;
    while (ptr < chunk_end) {
        chunk = (chunkT*)ptr;

             if (chunk->id == 0x4110) readVertCoords(ptr, obj_data->mesh);
        else if (chunk->id == 0x4120) readTriIndices(ptr, obj_data->mesh);
        else if (chunk->id == 0x4140) readTexCoords (ptr, obj_data->mesh);

        ptr = (uint8_t*)chunk + chunk->length;
    }
}

static void readMaterialTexture(const uint8_t* ptr, a3dsMaterialDataT* material_data) {
    chunkT* chunk = (chunkT*)ptr;
    ptr = chunk->data;

    assert(chunk->id == 0xa200);

    chunk = (chunkT*)ptr;

    void* chunk_end = (uint8_t*)chunk + chunk->length;
    while (ptr < chunk_end) {
        chunk = (chunkT*)ptr;

        if (chunk->id == 0xa300)
            material_data->texture = chunk->data;

        ptr = (uint8_t*)chunk + chunk->length;
    }
}

static void readMaterialSpecular(const uint8_t* ptr, a3dsMaterialDataT* material_data) {
    chunkT* chunk = (chunkT*)ptr;
    ptr = chunk->data;

    assert(chunk->id == 0xa030);

    chunk = (chunkT*)ptr;

    assert(chunk->id == 0x0011);

    uint8_t r = chunk->data[0];
    uint8_t g = chunk->data[1];
    uint8_t b = chunk->data[2];

    material_data->specular = (vec3) { r/255.0f, g/255.0f, b/255.0f };
}

static void readMaterialDiffuse(const uint8_t* ptr, a3dsMaterialDataT* material_data) {
    chunkT* chunk = (chunkT*)ptr;
    ptr = chunk->data;

    assert(chunk->id == 0xa020);

    chunk = (chunkT*)ptr;

    assert(chunk->id == 0x0011);

    uint8_t r = chunk->data[0];
    uint8_t g = chunk->data[1];
    uint8_t b = chunk->data[2];

    material_data->diffuse = (vec3) { r/255.0f, g/255.0f, b/255.0f };
}

static void readMaterialAmbient(const uint8_t* ptr, a3dsMaterialDataT* material_data) {
    chunkT* chunk = (chunkT*)ptr;
    ptr = chunk->data;

    assert(chunk->id == 0xa010);

    chunk = (chunkT*)ptr;

    assert(chunk->id == 0x0011);

    uint8_t r = chunk->data[0];
    uint8_t g = chunk->data[1];
    uint8_t b = chunk->data[2];

    material_data->ambient = (vec3) { r/255.0f, g/255.0f, b/255.0f };
}

static void readMaterialName(const uint8_t* ptr, a3dsMaterialDataT* material_data) {
    chunkT* chunk = (chunkT*)ptr;
    ptr = chunk->data;

    assert(chunk->id == 0xa000);

    material_data->name = (string*)chunk->data;
}

static void readMaterial(const uint8_t* ptr, a3dsDataT* a3ds) {
    chunkT* chunk = (chunkT*)ptr;
    ptr = chunk->data;

    assert(chunk->id == 0xafff);

    a3dsMaterialDataT* material_data = calloc(1, sizeof(a3dsMaterialDataT));
    arrayAdd(a3ds->materials, &material_data);

    material_data->ambient   = (vec3) { 0.0f, 0.0f, 0.0f };
    material_data->diffuse   = (vec3) { 0.5f, 0.5f, 0.5f };
    material_data->specular  = (vec3) { 1.0f, 1.0f, 1.0f };
    material_data->shininess = 10.0f;

    void* chunk_end = (uint8_t*)chunk + chunk->length;
    while (ptr < chunk_end) {
        chunk = (chunkT*)ptr;

             if (chunk->id == 0xa000) readMaterialName    (ptr, material_data);
        else if (chunk->id == 0xa010) readMaterialAmbient (ptr, material_data);
        else if (chunk->id == 0xa020) readMaterialDiffuse (ptr, material_data);
        else if (chunk->id == 0xa030) readMaterialSpecular(ptr, material_data);
        else if (chunk->id == 0xa200) readMaterialTexture (ptr, material_data);

        ptr = (uint8_t*)chunk + chunk->length;
    }
}

static void readObject(const uint8_t* ptr, a3dsDataT* a3ds) {
    chunkT* chunk = (chunkT*)ptr;
    ptr = chunk->data;

    assert(chunk->id == 0x4000);

    a3dsObjectDataT* obj_data = calloc(1, sizeof(a3dsObjectDataT));
    arrayAdd(a3ds->objects, &obj_data);

    obj_data->name = (string*)ptr;

    ptr += strlen(obj_data->name)+1;

    void* chunk_end = (uint8_t*)chunk + chunk->length;
    while (ptr < chunk_end) {
        chunk = (chunkT*)ptr;

        if (chunk->id == 0x4100) readTrimesh(ptr, obj_data);

        ptr = (uint8_t*)chunk + chunk->length;
    }
}

static void readData(const uint8_t* ptr, a3dsDataT* a3ds) {
    chunkT* chunk = (chunkT*)ptr;
    ptr = chunk->data;

    assert(chunk->id == 0x3d3d);

    void* chunk_end = (uint8_t*)chunk + chunk->length;
    while (ptr < chunk_end) {
        chunk = (chunkT*)ptr;

                  if (chunk->id == 0x4000) readObject (ptr, a3ds);
             else if (chunk->id == 0xafff) readMaterial(ptr, a3ds);

        ptr = (uint8_t*)chunk + chunk->length;
    }
}

static void read3DS(const uint8_t* ptr, a3dsDataT* a3ds) {
    chunkT* chunk = (chunkT*)ptr;

    // The main chunk data block is 10 bytes, so we skip past it.
    ptr = chunk->data + 10;

    if (chunk->id != 0x4d4d)
        error("invalid 3ds data");

    a3ds->version = chunk->data[6];

    void* chunk_end = (uint8_t*)chunk + chunk->length;
    while (ptr < chunk_end) {
        chunk = (chunkT*)ptr;

        if (chunk->id = 0x3d3d) readData(ptr, a3ds);

        ptr = (uint8_t*)chunk + chunk->length;
    }
}

a3dsDataT* a3dsLoad(const uint8_t* ptr) {
    a3dsDataT* a3ds = malloc(sizeof(a3dsDataT));

    a3ds->materials = arrayNew(sizeof(a3dsMaterialDataT*));
    a3ds->objects = arrayNew(sizeof(a3dsObjectDataT*));

    read3DS(ptr, a3ds);

    if (a3ds->version != 3)
        error("unsupported 3ds version");

    trace("loaded 3ds");

    trace("  materials: %d", arrayLength(a3ds->materials));
    for (int i = 0; i < arrayLength(a3ds->materials); i++) {
        a3dsMaterialDataT* material_data = *(a3dsMaterialDataT**)arrayGet(a3ds->materials, i);
        trace("    name: %s", material_data->name);
        trace("      texture:  %s", material_data->texture);
        trace("      ambient:  %4.2f %4.2f %4.2f", material_data->ambient.x, material_data->ambient.y, material_data->ambient.z);
        trace("      diffuse:  %4.2f %4.2f %4.2f", material_data->diffuse.x, material_data->diffuse.y, material_data->diffuse.z);
        trace("      specular: %4.2f %4.2f %4.2f", material_data->specular.x, material_data->specular.y, material_data->specular.z);
    }
    
    for (int i = 0; i < arrayLength(a3ds->objects); i++) {
        a3dsObjectDataT* obj_data = *(a3dsObjectDataT**)arrayGet(a3ds->objects, i);

        trace("  object: %s", obj_data->name);

        if (!obj_data->mesh)
            continue;

        trace("    material: %s", obj_data->mesh->material);
        trace("    num verts: %d", obj_data->mesh->num_verts);
        trace("    num tris:  %d", obj_data->mesh->num_tris);
    }

    return (a3ds);
}

void a3dsFree(a3dsDataT* a3ds) {
    for (int i = 0; i < arrayLength(a3ds->materials); i++) {
        a3dsMaterialDataT* mat = *(a3dsMaterialDataT**)arrayGet(a3ds->materials, i);
        free(mat);
    }

    arrayFree(a3ds->materials);

    for (int i = 0; i < arrayLength(a3ds->objects); i++) {
        a3dsObjectDataT* obj = *(a3dsObjectDataT**)arrayGet(a3ds->objects, i);

        if (obj->mesh)
            free(obj->mesh);

        free(obj);
    }

    arrayFree(a3ds->objects);
    free(a3ds);
}

const string* a3dsMaterialName(const a3dsDataT* a3ds, const string* obj_name) {
    for (int i = 0; i < arrayLength(a3ds->objects); i++) {
        a3dsObjectDataT* obj = *(a3dsObjectDataT**)arrayGet(a3ds->objects, i);

        if (obj->mesh && strcmp(obj_name, obj->name)==0)
            return (obj->mesh->material);
    }
}

const a3dsMaterialDataT* a3dsMaterial(const a3dsDataT* a3ds,
                                      const string* name)
{
    for (int i = 0; i < arrayLength(a3ds->materials); i++) {
        a3dsMaterialDataT* mat = *(a3dsMaterialDataT**)arrayGet(a3ds->materials, i);

        if (strcmp(name, mat->name) == 0)
            return (mat);
    }

    return (NULL);
}

const a3dsObjectDataT* a3dsObject(const a3dsDataT* a3ds, const string* name) {
    for (int i = 0; i < arrayLength(a3ds->objects); i++) {
        a3dsObjectDataT* obj = *(a3dsObjectDataT**)arrayGet(a3ds->objects, i);

        if (strcmp(name, obj->name)==0)
            return (obj);
    }

    return (NULL);
}

materialT* a3dsCreateMaterial(const a3dsDataT* a3ds, const string* mat_name) {
    a3dsMaterialDataT* m = a3dsMaterial(a3ds, mat_name);

    materialT* mat = createADSMaterial(
            NULL, NULL, NULL,
            (vec3) {0.0f, 0.0f, 0.0f },
            m->diffuse,
            m->specular,
            m->shininess);

    return (mat);
}

triMeshT* a3dsCreateMesh(const a3dsDataT* a3ds, const string* obj_name) {
    a3dsObjectDataT* obj = a3dsObject(a3ds, obj_name);

    if (!obj || !obj->mesh)
        return (NULL);

    triMeshT* mesh = newMesh(obj->mesh->num_tris*3, obj->mesh->num_tris);

    triT*    tris  = meshTrisPtr(mesh);
    vertexT* verts = meshVertsPtr(mesh);

    // We separate the triangles so that none of them share the same vertices.
    /*for (int i = 0; i < obj->mesh->num_tris; i++) {
        tris[i] = (triT) { i*3, i*3+1, i*3+2 };
        
        float* pos;
        float* uv;

        int v0 = obj->mesh->tris[i].v0,
            v1 = obj->mesh->tris[i].v1,
            v2 = obj->mesh->tris[i].v2;

        pos = (float*)&obj->mesh->vert_pos[v0];
        if (obj->mesh->vert_uv) {
            float* uv = (float*)&obj->mesh->vert_uv[v0];
            verts->uv = *(vec2*)uv;
        }
        (verts++)->p = *(vec3*)pos;

        pos = (float*)&obj->mesh->vert_pos[v1];
        if (obj->mesh->vert_uv) {
            float* uv = (float*)&obj->mesh->vert_uv[v1];
            verts->uv = *(vec2*)uv;
        }
        (verts++)->p = *(vec3*)pos;

        pos = (float*)&obj->mesh->vert_pos[v2];
        if (obj->mesh->vert_uv) {
            float* uv = (float*)&obj->mesh->vert_uv[v2];
            verts->uv = *(vec2*)uv;
        }
        (verts++)->p = *(vec3*)pos;
    }*/

    for (int i = 0; i < obj->mesh->num_tris; i++) {
        tris[i] = (triT) { obj->mesh->tris[i].v0,
                           obj->mesh->tris[i].v1,
                           obj->mesh->tris[i].v2 };
    }

    for (int i = 0; i < obj->mesh->num_verts; i++)
        verts[i].p = *(vec3*)&obj->mesh->vert_pos[i];

    calcSmoothNormals(mesh);
    updateMesh(mesh);

    return (mesh);
}
