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
#include "graphics/trimesh.h"

#include <stdint.h>
#include <string.h>

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

#pragma pack(1)
typedef struct {
    uint16_t id;
    uint32_t length;
    uint8_t  data[1];
} chunkT;

typedef struct {
    int version;

    int num_verts;
    struct { float x, y, z; }* verts;

    int num_tris;
    struct { short v0, v1, v2, flags; }* tris;
} a3dsDataT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static void readFaceListChunk(const uint8_t* ptr, a3dsDataT* model) {
    chunkT* chunk = (chunkT*)ptr;

    assert(chunk->id == 0x4120);

    model->num_tris = *(short*)chunk->data;
    model->tris     = chunk->data + sizeof(short);
}

static void readVertexListChunk(const uint8_t* ptr, a3dsDataT* model) {
    chunkT* chunk = (chunkT*)ptr;

    assert(chunk->id == 0x4110);

    model->num_verts = *(short*)chunk->data;
    model->verts     = chunk->data + sizeof(short);
}

static void readTrimeshChunk(const uint8_t* ptr, a3dsDataT* data) {
    chunkT* chunk = (chunkT*)ptr;
    ptr = chunk->data;

    assert(chunk->id == 0x4100);

    void* chunk_end = (uint8_t*)chunk + chunk->length;
    while (ptr < chunk_end) {
        chunk = (chunkT*)ptr;

             if (chunk->id == 0x4110) readVertexListChunk(ptr, data);
        else if (chunk->id == 0x4120) readFaceListChunk  (ptr, data);

        ptr = (uint8_t*)chunk + chunk->length;
    }
}

static void readObjChunk(const uint8_t* ptr, a3dsDataT* data) {
    chunkT* chunk = (chunkT*)ptr;
    ptr = chunk->data;

    assert(chunk->id == 0x4000);

    char* obj_name = ptr;
    ptr += strlen(obj_name) + 1;

    void* chunk_end = (uint8_t*)chunk + chunk->length;
    while (ptr < chunk_end) {
        chunk = (chunkT*)ptr;

        if (chunk->id == 0x4100) readTrimeshChunk(ptr, data);

        ptr = (uint8_t*)chunk + chunk->length;
    }
}

static void readMeshChunk(const uint8_t* ptr, a3dsDataT* data) {
    chunkT* chunk = (chunkT*)ptr;
    ptr = chunk->data;

    assert(chunk->id == 0x3d3d);

    void* chunk_end = (uint8_t*)chunk + chunk->length;
    while (ptr < chunk_end) {
        chunk = (chunkT*)ptr;

        if (chunk->id == 0x4000) readObjChunk(ptr, data);

        ptr = (uint8_t*)chunk + chunk->length;
    }
}

static void readMainChunk(const uint8_t* ptr, a3dsDataT* data) {
    chunkT* chunk = (chunkT*)ptr;

    // The main chunk data block is 10 bytes, so we skip past it.
    ptr = chunk->data + 10;

    if (chunk->id != 0x4d4d)
        error("invalid 3ds data");

    data->version = chunk->data[6];

    void* chunk_end = (uint8_t*)chunk + chunk->length;
    while (ptr < chunk_end) {
        chunk = (chunkT*)ptr;

        if (chunk->id = 0x3d3d) readMeshChunk(ptr, data);

        ptr = (uint8_t*)chunk + chunk->length;
    }
}

triMeshT* load3DS(const uint8_t* ptr) {
    a3dsDataT data;

    readMainChunk(ptr, &data);

    triMeshT* mesh = newMesh(data.num_verts, data.num_tris);

    vertexT* v = meshVertsPtr(mesh);
    for (int i = 0; i < data.num_verts; i++) {
        v[i].x = data.verts[i].x;
        v[i].y = data.verts[i].y;
        v[i].z = data.verts[i].z;
    }

    triT* t = meshTrisPtr(mesh);
    for (int i = 0; i < data.num_tris; i++) {
        t[i].v0 = data.tris[i].v0;
        t[i].v1 = data.tris[i].v1;
        t[i].v2 = data.tris[i].v2;
    }

    calcSmoothNormals(mesh);
    updateMesh(mesh);

    return (mesh);
}
