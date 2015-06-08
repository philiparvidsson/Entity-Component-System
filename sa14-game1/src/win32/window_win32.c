/*------------------------------------------------------------------------------
 * File: window.c
 * Created: June 8, 2015
 * Last changed: June 8, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Funktioner f�r att skapa f�nster.
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
 *   Namnet p� klassen som anv�nds f�r att skapa grafikf�nstret.
 *------------------------------------*/
#define CLASS_NAME (L"sa14-game1")

/*--------------------------------------
 * Constant: MAX_WINDOWS
 *
 * Description:
 *  Maximalt antal f�nster som f�r vara �ppna vid ett och samma tillf�lle.
 *------------------------------------*/
#define MAX_WINDOWS (10)

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: window
 *
 * Description:
 *   Typ som representerar ett f�nster.
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
 *   F�nsterklassen.
 *------------------------------------*/
boolT class_registered = FALSE;

/*--------------------------------------
 * Variable: num_windows
 *
 * Description:
 *   Antal f�nster som �r �ppna just nu.
 *------------------------------------*/
int num_windows = 0;

/*--------------------------------------
 * Variable: windows
 *
 * Description:
 *   De f�nster som �r �ppna just nu.
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
 *   hwnd  Handtag till f�nstret.
 *
 * Returns:
 *   En pekare till f�nstret.
 *
 * Description:
 *   Letar upp ett f�nster utifr�n ett specificerat hwnd-v�rde.
 *------------------------------------*/
static windowADT findWindow(HWND hwnd) {
    /*
     * En hash-tabell kanske hade varit snyggare, men jag r�knar inte med att
     * n�gon kommer �ppna speciellt m�nga f�nster samtidigt s� detta duger fint.
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
 *   Registrerar f�nsterklassen.
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
 *   Avregistrerar f�nsterklassen.
 *------------------------------------*/
static void unregisterWindowClass() {
    UnregisterClassW(CLASS_NAME, GetModuleHandleW(NULL));

    class_registered = FALSE;
}

/*--------------------------------------
 * Function: createWindow()
 * Parameters:
 *   title   F�nstrets titel.
 *   width   F�nstrets bredd, i antal pixlar.
 *   height  F�nstrets h�jd, i antal pixlar.
 *
 * Returns:
 *   En pekare till f�nstret.
 *
 * Description:
 *   Skapar ett f�nster med de specificerade dimensionerna. Den angivna bredden
 *   och h�jden inkluderar inte f�nsterdekorationer, utan endast storleken p�
 *   klientytan.
 *------------------------------------*/
windowADT createWindow(stringT title, int width, int height) {
    /*
     * Varje f�nster beh�ver en f�nsterklass, men flera f�nster kan dela p� en
     * och samma klass. S� vi har en global variabel f�r att h�lla reda p� om vi
     * registrerat v�r f�nsterklass eller ej.
     */
    if (!class_registered)
        registerWindowClass();

    RECT  rect  = { 0, 0, width, height };
    DWORD style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;

    /*
     * H�r justerar vi storleken p� f�nstret s� att de angivna dimensionerna
     * inte inkluderar f�nsterdekorationer s� som kanter, titel m.m.
     */
    assert(AdjustWindowRectEx(&rect, style, FALSE, WS_EX_LEFT));

    /* Dags att allokera den konkreta datatypen. */
    windowADT window = malloc(sizeof(struct windowCDT));

    /*
     * Eftersom CreateWindowExW()-funktionen vill ha Unicode-str�ngar, s� vi
     * m�ste konvertera titel-str�ngen till en Unicode-str�ng och skicka den
     * till funktionen.
     */
    size_t   window_name_length = mbstowcs(NULL, title, 0) + 1;
    wchar_t *window_name        = malloc(sizeof(wchar_t) * window_name_length);

    mbstowcs(window_name, title, window_name_length);

    /* H�r skapar vi f�nstret med ett anrop ner i Windows API. */
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
     * CreateWindowEx()-funktionen kopierar titel-str�ngen s� det �r ok att
     * deallokera den h�r.
     */
    free(window_name);

    /* Om window->hwnd �r NULL s� skapades inget f�nster. */
    assert(window->hwnd != NULL);

    /* Utan detta anrop syns inte f�nstret. */
    ShowWindow(window->hwnd, SW_SHOW);

    window->is_open = TRUE;
    num_windows++;

    /*
     * H�r sparar vi f�nsterpekaren p� f�rsta lediga plats i arrayen, s� att vi
     * kan leta upp f�nstret utifr�n hwnd i WindowProc()-funktionen.
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
 *   window  F�nstret som ska st�ngas och f�rst�ras.
 *
 * Description:
 *   St�nger och f�rst�r det specificerade f�nstret.
 *------------------------------------*/
void destroyWindow(windowADT window) {
    if (!DestroyWindow(window->hwnd))
        return; /* Om vi inte lyckas f�rst�ra f�nstret s� avbryter vi h�r. */

    /* @To-do: �r det en bra id� att anropa updateWindow() h�r? */
    updateWindow(window);

    /*
     * Nu ska f�nstret bort ur arrayen s� vi letar upp det och nollar den
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
 *   window  F�nstret som anropet g�ller.
 *
 * Returns:
 *   Sant om det specificerade f�nstret �r �ppet.
 *
 * Description:
 *   Kontrollerar om det specificerade f�nstret �r �ppet.
 *------------------------------------*/
boolT isWindowOpen(windowADT window) {
    return (window->is_open);
}

/*--------------------------------------
 * Function: updateWindow()
 * Parameters:
 *   window  F�nstret som ska uppdateras.
 *
 * Description:
 *   Uppdaterar det specificerade f�nstret.
 *------------------------------------*/
void updateWindow(windowADT window) {
    MSG msg;
    while (PeekMessageW(&msg, window->hwnd, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

#endif /* _WIN32 */
