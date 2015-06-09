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
 * MACROS
 *----------------------------------------------*/

/*--------------------------------------
 * Macro: readHwnd()
 * Parameters:
 *   wnd  Det fönster vars hwnd-värde ska läsas ut.
 *
 * Description:
 *   Läser ut det specificerade fönstrets hwnd-värde. Detta är lite av ett hack
 *   för att komma åt fönstrets privata data.
 *------------------------------------*/
#define readHwnd(wnd) (*(HWND *)((windowT *)wnd+1))

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

    HDC     hdc;   /* Den DC (device context) som används. */
    HGLRC   hglrc; /* Den renderingskontext som används. */
} graphicsT_;

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

/*--------------------------------------
 * Variable: curr_graphics
 *
 * Description:
 *   Det grafikobjekt som vi ritar till "just nu."
 *------------------------------------*/
static graphicsT_ *curr_graphics = NULL;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: initPixelFormat()
 * Parameters:
 *   hdc  Den DC (device context) soma ska användas i fönstret.
 *
 * Description:
 *   Initierar grafikläge för det specificerade fönstret.
 *------------------------------------*/
static void initPixelFormat(HDC hdc) {
    /* Detta krävs för att ett fönster ska acceptera OpenGL-läge. */

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

/*--------------------------------------
 * Function: makeCurrent()
 * Parameters:
 *   g  Det grafikobjekt som nästkommande anrop till ritfunktioner ska använda.
 *
 * Description:
 *   Sätter det specificerade grafikobjektet till det grafikobjekt som används
 *   för ritfunktioner.
 *------------------------------------*/
static void makeCurrent(graphicsT_ *g) {
    if (g == curr_graphics)
        return;

    curr_graphics = g;

    if (g)
        wglMakeCurrent(g->hdc, g->hglrc);
}

/*--------------------------------------
 * Function: initGraphics()
 * Parameters:
 *   window  Det fönster som grafikobjektet ska initieras för.
 *
 * Returns:
 *   En pekare till grafikobjektet.
 *
 * Description:
 *   Initierar grafik för det specificerade fönstret.
 *------------------------------------*/
graphicsT *initGraphics(windowT *window) {
    HDC hdc = GetDC(readHwnd(window));

    initPixelFormat(hdc);

    HGLRC hglrc = wglCreateContext(hdc);

    assert(hglrc != NULL);
    assert(wglMakeCurrent(hdc, hglrc));

    graphicsT_ *g = malloc(sizeof(graphicsT_));

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

/*--------------------------------------
 * Function: freeGraphics()
 * Parameters:
 *   g  Det grafikobjekt som ska frias.
 *
 * Description:
 *   Frigör det specificerade grafikobjektet.
 *------------------------------------*/
void freeGraphics(graphicsT *g) {
    /*
     * Om vi friar det grafikobjekt som är aktivt så ser vi till att sätta det
     * till NULL och avaktivera det.
     */
    if ((graphicsT_ *)g == curr_graphics) {
        wglMakeCurrent(((graphicsT_ *)g)->hdc, NULL);
        curr_graphics = NULL;
    }

    wglDeleteContext(((graphicsT_ *)g)->hglrc);

    free(g);
}

/*--------------------------------------
 * Function: clearCanvas()
 * Parameters:
 *   g      Det grafikobjekt som anropet gäller.
 *   red    Röd färgkomponent.
 *   green  Grön färgkomponent.
 *   blue   Blå färgkomponent.
 *
 * Description:
 *   Rensar ritytan til den specificerade färgen.
 *------------------------------------*/
void clearCanvas(graphicsT *g, float red, float green, float blue) {
    makeCurrent((graphicsT_ *)g);

    glClearColor(red, green, blue, 1.0f);
    glClear     (GL_COLOR_BUFFER_BIT);
}

/*--------------------------------------
 * Function: setColor()
 * Parameters:
 *   g      Det grafikobjekt som anropet gäller.
 *   red    Röd färgkomponent.
 *   green  Grön färgkomponent.
 *   blue   Blå färgkomponent.
 *
 * Description:
 *   Ändrar färg för nästkommande anrop till ritfunktioner.
 *------------------------------------*/
void setColor(graphicsT *g, float red, float green, float blue) {
    makeCurrent((graphicsT_ *)g);

    glColor3f(red, green, blue);
}

/*--------------------------------------
 * Function: updateGraphics()
 * Parameters:
 *   g      Det grafikobjekt som anropet gäller.
 *
 * Description:
 *   Uppdaterar grafiken och ritar upp den i det associerade fönstret.
 *------------------------------------*/
void updateGraphics(graphicsT *g) {
    makeCurrent((graphicsT_ *)g);

    SwapBuffers(((graphicsT_ *)g)->hdc);
}
