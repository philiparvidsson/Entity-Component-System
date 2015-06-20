/*------------------------------------------------------------------------------
 * File: graphics.c
 * Created: June 8, 2015
 * Last changed: June 20, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   This file implements a graphics library using modern OpenGL, supporting
 *   shaders and much more, using a right-handed coordinate system.
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "graphics.h"

#include "core/common.h"
#include "core/debug.h"

#include <GL/glew.h>

#include <windows.h>

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: ClassName
 *
 * Description:
 *   The name of the class used to create the graphics window.
 *------------------------------------*/
#define ClassName ("sa14-game1")

#define WindowStyle (~WS_MAXIMIZEBOX & WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME)

#define WindowStyleEx WS_EX_LEFT

/*--------------------------------------
 * Constant: DefaultFPS
 *
 * Description:
 *   The number of frames to display each second, by defaukt. This can be
 *   configured with the setFrameRate() function.
 *------------------------------------*/
#define DefaultFPS (60.0f)

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: windowT
 *
 * Description:
 *   Represents the graphics window.
 *------------------------------------*/
typedef struct windowT {
    string const *title;      // Window title.
    int           width,      // Window width, in pixels.
                  height;     // Window height, in pixels.
    int           frame_time;

    // Below are platform specific system fields.

    HWND  hwnd;  // Window handle.
    HDC   hdc;   // Device context (DC) handle.
    HGLRC hglrc; // OpenGL render context.

    LARGE_INTEGER last_update; // Timestap of last graphics update.
} windowT;

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

windowT *window = NULL;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static string *createWideString(string const *str) {
    size_t  len   = mbstowcs(NULL, str, 0)+1;
    wchar_t *wstr = malloc(sizeof(wchar_t) * len);

    mbstowcs(wstr, str, len);

    return (wstr);
}

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
        window->hwnd = NULL;
        break;
    }

    };

    return (DefWindowProc(hwnd, uMsg, wParam, lParam));
}

static void registerWindowClass(void) {
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
    wcx.lpszClassName = ClassName;
    wcx.hIconSm       = NULL;

    assert(RegisterClassExW(&wcx));
}

static void unregisterWindowClass(void) {
    assert(UnregisterClassW(ClassName, GetModuleHandleW(NULL)));
}

static void createWindow(string const *title, int width, int height) {
    if (window)
        error("createWindow() was called twice");

    RECT  rect  = { 0 };
    rect.right  = width;
    rect.bottom = height;

    assert(AdjustWindowRectEx(&rect, WindowStyle, FALSE, WindowStyleEx));

    registerWindowClass();

    window = malloc(sizeof(windowT));

    wchar_t *window_name = createWideString(title);

    window->hwnd = CreateWindowExW(WindowStyleEx,
                                   ClassName,
                                   window_name,
                                   WindowStyle,
                                   CW_USEDEFAULT,
                                   CW_USEDEFAULT,
                                   rect.right - rect.left,
                                   rect.bottom - rect.top,
                                   HWND_DESKTOP,
                                   NULL,
                                   GetModuleHandleW(NULL),
                                   NULL);

    free(window_name);

    assert(window->hwnd != NULL);

    window->hdc = GetDC(window->hwnd);

    assert(window->hdc != NULL);

    ShowWindow(window->hwnd, SW_SHOW);

    window->width  = width;
    window->height = height;
    window->title  = title;
}

static void destroyWindow(void) {
    if (!window)
        return;
    
    if (window->hwnd)
        assert(DestroyWindow(window->hwnd));

    free(window);
    window = NULL;

    unregisterWindowClass();
}

