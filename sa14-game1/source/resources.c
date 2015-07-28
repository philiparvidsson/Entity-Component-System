#include "resources.h"

#include "base/common.h"
#include "base/fileio.h"
#include "base/pak.h"
#include "engine/game.h"
#include "graphics/io/3ds.h"
#include "graphics/shader.h"
#include "graphics/text.h"

#include <string.h>

// @To-do: This file should ultimately be a run-time loaded script, not a
//         hardcoded set of instructions.

static void compileShader(const string* name, const string* vs, const string* gs, const string* fs) {
    shaderT* shader = createShader();

    if (vs) compileVertexShader  (shader, gameResource(vs, ResString));
    if (gs) compileGeometryShader(shader, gameResource(gs, ResString));
    if (fs) compileFragmentShader(shader, gameResource(fs, ResString));

    string buf[256];
    sprintf(buf, "shader:%s", name);
    gameAddResource(buf, shader, ResShader);
    trace("  compiled shader: %s", name);
}

static void compileShaders(void) {
    trace("compiling shaders...");

    //--------------------------------------------
    // Post effects
    //--------------------------------------------

    compileShader("mblur0",
                      "shaders/default.vert",
                      NULL,
                      "shaders/postfx/motionblur0.frag");

    compileShader("mblur1",
                      "shaders/discard_z.vert",
                      NULL,
                      "shaders/postfx/motionblur1.frag");

    compileShader("noise",
                      "shaders/discard_z.vert",
                      NULL,
                      "shaders/postfx/noise.frag");

    //--------------------------------------------
    // Debug
    //--------------------------------------------

    compileShader("normals",
                      "shaders/normals.vert",
                      "shaders/normals.geom",
                      "shaders/normals.frag");

    //--------------------------------------------
    // Special
    //--------------------------------------------

    compileShader("splashscreen",
                      "shaders/discard_z.vert",
                      NULL,
                      "shaders/splashscreen.frag");

    compileShader("text",
                      "shaders/text.vert",
                      NULL,
                      "shaders/text.frag");

    //--------------------------------------------
    // Materials
    //--------------------------------------------

    compileShader("adsmaterial",
                      "shaders/default.vert",
                      NULL,
                      "shaders/materials/adsmaterial.frag");

    compileShader("refractmaterial",
                      "shaders/default.vert",
                      NULL,
                      "shaders/materials/refractmaterial.frag");
}

static void loadTexture(const string* name, const string* res_name) {
    textureT* tex = loadTextureFromMemory(gameResource(res_name, ResBinary));
    
    // For proper tiling.
    setTextureRepeat(tex, true);

    string buf[256];
    sprintf(buf, "texture:%s", name);
    gameAddResource(buf, tex, ResTexture);
    trace("  loaded texture: %s", name);
}

static void loadTextures(void) {
    trace("loading textures...");

    loadTexture("splashscreen0",
                    "textures/splashscreen0.bmp");

    loadTexture("CHECKER.BMP",
                    "textures/CHECKER.BMP");
}

static void loadMesh(const string* name, const string* res_name) {
    a3dsDataT* a3ds = a3dsLoad(gameResource(res_name, ResBinary));
    string buf[256];
    sprintf(buf, "mesh:%s", name);
    gameAddResource(buf, a3ds, ResMesh);
    trace("  loaded mesh: %s", name);
}

static void loadMeshes(void) {
    trace("loading meshes...");

    loadMesh("monkey",
                 "meshes/monkey.3ds");

    loadMesh("player",
                 "meshes/player.3ds");
}

static void compileResources(void) {
    trace("");

    compileShaders();
    trace("");

    loadTextures();
    trace("");

    loadMeshes();

    trace("");
}

void loadResources(void) {
    pakArchiveT* pak = pakOpenArchive("data.pak", PakPassword);

    if (!pak)
        error("couldn't load resources");

    trace("loading resources...");

    for (int i = 0; i < pakNumFiles(pak); i++) {
        const string* file_name = pakGetFilename(pak, i);
        const string* file_ext  = ioFileExt(file_name);

        void* res_data = NULL;
        int   res_type = -1;

        if ((strcmp(file_ext, ".frag") == 0)
         || (strcmp(file_ext, ".geom") == 0)
         || (strcmp(file_ext, ".vert") == 0))
        {
            res_data = pakReadFile(pak, file_name);
            res_type = ResString;
        }
        else {
            res_data = pakReadFile(pak, file_name);
            res_type = ResBinary;
        }

        if (!res_data)
            error("failed to load %s", file_name);

        gameAddResource(file_name, res_data, res_type);

        trace("  loaded %s", file_name);
    }

    pakFileT* pf = pakOpenFile(pak, "fonts/sector_034.ttf");
    assert(pf != NULL);

    int num_bytes = pakFileSize(pf);
    uint8_t* font_data = malloc(sizeof(uint8_t) * num_bytes);
    assert(pakRead(pf, font_data, num_bytes) == num_bytes);
    pakCloseFile(pf);
    loadFontFromMemory(font_data, num_bytes);
    free(font_data);
    trace("\nloading fonts...\n  loaded font: Sector 034");

    pakCloseArchive(pak);

    compileResources();
}
