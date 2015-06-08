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

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: window
 *
 * Description:
 *   Typ som representerar ett fönster.
 *-------------------u-----------------*/
struct windowCDT {
    HWND hwnd;
};

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

/*--------------------------------------
 * Type: class_registered
 *
 * Description:
 *   Indikerar om fönsterklassen registrerats.
 *-------------------u-----------------*/
boolT class_registered = FALSE;

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
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
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
    wcx.hInstance     = GetModuleHandle(NULL);
    wcx.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wcx.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wcx.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wcx.lpszMenuName  = NULL;
    wcx.lpszClassName = CLASS_NAME;
    wcx.hIconSm       = NULL;

    assert(RegisterClassExW(&wcx));
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
    if (!class_registered) {
        registerWindowClass();
        class_registered = TRUE;
    }

    RECT  rect  = { 0, 0, width, height };
    DWORD style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;

    assert(AdjustWindowRectEx(&rect, style, FALSE, WS_EX_LEFT));

    windowADT window = malloc(sizeof(struct windowCDT));

    window->hwnd = CreateWindowExW(WS_EX_LEFT,
                                   CLASS_NAME,
                                   title,
                                   style,
                                   CW_USEDEFAULT,
                                   CW_USEDEFAULT,
                                   rect.right - rect.left,
                                   rect.bottom - rect.top,
                                   HWND_DESKTOP,
                                   NULL,
                                   GetModuleHandle(NULL),
                                   NULL);

    assert(window->hwnd != NULL);

    ShowWindow(window->hwnd, SW_SHOW);

    return window;
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
    DestroyWindow(window->hwnd);

    free(window);
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
