/*------------------------------------------------------------------------------
 * File: 3ds.h
 * Created: July 23, 2015
 * Last changed: July 24, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Provides functionality for loading .3ds-file data.
 *----------------------------------------------------------------------------*/

#ifndef a3ds_h_
#define a3ds_h_

/*------------------------------------------------
 * INCLUDE
 *----------------------------------------------*/

#include "base/array.h"
#include "base/common.h"
#include "graphics/material.h"
#include "graphics/trimesh.h"
#include "math/vector.h"

#include <stdint.h>

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

typedef struct {
    int version;

    arrayT* materials;
    arrayT* objects;
} a3dsDataT;

#pragma pack(1)
typedef struct {
    uint16_t id;
    uint32_t length;
    uint8_t  data[1];
} chunkT;

typedef struct {
    string* name;
    string* texture;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float shininess;
} a3dsMaterialDataT;

typedef struct {
    int num_verts;
    struct { float x, y, z; }* vert_pos;
    struct { float u, v; }*    vert_uv;

    int num_tris;
    struct { uint16_t v0, v1, v2, flags; }* tris;
    uint32_t* smoothing_groups;

    string* material;
} a3dsMeshDataT;

typedef struct {
    string* name;

    a3dsMeshDataT* mesh;
} a3dsObjectDataT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

a3dsDataT* a3dsLoad(const uint8_t* ptr);
void a3dsFree(a3dsDataT* a3ds);

const string* a3dsGetObjectMaterialName(const a3dsDataT* a3ds,
                                        const string* object_name);

const a3dsMaterialDataT* a3dsGetMaterialData(const a3dsDataT* a3ds,
                                             const string* material_name);
const a3dsObjectDataT* a3dsGetObjectData(const a3dsDataT* a3ds,
                                         const string* object_name);

materialT* a3dsCreateMaterial(const a3dsDataT* a3ds,
                              const string* material_name);
triMeshT* a3dsCreateMesh(const a3dsDataT* a3ds,
                         const string* object_name);

#endif // a3ds_h_
