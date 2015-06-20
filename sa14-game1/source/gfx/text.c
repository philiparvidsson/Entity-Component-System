/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "text.h"

#include "core/common.h"
#include "core/debug.h"

#include "gfx/trimesh.h"

#define VECTOR_RGB
#include "math/vector.h"

#include <stddef.h>
#include <stdint.h>

#include <GL/glew.h>

#include <windows.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static string *strToWide(string const *str) {
    size_t len = mbstowcs(NULL, str, 0) + 1;
    wchar_t *wstr = malloc(sizeof(wchar_t)*len);
    mbstowcs(wstr, str, len);
    return wstr;
}

void drawText(string const *text, int point_size) {
    //checkGraphicsInited();

    text = strToWide(text);

    HDC hdc = CreateCompatibleDC(0);

    int font_height = -MulDiv(point_size, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    HFONT hfont = CreateFontW(font_height, 0, 0, 0, FW_NORMAL, FALSE, FALSE,
                              FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                              CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
                              DEFAULT_PITCH, L"Sector 034");

    SelectObject(hdc, hfont);


    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 0, 255));

    RECT rect = { 0 };
    DrawTextW(hdc, text, -1, &rect, DT_CALCRECT);

    int width  = rect.right  - rect.left,
        height = rect.bottom - rect.top;

    void *bitmap_data = calloc(width*height, sizeof(uint32_t));
    for (int i = 3; i < width*height*4; i+= 4)
        *((uint8_t *)bitmap_data+i) = 0xff;

    HBITMAP hbitmap = CreateBitmap(width, height, 1, 32, bitmap_data);
    SelectObject(hdc, hbitmap);

    DrawTextW(hdc, text, -1, &rect, DT_TOP | DT_LEFT);
    GetBitmapBits(hbitmap, width*height*4, bitmap_data);

    for (int i = 3; i < width*height * 4; i += 4)
        *((uint8_t *)bitmap_data+i) ^= 0xff;

    DeleteObject(hbitmap);
    DeleteObject(hfont);
    DeleteDC(hdc);

    free(text);

    GLuint text_tex_id;
    glGenTextures(1, &text_tex_id);

    glBindTexture(GL_TEXTURE_2D, text_tex_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, bitmap_data);

    free(bitmap_data);

    triMeshT *text_quad = createQuad(1.0f, 1.0f);

    /*text_quad->verts[0].xyz = (vec3) { (width / 320.0f)-1.0f, 1.0f, 0.0f };
    text_quad->verts[1].xyz = (vec3) { -1.0f, 1.0f, 0.0f };
    text_quad->verts[2].xyz = (vec3) { -1.0f, 1.0f - height / 320.0f, 0.0f };
    text_quad->verts[3].xyz = (vec3) { (width / 320.0f) - 1.0f, 1.0f - height / 320.0f, 0.0f };
    */

    updateMesh(text_quad);
    drawMesh(text_quad);
    
    deleteMesh(text_quad);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &text_tex_id);
}

void loadFontFromFile(string const *file_name) {
    file_name = strToWide(file_name);
    assert(AddFontResourceExW(file_name, FR_PRIVATE, NULL) > 0);
    free(file_name);
}
