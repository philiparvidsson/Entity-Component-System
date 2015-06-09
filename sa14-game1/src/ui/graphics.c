/*------------------------------------------------------------------------------
 * File: graphics.c
 * Created: June 9, 2015
 * Last changed: June 9, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Erbjuder grafikfunktioner.
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "graphics.h"

#include "core/debug.h"

#include "ui/window.h"

/* Windows kräver att vi inkluderar Windows.h innan gl.h. */
#ifdef _WIN32
    #include <Windows.h>
#endif

#include <GL/gl.h>

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: graphicsT_
 *
 * Description:
 *   Typ som representerar ett grafik för ett visst fönster. Den publika delen
 *   är ok att exponera utåt, medan den privata endast används internt.
 *------------------------------------*/
typedef struct {
    /* --- Public --- */

    windowT *window; /* Fönstret som grafikobjektet är initierat för. */

    /* --- Private --- */

    HDC     hdc;
    HGLRC   hglrc;
} graphicsT_;

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

/*--------------------------------------
 * Variable: current
 *
 * Description:
 *   Det grafikobjekt som vi ritar till "just nu."
 *------------------------------------*/
static graphicsT_ *current = NULL;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static void initPixelFormat(windowT *window, HDC hdc) {
    PIXELFORMATDESCRIPTOR pfd;
    pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion   = 1;
    pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cAlphaBits = 8;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixel_format = ChoosePixelFormat(hdc, &pfd);

    assert(pixel_format != 0);
    assert(SetPixelFormat(hdc, pixel_format, &pfd));
}

static void makeCurrent(graphicsT_ *g) {
    if (g == current)
        return;

    current = g;

    if (g)
        wglMakeCurrent(g->hdc, g->hglrc);
}

graphicsT *initGraphics(windowT *window) {
    HDC hdc = GetDC(_getHwnd(window));

    initPixelFormat(window, hdc);

    HGLRC hglrc = wglCreateContext(hdc);

    assert(hglrc != NULL);
    assert(wglMakeCurrent(hdc, hglrc));

    graphicsT_ *g = malloc(sizeof(*(graphicsT_ *)NULL));

    g->window = window;
    g->hdc    = hdc;
    g->hglrc  = hglrc;

    assert(g != NULL);

    /* Här nedanför initierar vi OpenGL. */

    /* Utan GL_BLEND fungerar inte kantutjämningen för linjer. */
    glEnable   (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Runda, fina prickar! :-) */
    glEnable   (GL_POINT_SMOOTH);
    glHint     (GL_POINT_SMOOTH_HINT, GL_NICEST);

    /* Mjuka, fina linjer! */
    glEnable   (GL_LINE_SMOOTH);
    glHint     (GL_LINE_SMOOTH_HINT, GL_NICEST);

    return ((graphicsT *)g);
}

void freeGraphics(graphicsT *g) {
    if (g == current) {
        wglMakeCurrent(((graphicsT_ *)g)->hdc, NULL);
        current = NULL;
    }

    wglDeleteContext(((graphicsT_ *)g)->hglrc);

    free(g);
}

void clearCanvas(graphicsT *g, float red, float green, float blue) {
    makeCurrent((graphicsT_ *)g);

    glClearColor(red, green, blue, 1.0f);
    glClear     (GL_COLOR_BUFFER_BIT);
}

void setColor(graphicsT *g, float red, float green, float blue) {
    makeCurrent((graphicsT_ *)g);

    glColor3f(red, green, blue);
}

void swapBuffers(graphicsT *g) {
    makeCurrent((graphicsT_ *)g);

    SwapBuffers(((graphicsT_ *)g)->hdc);
}
