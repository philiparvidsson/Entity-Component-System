/*------------------------------------------------------------------------------
 * File: graphics.c
 * Created: June 8, 2015
 * Last changed: June 10, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Erbjuder ett enkelt grafikbibliotek för att rita och visa grafik. Biblio-
 *   teket använder OpenGL och renderar således med hårdvara om möjlighet finns.
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
 *   Namnet på klassen som används för att skapa grafikfönstret.
 *------------------------------------*/
#define CLASS_NAME (L"sa14-game1")

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: windowT
 *
 * Description:
 *   Datatyp som representerar ett fönster.
 *------------------------------------*/
typedef struct {
    boolT       is_open; /* Indikerar om fönstret stängts av användaren. */
    const char* title;   /* Fönstrets titel.                             */
    int         width,   /* Fönstrets bredd i antal pixlar.              */
                height;  /* Fönstrets höjd i antal pixlar.               */

    /* Nedan är plattformsspecifika, systemrelaterade variabler. */

    HWND  hwnd;  /* Systemets egna "handtag" till fönstret. */
    HDC   hdc;   /* Den DC (device context) som används.    */
    HGLRC hglrc; /* Den renderingskontext som används.      */
} windowT;

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

windowT window = { 0 };

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*------------------------------------------------------------------------------
 * Nedan är de privata funktionerna som inte är tänkta att användas utanför
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
        /* Användaren har stängt fönstret, så vi markerar det som stängt. */
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
     * Här justerar vi storleken på fönstret så att de angivna dimensionerna
     * inte inkluderar fönsterdekorationer så som kantområden, titelområde m.m.
     */
    assert(AdjustWindowRectEx(&rect, style, FALSE, WS_EX_LEFT));

    /*
     * Eftersom CreateWindowExW()-funktionen vill ha Unicode-strängar, så måste
     * vi konvertera titel-strängen till en Unicode-sträng och skicka den till
     * funktionen.
     */
    size_t   window_name_length = mbstowcs(NULL, title, 0) + 1;
    wchar_t *window_name        = malloc(sizeof(wchar_t) * window_name_length);

    mbstowcs(window_name, title, window_name_length);

    /* Här skapar vi fönstret med ett anrop ner i Windows API. */
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
     * CreateWindowEx()-funktionen kopierar titel-strängen så det är ok att
     * deallokera den här.
     */
    free(window_name);

    /* Om window.hwnd är NULL så skapades inget fönster. */
    assert(window.hwnd != NULL);

    /* Utan detta anrop syns inte fönstret. */
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
 *   Registrerar fönsterklassen.
 *------------------------------------*/
static void registerWindowClass(void) {
    /*
     * Innan vi kan skapa fönster kräver Windows att vi registrerar en fönster-
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
 *   Initierar grafikläge för fönstret.
 *------------------------------------*/
static void setPixelFormat() {
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

    int pixel_format = ChoosePixelFormat(window.hdc, &pfd);

    assert(pixel_format != 0);
    assert(SetPixelFormat(window.hdc, pixel_format, &pfd));
}

/*--------------------------------------
 * Function: unregisterWindowClass()
 *
 * Description:
 *   Avregistrerar fönsterklassen.
 *------------------------------------*/
static void unregisterWindowClass(void) {
    assert(UnregisterClassW(CLASS_NAME, GetModuleHandleW(NULL)));
}

/*------------------------------------------------------------------------------
 * Här nedanför är de publika funktionerna som är tänkta att användas av klient-
 * programmen för att initiera grafikläget m.m.
 *----------------------------------------------------------------------------*/

/*--------------------------------------
 * Function: initGraphics()
 * Parameters:
 *   title   Fönstrets titel.
 *   width   Fönstrets bredd, i antal pixlar.
 *   height  Fönstrets höjd, i antal pixlar.
 *
 * Description:
 *   Skapar ett grafikfönster med de specificerade dimensionerna. Den angivna
 *   bredden och höjden inkluderar inte fönsterdekorationer, utan endast
 *   storleken på klientytan.
 *------------------------------------*/
void initGraphics(const char *title, int width, int height) {
    /*
     * Först måste vi registrera fönsterklassen, annars går det inte att skapa
     * ett fönster.
     */
    registerWindowClass();
    createWindow       (title, width, height);
    setPixelFormat     ();

    window.hglrc = wglCreateContext(window.hdc);

    assert(window.hglrc != NULL);
    assert(wglMakeCurrent(window.hdc, window.hglrc));

    /* Här nedanför initierar vi OpenGL. */

    /* Utan GL_BLEND fungerar inte kantutjämningen för linjer. */
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
 *   Stänger grafikfönstret.
 *------------------------------------*/
void exitGraphics(void) {
    checkInitGraphics();

    wglMakeCurrent  (window.hdc, NULL);
    wglDeleteContext(window.hglrc);

    window.hdc   = NULL;
    window.hglrc = NULL;

    assert(DestroyWindow(window.hwnd));

    /* @To-do: Är det en bra idé att anropa updateWindow() här? */
    updateDisplay();

    /* Vi städar upp efter oss genom att avregistrera fönsterklassen här. */
    unregisterWindowClass();

    /* Nolla all fönsterdata så inga spår finns kvar. */
    memset(&window, 0, sizeof(windowT));
}

/*--------------------------------------
 * Function: clearCanvas()
 * Parameters:
 *   r  Röd färgkomponent.
 *   g  Grön färgkomponent.
 *   b  Blå färgkomponent.
 *
 * Description:
 *   Rensar ritytan til den specificerade färgen.
 *------------------------------------*/
void clearCanvas(float r, float g, float b) {
    checkInitGraphics();

    glClearColor(r, g, b, 1.0f);
    glClear     (GL_COLOR_BUFFER_BIT);
}

/*--------------------------------------
 * Function: setColor()
 * Parameters:
 *   r  Röd färgkomponent.
 *   g  Grön färgkomponent.
 *   b  Blå färgkomponent.
 *
 * Description:
 *   Ändrar färg för nästkommande anrop till ritfunktioner.
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
 *   Uppdaterar grafikfönstret.
 *------------------------------------*/
void updateDisplay(void) {
    checkInitGraphics();

    /*
     * Här ser vi till att fönstret inte hänger sig genom att ta emot och
     * hantera fönstermeddelanden. De skickas vidare av anropet till
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
 *   Sant om grafikfönstret är öppet, annars falskt.
 *
 * Description:
 *   Returnerar sant om grafikfönstret är öppet.
 *------------------------------------*/
boolT windowIsOpen(void) {
    checkInitGraphics();

    return (window.is_open);
}
