/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "text.h"

#include "base/common.h"
#include "base/debug.h"
#include "engine/game.h"
#include "graphics/graphics.h"
#include "graphics/shader.h"
#include "graphics/trimesh.h"

#include <stddef.h>
#include <stdint.h>

#include <GL/glew.h>

#include <windows.h>

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

shaderT* text_shader = NULL;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static void initTextShader(void) {
    text_shader = createShader();

    string *vert_src = readGamePakFile("text.vert");
    string *frag_src = readGamePakFile("text.frag");

    compileVertexShader(text_shader, vert_src);
    compileFragmentShader(text_shader, frag_src);

    free(vert_src);
    free(frag_src);
}

void drawText(const string* text, float x, float y, const string* font_name, int font_size) {
    text = wstrdup(text);

    HDC hdc = CreateCompatibleDC(0);

    string* font_face = wstrdup(font_name);
    int font_height = -MulDiv(font_size, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    HFONT hfont = CreateFontW(font_height, 0, 0, 0, FW_NORMAL, FALSE, FALSE,
                              FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                              CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
                              DEFAULT_PITCH, font_face);

    free(font_face);

    SelectObject(hdc, hfont);

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 0, 0));

    RECT rect = { 0 };
    DrawTextW(hdc, text, -1, &rect, DT_CALCRECT | DT_NOCLIP);

    int width  = rect.right  - rect.left + 2,
        height = rect.bottom - rect.top  + 2;
    rect.left++;
    rect.top++;

    void* bitmap_data = calloc(width*height, sizeof(uint32_t));
    for (int i = 3; i < width*height*4; i+= 4)
        *((uint8_t*)bitmap_data+i) = 0xff;

    HBITMAP hbitmap = CreateBitmap(width, height, 1, 32, bitmap_data);
    SelectObject(hdc, hbitmap);

    DrawTextW(hdc, text, -1, &rect, DT_TOP | DT_LEFT);
    DeleteObject(hfont);
    free(text);

    GetBitmapBits(hbitmap, width*height*4, bitmap_data);

    DeleteObject(hbitmap);
    DeleteDC(hdc);

    for (int i = 0; i < width*height*4; i += 4) {
        *((uint8_t*)bitmap_data+i+3) = *((uint8_t*)bitmap_data+i+2);
        *((uint8_t*)bitmap_data+i  ) = 0x10;
        *((uint8_t*)bitmap_data+i+1) = 0x10;
        *((uint8_t*)bitmap_data+i+2) = 0x10;
    }
    
    textureT* tex = createTexture();
    textureT* old_tex = useTexture(tex, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA,
                 GL_UNSIGNED_BYTE, bitmap_data);

    free(bitmap_data);
    
    triMeshT* text_quad = createQuad(2.0f, 2.0f);

    if (!text_shader)
        initTextShader();
    
    shaderT* old_shader = useShader(text_shader);
    
    setShaderParam("ScreenSize", &(vec2) { (float)screenWidth(), (float)screenHeight() });
    setShaderParam("TextRect"  , &(vec4) { (float)x, (float)y, (float)width, (float)height });
    
    GLint depth_mask;
    glGetIntegerv(GL_DEPTH_WRITEMASK, &depth_mask);
    
    GLboolean cull_face, depth_test;
    glGetBooleanv(GL_CULL_FACE, &cull_face);
    glGetBooleanv(GL_DEPTH_TEST, &depth_test);
    
    glDepthMask(GL_FALSE);
    glDisable  (GL_CULL_FACE);
    glDisable  (GL_DEPTH_TEST);
    
    drawMesh(text_quad);
    
    glDepthMask(depth_mask);
    
    if (cull_face)  glEnable(GL_CULL_FACE);
    if (depth_test) glEnable(GL_DEPTH_TEST);
    
    useTexture (old_tex, 0);
    useShader  (old_shader);
    freeTexture(tex);
    freeMesh   (text_quad);
}

void loadFontFromFile(const string* file_name) {
    string* s = wstrdup(file_name);
    assert(AddFontResourceExW(s, FR_PRIVATE, NULL) > 0);
    free(s);
}

void loadFontFromMemory(void* font_data, int num_bytes) {
    DWORD one = 1;
    assert(AddFontMemResourceEx(font_data, num_bytes, 0, &one));
}
