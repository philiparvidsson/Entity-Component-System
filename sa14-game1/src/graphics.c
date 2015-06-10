/*------------------------------------------------------------------------------
 * File: graphics.c
 * Created: June 8, 2015
 * Last changed: June 10, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Erbjuder ett enkelt grafikbibliotek f�r att rita och visa grafik. Biblio-
 *   teket anv�nder OpenGL och renderar s�ledes med h�rdvara om m�jlighet finns.
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "graphics.h"

#include "core/common.h"
#include "core/debug.h"

#include <stdlib.h>
#include <string.h> /* memset() */

#include <Windows.h>
#include <GL/gl.h>

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: CLASS_NAME
 *
 * Description:
 *   Namnet p� klassen som anv�nds f�r att skapa grafikf�nstret.
 *------------------------------------*/
#define CLASS_NAME (L"sa14-game1")

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: windowT
 *
 * Description:
 *   Datatyp som representerar ett f�nster.
 *------------------------------------*/
typedef struct {
    boolT       is_open; /* Indikerar om f�nstret st�ngts av anv�ndaren. */
    const char* title;   /* F�nstrets titel.                             */
    int         width,   /* F�nstrets bredd i antal pixlar.              */
                height;  /* F�nstrets h�jd i antal pixlar.               */

    /* Nedan �r plattformsspecifika, systemrelaterade variabler. */

    HWND  hwnd;  /* Systemets egna "handtag" till f�nstret. */
    HDC   hdc;   /* Den DC (device context) som anv�nds.    */
    HGLRC hglrc; /* Den renderingskontext som anv�nds.      */
} windowT;

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

windowT window = { 0 };

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*------------------------------------------------------------------------------
 * Nedan �r de privata funktionerna som inte �r t�nkta att anv�ndas utanf�r
 * denna modul.
 *----------------------------------------------------------------------------*/

/*--------------------------------------
 * Function: WindowProc()
 *
 * Description:
 *   http://en.wikipedia.org/wiki/WindowProc
 *------------------------------------*/
static LRESULT CALLBACK WindowProc(_In_ HWND   hwnd,
                                   _In_ UINT   uMsg,
                                   _In_ WPARAM wParam,
                                   _In_ LPARAM lParam)
{
    switch (uMsg) {

    case WM_CLOSE: {
        /* Anv�ndaren har st�ngt f�nstret, s� vi markerar det som st�ngt. */
        window.is_open = FALSE;
        break;
    }

    };

    return (DefWindowProc(hwnd, uMsg, wParam, lParam));
}

static void checkInitGraphics(void) {
    if (!window.hwnd)
        error("Graphics not initialized");
}

static void createWindow(const char *title, int width, int height) {
    RECT  rect  = { 0 }; rect.right = width; rect.bottom = height;
    DWORD style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;

    /*
     * H�r justerar vi storleken p� f�nstret s� att de angivna dimensionerna
     * inte inkluderar f�nsterdekorationer s� som kantomr�den, titelomr�de m.m.
     */
    assert(AdjustWindowRectEx(&rect, style, FALSE, WS_EX_LEFT));

    /*
     * Eftersom CreateWindowExW()-funktionen vill ha Unicode-str�ngar, s� m�ste
     * vi konvertera titel-str�ngen till en Unicode-str�ng och skicka den till
     * funktionen.
     */
    size_t   window_name_length = mbstowcs(NULL, title, 0) + 1;
    wchar_t *window_name        = malloc(sizeof(wchar_t) * window_name_length);

    mbstowcs(window_name, title, window_name_length);

    /* H�r skapar vi f�nstret med ett anrop ner i Windows API. */
    window.hwnd = CreateWindowExW(WS_EX_LEFT,
                                  CLASS_NAME,
                                  window_name,
                                  style,
                                  CW_USEDEFAULT,
                                  CW_USEDEFAULT,
                                  rect.right - rect.left,
                                  rect.bottom - rect.top,
                                  HWND_DESKTOP,
                                  NULL,
                                  GetModuleHandleW(NULL),
                                  NULL);

    /*
     * CreateWindowEx()-funktionen kopierar titel-str�ngen s� det �r ok att
     * deallokera den h�r.
     */
    free(window_name);

    /* Om window.hwnd �r NULL s� skapades inget f�nster. */
    assert(window.hwnd != NULL);

    /* Utan detta anrop syns inte f�nstret. */
    ShowWindow(window.hwnd, SW_SHOW);

    window.is_open = TRUE;
    window.width   = width;
    window.height  = height;
    window.title   = title;

    window.hdc = GetDC(window.hwnd);
    
    assert(window.hdc != NULL);
}

/*--------------------------------------
 * Function: registerWindowClass()
 *
 * Description:
 *   Registrerar f�nsterklassen.
 *------------------------------------*/
static void registerWindowClass(void) {
    /*
     * Innan vi kan skapa f�nster kr�ver Windows att vi registrerar en f�nster-
     * klass.
     */

    WNDCLASSEXW wcx = { 0 };

    wcx.cbSize        = sizeof(WNDCLASSEXW);
    wcx.style         = CS_HREDRAW | CS_VREDRAW;
    wcx.lpfnWndProc   = WindowProc;
    wcx.cbClsExtra    = 0;
    wcx.cbWndExtra    = 0;
    wcx.hInstance     = GetModuleHandleW(NULL);
    wcx.hIcon         = LoadIconW(NULL, IDI_APPLICATION);
    wcx.hCursor       = LoadCursorW(NULL, IDC_ARROW);
    wcx.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wcx.lpszMenuName  = NULL;
    wcx.lpszClassName = CLASS_NAME;
    wcx.hIconSm       = NULL;

    assert(RegisterClassExW(&wcx));
}

