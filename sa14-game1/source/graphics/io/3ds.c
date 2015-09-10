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
#include "engine/game.h" // gameResource()
#include "graphics/material.h"
#include "graphics/trimesh.h"
#include "graphics/materials/adsmaterial.h"
#include "math/vector.h"

#include <stdint.h>
#include <string.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static void readMeshSmoothingGroups(const uint8_t* ptr, a3dsMeshDataT* mesh) {
    chunkT* chunk = (chunkT*)ptr;

    assert(chunk->id == 0x4150);
    mesh->smoothing_groups = (uint32_t*)chunk->data;
}

static void readMeshMaterial(const uint8_t* ptr, a3dsMeshDataT* mesh) {
    chunkT* chunk = (chunkT*)ptr;

    assert(chunk->id == 0x4130);

    mesh->material = (string*)chunk->data;
}

static void readTexCoords(const uint8_t* ptr, a3dsMeshDataT* mesh) {
    chunkT* chunk = (chunkT*)ptr;

    assert(chunk->id == 0x4140);

    int num_verts = *(uint16_t*)chunk->data;
    if (mesh->num_verts == (-1)) mesh->num_verts = num_verts;
    else assert(mesh->num_verts == num_verts);

    mesh->vert_uv = chunk->data + sizeof(uint16_t);
}

static void readTriIndices(const uint8_t* ptr, a3dsMeshDataT* mesh) {
    chunkT* chunk = (chunkT*)ptr;

    assert(chunk->id == 0x4120);

    mesh->num_tris = *(uint16_t*)chunk->data;
    mesh->tris     = chunk->data + sizeof(uint16_t);

    ptr = (uint8_t*)chunk->data;
    ptr += sizeof(uint16_t) + sizeof(uint16_t)*4*mesh->num_tris;

    void* chunk_end = (uint8_t*)chunk + chunk->length;
    while (ptr < chunk_end) {
        chunk = (chunkT*)ptr;

             if (chunk->id == 0x4130) readMeshMaterial       (ptr, mesh);
        else if (chunk->id == 0x4150) readMeshSmoothingGroups(ptr, mesh);

        ptr = (uint8_t*)chunk + chunk->length;
    }
}

static void readVertCoords(const uint8_t* ptr, a3dsMeshDataT* mesh) {
    chunkT* chunk = (chunkT*)ptr;

    assert(chunk->id == 0x4110);

    int num_verts = *(uint16_t*)chunk->data;
    if (mesh->num_verts == (-1)) mesh->num_verts = num_verts;
    else assert(mesh->num_verts == num_verts);

    mesh->vert_pos = chunk->data + sizeof(uint16_t);
}

static void readTrimesh(const uint8_t* ptr, a3dsObjectDataT* obj) {
    chunkT* chunk = (chunkT*)ptr;
    ptr = chunk->data;

    assert(chunk->id == 0x4100);

    assert(obj->mesh == NULL);
    obj->mesh = calloc(1, sizeof(a3dsMeshDataT));
    obj->mesh->num_verts = (-1);

    void* chunk_end = (uint8_t*)chunk + chunk->length;
    while (ptr < chunk_end) {
        chunk = (chunkT*)ptr;

             if (chunk->id == 0x4110) readVertCoords(ptr, obj->mesh);
        else if (chunk->id == 0x4120) readTriIndices(ptr, obj->mesh);
        else if (chunk->id == 0x4140) readTexCoords (ptr, obj->mesh);

        ptr = (uint8_t*)chunk + chunk->length;
    }
}

static void readMaterialTexture(const uint8_t* ptr, a3dsMaterialDataT* material) {
    chunkT* chunk = (chunkT*)ptr;
    ptr = chunk->data;

    assert(chunk->id == 0xa200);

    void* chunk_end = (uint8_t*)chunk + chunk->length;
    while (ptr < chunk_end) {
        chunk = (chunkT*)ptr;

        if (chunk->id == 0xa300)
            material->texture = (string*)chunk->data;

        ptr = (uint8_t*)chunk + chunk->length;
    }
}

