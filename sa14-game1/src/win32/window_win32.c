/*------------------------------------------------------------------------------
 * File: window.c
 * Created: June 8, 2015
 * Last changed: June 8, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Funktioner för att skapa fönster.
 *
 *----------------------------------------------------------------------------*/

#ifdef _WIN32

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"
#include "core/debug.h"

#include "ui/window.h"

#include <stdlib.h>

#include <Windows.h>

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

/*--------------------------------------
 * Constant: MAX_WINDOWS
 *
 * Description:
 *  Maximalt antal fönster som får vara öppna vid ett och samma tillfälle.
 *------------------------------------*/
#define MAX_WINDOWS (10)

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: window
 *
 * Description:
 *   Typ som representerar ett fönster.
 *------------------------------------*/
struct windowCDT {
    HWND hwnd;
    boolT is_open;
};

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

/*--------------------------------------
 * Variable: class_registered
 *
 * Description:
 *   Fönsterklassen.
 *------------------------------------*/
boolT class_registered = FALSE;

/*--------------------------------------
 * Variable: num_windows
 *
 * Description:
 *   Antal fönster som är öppna just nu.
 *------------------------------------*/
int num_windows = 0;

/*--------------------------------------
 * Variable: windows
 *
 * Description:
 *   De fönster som är öppna just nu.
 *------------------------------------*/
windowADT windows[MAX_WINDOWS] = { 0 };

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: WindowProc()
 *
 * Description:
 *   http://en.wikipedia.org/wiki/WindowProc
 *------------------------------------*/
LRESULT CALLBACK WindowProc(_In_ HWND   hwnd,
                            _In_ UINT   uMsg,
                            _In_ WPARAM wParam,
                            _In_ LPARAM lParam)
{
    windowADT window = findWindow(hwnd);
    
    switch (uMsg) {

    case WM_CLOSE: {
        window->is_open = FALSE;
        break;
    }

    };

    return (DefWindowProc(hwnd, uMsg, wParam, lParam));
}

/*--------------------------------------
 * Function: findWindow()
 * Parameters:
 *   hwnd  Handtag till fönstret.
 *
 * Returns:
 *   En pekare till fönstret.
 *
 * Description:
 *   Letar upp ett fönster utifrån ett specificerat hwnd-värde.
 *------------------------------------*/
static windowADT findWindow(HWND hwnd) {
    /*
     * En hash-tabell kanske hade varit snyggare, men jag räknar inte med att
     * någon kommer öppna speciellt många fönster samtidigt så detta duger fint.
     */
    for (int i = 0; i < MAX_WINDOWS; i++) {
        if (windows[i] && windows[i]->hwnd == hwnd)
            return windows[i];
    }

    return NULL;
}

/*--------------------------------------
 * Function: registerWindowClass()
 *
 * Description:
 *   Registrerar fönsterklassen.
 *------------------------------------*/
static void registerWindowClass() {
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

    class_registered = TRUE;
}

/*--------------------------------------
 * Function: unregisterWindowClass()
 *
 * Description:
 *   Avregistrerar fönsterklassen.
 *------------------------------------*/
static void unregisterWindowClass() {
    UnregisterClassW(CLASS_NAME, GetModuleHandleW(NULL));

    class_registered = FALSE;
}

/*--------------------------------------
 * Function: createWindow()
 * Parameters:
 *   title   Fönstrets titel.
 *   width   Fönstrets bredd, i antal pixlar.
 *   height  Fönstrets höjd, i antal pixlar.
 *
 * Returns:
 *   En pekare till fönstret.
 *
 * Description:
 *   Skapar ett fönster med de specificerade dimensionerna. Den angivna bredden
 *   och höjden inkluderar inte fönsterdekorationer, utan endast storleken på
 *   klientytan.
 *------------------------------------*/
windowADT createWindow(stringT title, int width, int height) {
    /*
     * Varje fönster behöver en fönsterklass, men flera fönster kan dela på en
     * och samma klass. Så vi har en global variabel för att hålla reda på om vi
     * registrerat vår fönsterklass eller ej.
     */
    if (!class_registered)
        registerWindowClass();

    RECT  rect  = { 0, 0, width, height };
    DWORD style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;

    /*
     * Här justerar vi storleken på fönstret så att de angivna dimensionerna
     * inte inkluderar fönsterdekorationer så som kanter, titel m.m.
     */
    assert(AdjustWindowRectEx(&rect, style, FALSE, WS_EX_LEFT));

    /* Dags att allokera den konkreta datatypen. */
    windowADT window = malloc(sizeof(struct windowCDT));

    /*
     * Eftersom CreateWindowExW()-funktionen vill ha Unicode-strängar, så vi
     * måste konvertera titel-strängen till en Unicode-sträng och skicka den
     * till funktionen.
     */
    size_t   window_name_length = mbstowcs(NULL, title, 0) + 1;
    wchar_t *window_name        = malloc(sizeof(wchar_t) * window_name_length);

    mbstowcs(window_name, title, window_name_length);

    /* Här skapar vi fönstret med ett anrop ner i Windows API. */
    window->hwnd = CreateWindowExW(WS_EX_LEFT,
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

    /* Om window->hwnd är NULL så skapades inget fönster. */
    assert(window->hwnd != NULL);

    /* Utan detta anrop syns inte fönstret. */
    ShowWindow(window->hwnd, SW_SHOW);

    window->is_open = TRUE;
    num_windows++;

    /*
     * Här sparar vi fönsterpekaren på första lediga plats i arrayen, så att vi
     * kan leta upp fönstret utifrån hwnd i WindowProc()-funktionen.
     */
    for (int i = 0; i < MAX_WINDOWS; i++) {
        if (!windows[i]) {
            windows[i] = window;
            break;
        }
    }

    return (window);
}

/*--------------------------------------
 * Function: destroyWindow()
 * Parameters:
 *   window  Fönstret som ska stängas och förstöras.
 *
 * Description:
 *   Stänger och förstör det specificerade fönstret.
 *------------------------------------*/
void destroyWindow(windowADT window) {
    if (!DestroyWindow(window->hwnd))
        return; /* Om vi inte lyckas förstöra fönstret så avbryter vi här. */

    /* @To-do: Är det en bra idé att anropa updateWindow() här? */
    updateWindow(window);

    /*
     * Nu ska fönstret bort ur arrayen så vi letar upp det och nollar den
     * platsen i arrayen.
     */
    for (int i = 0; i < MAX_WINDOWS; i++) {
        if (windows[i] == window) {
            windows[i] = NULL;
            break;
        }
    }

    free(window);

    if (--num_windows == 0)
        unregisterWindowClass();
}

/*--------------------------------------
 * Function: isWindowOpen()
 * Parameters:
 *   window  Fönstret som anropet gäller.
 *
 * Returns:
 *   Sant om det specificerade fönstret är öppet.
 *
 * Description:
 *   Kontrollerar om det specificerade fönstret är öppet.
 *------------------------------------*/
boolT isWindowOpen(windowADT window) {
    return (window->is_open);
}

/*--------------------------------------
 * Function: updateWindow()
 * Parameters:
 *   window  Fönstret som ska uppdateras.
 *
 * Description:
 *   Uppdaterar det specificerade fönstret.
 *------------------------------------*/
void updateWindow(windowADT window) {
    MSG msg;
    while (PeekMessageW(&msg, window->hwnd, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

#endif /* _WIN32 */
