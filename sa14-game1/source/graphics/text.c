/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "text.h"

#include "core/common.h"
#include "core/debug.h"

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

string *font_name = "Calibri";

int font_size = 24;

shaderT* text_shader = NULL;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static void initTextShader(void) {
    text_shader = createShader();

    string *vs_src = readFile("resources/shaders/text.vert");
    string *fs_src = readFile("resources/shaders/text.frag");

    if (!vs_src || !fs_src)
        error("Could not load text shader");

    compileVertexShader(text_shader, vs_src);
    compileFragmentShader(text_shader, fs_src);

    free(vs_src);
    free(fs_src);
}

static string *strToWide(const string* str) {
    size_t len = mbstowcs(NULL, str, 0) + 1;
    wchar_t *wstr = malloc(sizeof(wchar_t)*len);
    mbstowcs(wstr, str, len);
    return wstr;
}

void setTextFont(const string* name, int size) {
    font_name = name;
    font_size = size;
}

void drawText(const string* text, float x, float y) {
    //checkGraphicsInited();

    text = strToWide(text);

    HDC hdc = CreateCompatibleDC(0);

    string *font_face = strToWide(font_name);
    int font_height = -MulDiv(font_size, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    HFONT hfont = CreateFontW(font_height, 0, 0, 0, FW_NORMAL, FALSE, FALSE,
                              FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                              CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
                              DEFAULT_PITCH, font_face);

    free(font_face);

    SelectObject(hdc, hfont);

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 0, 255));

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

    for (int i = 3; i < width*height*4; i += 4)
        *((uint8_t*)bitmap_data+i) ^= 0xff;

    GLuint text_tex_id;
    glGenTextures(1, &text_tex_id);

    glBindTexture(GL_TEXTURE_2D, text_tex_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, bitmap_data);

    free(bitmap_data);

    triMeshT* text_quad = createQuad(2.0f, 2.0f);

    if (!text_shader)
        initTextShader();

    shaderT* old_shader = useShader(text_shader);

    setShaderParam("ScreenSize", &(vec2) { (float)screenWidth(), (float)screenHeight() });
    setShaderParam("TextRect", &(vec4) { (float)x, (float)y, (float)width, (float)height });

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
    
    freeMesh(text_quad);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &text_tex_id);

    useShader(old_shader);
}

void loadFontFromFile(const string* file_name) {
    string *s = strToWide(file_name);
    assert(AddFontResourceExW(s, FR_PRIVATE, NULL) > 0);
    free(s);
}