static float readPercentageChunk(const uint8_t* ptr) {
    chunkT* chunk = (chunkT*)ptr;

    assert(chunk->id == 0x30);

    uint16_t percent = *(uint16_t*)chunk->data;

    return ((float)percent);
}

static vec3 readColorChunk(const uint8_t* ptr) {
    chunkT* chunk = (chunkT*)ptr;

    assert(chunk->id == 0x0011);

    uint8_t r = chunk->data[0],
            g = chunk->data[1],
            b = chunk->data[2];

    return ((vec3) { r/255.0f, g/255.0f, b/255.0f });
}

static void readMaterial(const uint8_t* ptr, a3dsDataT* a3ds) {
    chunkT* chunk = (chunkT*)ptr;
    ptr = chunk->data;

    assert(chunk->id == 0xafff);

    a3dsMaterialDataT* material = calloc(1, sizeof(a3dsMaterialDataT));
    arrayAdd(a3ds->materials, &material);

    material->ambient   = (vec3) { 0.0f, 0.0f, 0.0f };
    material->diffuse   = (vec3) { 0.5f, 0.5f, 0.5f };
    material->specular  = (vec3) { 1.0f, 1.0f, 1.0f };
    material->shininess = 10.0f;

    void* chunk_end = (uint8_t*)chunk + chunk->length;
    while (ptr < chunk_end) {
        chunk = (chunkT*)ptr;

        if (chunk->id == 0xa000)
            material->name = (string*)chunk->data;
        else if (chunk->id == 0xa010)
            material->ambient = readColorChunk((uint8_t*)chunk->data);
        else if (chunk->id == 0xa020)
            material->diffuse = readColorChunk((uint8_t*)chunk->data);
        else if (chunk->id == 0xa030)
            material->specular = readColorChunk((uint8_t*)chunk->data);
        else if (chunk->id == 0xa040)
            material->shininess = readPercentageChunk((uint8_t*)chunk->data);
        else if (chunk->id == 0xa200)
            readMaterialTexture(ptr, material);

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

        if (chunk->id == 0x3d3d) readData(ptr, a3ds);

        ptr = (uint8_t*)chunk + chunk->length;
    }
}