static void updateWindow(void) {
    MSG msg;
    while (window && PeekMessageW(&msg, window->hwnd, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

static void setupPixelFormat(void) {
    PIXELFORMATDESCRIPTOR pfd = { 0 };

    pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion   = 1;
    pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cAlphaBits = 8;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixel_format = ChoosePixelFormat(window->hdc, &pfd);

    assert(pixel_format != 0);
    assert(SetPixelFormat(window->hdc, pixel_format, &pfd));
}

/*--------------------------------------
 * Function: setFrameRate(fps)
 * Parameters:
 *   fps  The number of frames to display each second.
 *
 * Description:
 *   Sets the number of frames to display each second. Specify a framerate of
 *   zero to disable fps synchronization.
 *
 * Usage:
 *   setFrameRate(60.0f);
 *------------------------------------*/
static void setFrameRate(float fps) {
    // Zero fps means we disable fps synchronization.
    if (fps == 0.0f) {
        window->frame_time = -1;
        return;
    }

    LARGE_INTEGER freq;
    assert(QueryPerformanceFrequency(&freq));

    window->frame_time = (int)(freq.QuadPart/fps);

    assert(QueryPerformanceCounter(&window->last_update));
}

void initGraphics(string const *title, int width, int height) {
    createWindow(title, width, height);
    setupPixelFormat();

    window->hglrc = wglCreateContext(window->hdc);

    assert(window->hglrc != NULL);
    assert(wglMakeCurrent(window->hdc, window->hglrc));
    assert(glewInit() == GLEW_OK);

    // We need to enable GL_BLEND for transparency.
    glEnable   (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Ignore triangles that are "looking away" from the camera.
    glEnable(GL_CULL_FACE);

    // Enable z-buffering and make sure it's right-handed (i.e. -z is into the
    // screen).
    glEnable    (GL_DEPTH_TEST);
    glClearDepth(0.0f);
    glDepthFunc (GL_GREATER);

    setFrameRate(DefaultFPS);
}

/*--------------------------------------
 * Function: exitGraphics()
 * Parameters:
 *
 * Description:
 *   Closes the window and exits graphics mode.
 *
 * Usage:
 *   exitGraphics();
 *------------------------------------*/
void exitGraphics(void) {
    assert(wglMakeCurrent(window->hdc, NULL));
    assert(wglDeleteContext(window->hglrc));

    window->hdc   = NULL;
    window->hglrc = NULL;

    destroyWindow();
}

/*--------------------------------------
 * Function: clearDisplay()
 * Parameters:
 *   r  Red color component.
 *   g  Green color component.
 *   b  Blue color component..
 *
 * Description:
 *   Clears the display to the specified color.
 *
 * Usage:
 *   clearDisplay(1.0f, 1.0f, 1.0f);
 *------------------------------------*/
void clearDisplay(float r, float g, float b) {
    glClearColor(r, g, b, 1.0f);
    glClear     (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/*--------------------------------------
 * Function: updateDisplay()
 * Parameters:
 *
 * Description:
 *   Updates the display by processing pending window events and swapping the
 *   graphics buffer into the graphics window. The function also waits a certain
 *   amount of time before returning, to maintain the FPS specified with the
 *   setFrameRate() function.
 *
 * Usage:
 *   updateDisplay();
 *------------------------------------*/
void updateDisplay(void) {
    // Swap the buffer into the window if there's a device context handle.
    if (window->hdc)
        assert(SwapBuffers(window->hdc));

    // We enter a loop and stay in it just long enough to time and synchronize
    // the FPS, so we get a set amount of frames displayed each second.
    LARGE_INTEGER perf_count;
    do {
        updateWindow();

        // If window is NULL here, the user has closed the window and
        // destroyWindow() has been called by the WindowProc() function,
        // which, in turn, has been invoekd by the DispatchMessageW() function.
        if (!window)
            return;

        // We calculate how much time has passed since our latest display
        // update. If enough time has passed, we've displayed this frame exactly
        // the amount of time needed to maintain a set frame interval, so we can
        // exit the loop.
        assert(QueryPerformanceCounter(&perf_count));
        perf_count.QuadPart -= window->last_update.QuadPart;
    } while (perf_count.QuadPart < window->frame_time);

    assert(QueryPerformanceCounter(&window->last_update));
}

bool windowIsFocused(void) {
    return (window && (window->hwnd == GetFocus()));
}

bool windowIsOpen(void) {
    return (window && (window->hwnd != NULL));
}