/*--------------------------------------
 * Function: setPixelFormat()
 * Parameters:
 *
 * Description:
 *   Initierar grafikl�ge f�r f�nstret.
 *------------------------------------*/
static void setPixelFormat() {
    /* Detta kr�vs f�r att ett f�nster ska acceptera OpenGL-l�ge. */

    PIXELFORMATDESCRIPTOR pfd;
    pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion   = 1;
    pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cAlphaBits = 8;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixel_format = ChoosePixelFormat(window.hdc, &pfd);

    assert(pixel_format != 0);
    assert(SetPixelFormat(window.hdc, pixel_format, &pfd));
}

/*--------------------------------------
 * Function: unregisterWindowClass()
 *
 * Description:
 *   Avregistrerar f�nsterklassen.
 *------------------------------------*/
static void unregisterWindowClass(void) {
    assert(UnregisterClassW(CLASS_NAME, GetModuleHandleW(NULL)));
}

/*------------------------------------------------------------------------------
 * H�r nedanf�r �r de publika funktionerna som �r t�nkta att anv�ndas av klient-
 * programmen f�r att initiera grafikl�get m.m.
 *----------------------------------------------------------------------------*/

/*--------------------------------------
 * Function: initGraphics()
 * Parameters:
 *   title   F�nstrets titel.
 *   width   F�nstrets bredd, i antal pixlar.
 *   height  F�nstrets h�jd, i antal pixlar.
 *
 * Description:
 *   Skapar ett grafikf�nster med de specificerade dimensionerna. Den angivna
 *   bredden och h�jden inkluderar inte f�nsterdekorationer, utan endast
 *   storleken p� klientytan.
 *------------------------------------*/
void initGraphics(const char *title, int width, int height) {
    /*
     * F�rst m�ste vi registrera f�nsterklassen, annars g�r det inte att skapa
     * ett f�nster.
     */
    registerWindowClass();
    createWindow       (title, width, height);
    setPixelFormat     ();

    window.hglrc = wglCreateContext(window.hdc);

    assert(window.hglrc != NULL);
    assert(wglMakeCurrent(window.hdc, window.hglrc));

    /* H�r nedanf�r initierar vi OpenGL. */

    /* Utan GL_BLEND fungerar inte kantutj�mningen f�r linjer. */
    glEnable   (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Runda, fina prickar! :-) */
    glEnable(GL_POINT_SMOOTH);
    glHint  (GL_POINT_SMOOTH_HINT, GL_NICEST);

    /* Mjuka, fina linjer! */
    glEnable(GL_LINE_SMOOTH);
    glHint  (GL_LINE_SMOOTH_HINT, GL_NICEST);
}

/*--------------------------------------
 * Function: exitGraphics()
 * Parameters:
 *
 * Description:
 *   St�nger grafikf�nstret.
 *------------------------------------*/
void exitGraphics(void) {
    checkInitGraphics();

    wglMakeCurrent  (window.hdc, NULL);
    wglDeleteContext(window.hglrc);

    window.hdc   = NULL;
    window.hglrc = NULL;

    assert(DestroyWindow(window.hwnd));

    /* @To-do: �r det en bra id� att anropa updateWindow() h�r? */
    updateDisplay();

    /* Vi st�dar upp efter oss genom att avregistrera f�nsterklassen h�r. */
    unregisterWindowClass();

    /* Nolla all f�nsterdata s� inga sp�r finns kvar. */
    memset(&window, 0, sizeof(windowT));
}

/*--------------------------------------
 * Function: clearCanvas()
 * Parameters:
 *   r  R�d f�rgkomponent.
 *   g  Gr�n f�rgkomponent.
 *   b  Bl� f�rgkomponent.
 *
 * Description:
 *   Rensar ritytan til den specificerade f�rgen.
 *------------------------------------*/
void clearCanvas(float r, float g, float b) {
    checkInitGraphics();

    glClearColor(r, g, b, 1.0f);
    glClear     (GL_COLOR_BUFFER_BIT);
}

/*--------------------------------------
 * Function: setColor()
 * Parameters:
 *   r  R�d f�rgkomponent.
 *   g  Gr�n f�rgkomponent.
 *   b  Bl� f�rgkomponent.
 *
 * Description:
 *   �ndrar f�rg f�r n�stkommande anrop till ritfunktioner.
 *------------------------------------*/
void setColor(float r, float g, float b) {
    checkInitGraphics();

    glColor3f(r, g, b);
}

/*--------------------------------------
 * Function: updateDisplay()
 * Parameters:
 *
 * Description:
 *   Uppdaterar grafikf�nstret.
 *------------------------------------*/
void updateDisplay(void) {
    checkInitGraphics();

    /*
     * H�r ser vi till att f�nstret inte h�nger sig genom att ta emot och
     * hantera f�nstermeddelanden. De skickas vidare av anropet till
     * DispatchMessageW() och blir sedan hanterade i WindowProc()-funktionen.
     */
    while (TRUE) {
        MSG msg;
        while (PeekMessageW(&msg, window.hwnd, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        if (window.hdc)
            SwapBuffers(window.hdc);
        break;
    }
}

/*--------------------------------------
 * Function: windowIsOpen()
 * Parameters:
 *
 * Returns:
 *   Sant om grafikf�nstret �r �ppet, annars falskt.
 *
 * Description:
 *   Returnerar sant om grafikf�nstret �r �ppet.
 *------------------------------------*/
boolT windowIsOpen(void) {
    checkInitGraphics();

    return (window.is_open);
}