a3dsDataT* a3dsLoad(const uint8_t* ptr) {
    a3dsDataT* a3ds = calloc(1, sizeof(a3dsDataT));

    a3ds->materials = arrayNew(sizeof(a3dsMaterialDataT*));
    a3ds->objects  = arrayNew(sizeof(a3dsObjectDataT*));

    read3DS(ptr, a3ds);

    if (a3ds->version != 3)
        error("unsupported 3ds version");

    /*debug("materials: %d", arrayLength(a3ds->materials));
    for (int i = 0; i < arrayLength(a3ds->materials); i++) {
        a3dsMaterialDataT* material_data = *(a3dsMaterialDataT**)arrayGet(a3ds->materials, i);
        debug("  name: %s", material_data->name);
        debug("    texture:   %s", material_data->texture);
        debug("    ambient:   %4.2f %4.2f %4.2f", material_data->ambient.x, material_data->ambient.y, material_data->ambient.z);
        debug("    diffuse:   %4.2f %4.2f %4.2f", material_data->diffuse.x, material_data->diffuse.y, material_data->diffuse.z);
        debug("    specular:  %4.2f %4.2f %4.2f", material_data->specular.x, material_data->specular.y, material_data->specular.z);
        debug("    shininess: %4.2f", material_data->shininess);
    }
    
    for (int i = 0; i < arrayLength(a3ds->objects); i++) {
        a3dsObjectDataT* obj_data = *(a3dsObjectDataT**)arrayGet(a3ds->objects, i);

        debug("object: %s", obj_data->name);

        if (!obj_data->mesh)
            continue;

        //for (int j = 0; j < obj_data->mesh->num_verts; j++)
        //    warn("%4.2f %4.2f %4.2f", obj_data->mesh->vert_pos[j].x, obj_data->mesh->vert_pos[j].y, obj_data->mesh->vert_pos[j].z);

        debug("  material: %s", obj_data->mesh->material);
        debug("  num verts: %d", obj_data->mesh->num_verts);
        debug("  num tris:  %d", obj_data->mesh->num_tris);
    }*/

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

const string* a3dsGetObjectMaterialName(const a3dsDataT* a3ds,
                                        const string* object_name)
{
    for (int i = 0; i < arrayLength(a3ds->objects); i++) {
        a3dsObjectDataT** o = arrayGet(a3ds->objects, i);
        if ((*o)->mesh && strcmp(object_name, (*o)->name) == 0)
            return ((*o)->mesh->material);
    }

    return (NULL);
}

const a3dsMaterialDataT* a3dsGetMaterialData(const a3dsDataT* a3ds,
                                             const string* material_name)
{
    if (!material_name)
        return (NULL);

    for (int i = 0; i < arrayLength(a3ds->materials); i++) {
        a3dsMaterialDataT** m = arrayGet(a3ds->materials, i);
        if (strcmp(material_name, (*m)->name) == 0)
            return (*m);
    }

    return (NULL);
}

const a3dsObjectDataT* a3dsGetObjectData(const a3dsDataT* a3ds,
                                         const string* object_name)
{
    if (!object_name)
        return (NULL);

    for (int i = 0; i < arrayLength(a3ds->objects); i++) {
        a3dsObjectDataT** o = arrayGet(a3ds->objects, i);
        if (strcmp(object_name, (*o)->name) == 0)
            return (*o);
    }

    return (NULL);
}

materialT* a3dsCreateMaterial(const a3dsDataT* a3ds,
                              const string* material_name)
{
    const a3dsMaterialDataT* m = a3dsGetMaterialData(a3ds, material_name);

    if (!m)
        return (NULL);

    // @To-do: Allow client to specify texture loading function instead.

    textureT* diffuse_tex = NULL;
    if (m->texture) {
        string tex_name[1024];
        sprintf(tex_name, "texture:%s", m->texture);
        diffuse_tex = gameResource(tex_name, ResTexture);
        if (!diffuse_tex)
            warn("couldn't load texture '%s'", m->texture);
    }

    materialT* mat = createADSMaterial(
            NULL,
            diffuse_tex,
            NULL,
            m->ambient,
            m->diffuse,
            m->specular,
            m->shininess,
            false);

    // @To-do: strduping here will probably leak, so just null it for now.
    mat->name = NULL;

    return (mat);
}

triMeshT* a3dsCreateMesh(const a3dsDataT* a3ds, const string* object_name) {
    const a3dsObjectDataT* o = a3dsGetObjectData(a3ds, object_name);

    if (!o || !o->mesh)
        return (NULL);

    triMeshT* mesh  = newMesh(o->mesh->num_tris*3, o->mesh->num_tris);
    triT*     tris  = meshTrisPtr (mesh);
    vertexT*  verts = meshVertsPtr(mesh);

    // We separate the triangles so that none of them share the same vertices.
    for (int i = 0; i < o->mesh->num_tris; i++) {
        tris[i] = (triT) { i*3, i*3+1, i*3+2 };
        
        int v0 = o->mesh->tris[i].v0,
            v1 = o->mesh->tris[i].v1,
            v2 = o->mesh->tris[i].v2;

        if (o->mesh->smoothing_groups) verts->k = o->mesh->smoothing_groups[i];
        if (o->mesh->vert_uv) verts->uv = *(vec2*)&o->mesh->vert_uv[v0];
        (verts++)->p = *(vec3*)&o->mesh->vert_pos[v0];

        if (o->mesh->smoothing_groups) verts->k = o->mesh->smoothing_groups[i];
        if (o->mesh->vert_uv) verts->uv = *(vec2*)&o->mesh->vert_uv[v1];
        (verts++)->p = *(vec3*)&o->mesh->vert_pos[v1];

        if (o->mesh->smoothing_groups) verts->k = o->mesh->smoothing_groups[i];
        if (o->mesh->vert_uv) verts->uv = *(vec2*)&o->mesh->vert_uv[v2];
        (verts++)->p = *(vec3*)&o->mesh->vert_pos[v2];
    }

    calcSmoothNormals(mesh);
    updateMesh(mesh);

    return (mesh);
}
